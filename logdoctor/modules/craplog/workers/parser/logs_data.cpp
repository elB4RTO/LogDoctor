
#include "logs_data.h"

#include "modules/exceptions.h"

#include "modules/craplog/modules/lib.h"
#include "modules/craplog/utilities/datetime.h"

#include "utilities/strings.h"
#include "utilities/vectors.h"



LogLineData::LogLineData(const std::string& line, const LogsFormat& logs_format)
{
    using F = LogsFormatField;

    std::size_t start, stop{logs_format.initial.size()},
           sep_i{0};
    const std::size_t line_size{ line.size()-1ul },
                      max_seps{ logs_format.separators.size() },
                      n_seps{ max_seps-1ul };

    while ( sep_i <= max_seps ) {
        // split fields
        start = stop; // stop updated at the end of the loop

        std::string_view sep;
        if ( sep_i <= n_seps ) {
            sep = logs_format.separators.at( sep_i );
            stop = line.find( sep, start );
            if ( stop == std::string::npos ) {
                throw LogParserException( "Separator not found", std::string{sep} );
            }
        } else if ( sep_i == max_seps ) {
            // final separator
            sep = logs_format.final;
            if ( sep.empty() ) {
                stop = line_size+1ul;
            } else {
                stop = line.find( sep, start );
                if ( stop == std::string::npos ) {
                    throw LogParserException( "Final separator not found", std::string{sep} );
                }
            }
        } else [[unlikely]] {
            // should be unreachable
            throw ("Unexpected section reached");
        }
        const std::size_t sep_size{ sep.size() };

        // get the field
        const F fld{ logs_format.fields.at( sep_i ) };
        if ( _DISCARDED | fld ) {
            // only parse the considered fields

            std::string fld_str{ line.substr(start, stop-start) };

            if ( sep_i < n_seps ) {
                // not the last separator, check for mistakes
                std::size_t aux_stop = stop;

                if ( (_MAY_HAVE_SPACES & fld) && sep == " " ) {
                    // check the fields with whitespace-separated values
                    const std::size_t n{ fld == request_full ? 2ul
                                       : fld & _COUNT_SPACES };

                    std::size_t c{ StringOps::count( fld_str, ' ' ) };
                    if ( c < n ) {
                        // loop until the correct number of whitespaces is reached
                        std::size_t aux_start = line[stop+1ul] == ' ' ? stop : stop+1ul;
                        while ( c < n ) {
                            aux_stop = line.find( sep, aux_start );
                            if ( aux_stop == std::string::npos ) {
                                // not found
                                throw LogParserException( "Separator not found", std::string{sep} );
                            }
                            aux_start = aux_stop+1ul;
                            ++c;
                        }
                    } else if ( c > n ) [[unlikely]] {
                        // should be unreachable
                        throw LogParserException( "Unexpected count for separator", std::string{sep} );
                    }

                } else if ( sep.front() == '"' && fld == F::user_agent ) {
                    // atm the only support is for escaped quotes
                    if ( fld_str.back() == '\\' ) {
                        // the found separator is not actually the separator but is part of the user-agent string
                        // keep searching until the real separator is found
                        std::size_t aux_start = stop + sep_size;
                        while (true) {
                            aux_stop = line.find( sep, aux_start );
                            if ( aux_stop == std::string::npos ) {
                                // not found
                                throw LogParserException( "Separator not found", std::string{sep} );
                            } else if ( line.at( aux_stop-1ul ) != '\\' ) {
                                // non-backslashed quotes, real separator found (hopefully)
                                break;
                            }
                            aux_start = aux_stop + sep_size;
                        }
                    }
                }

                // finally update if needed
                if ( aux_stop > stop ) {
                    stop = aux_stop;
                    fld_str = line.substr(start, stop-start);
                }
            }

            if ( ! fld_str.empty() ) {
                // process the field
                if ( _NO_PARSE_NEEDED & fld ) {
                    // no need to process, append directly if non-empty
                    if ( fld == request_query && fld_str == "-" ) {
                        continue;
                    }
                    this->data( fld ) = FieldData( std::move(fld_str) );

                } else {
                    // process the field

                    // process the date to get year, month, day, hour and minute
                    if ( _DATE_TIME & fld ) {
                        auto dt = DateTimeOps::processDateTime( fld_str, fld ); // cut away the "date_time_" part
                        if ( auto& year{ dt.at(0) }; !year.empty() ) {
                            // year
                            this->year = FieldData( std::move(year) );
                        }
                        if ( auto& month{ dt.at(1) }; !month.empty() ) {
                            // month
                            this->month = FieldData( std::move(month) );
                        }
                        if ( auto& day{ dt.at(2) }; !day.empty() ) {
                            // day
                            this->day = FieldData( std::move(day) );
                        }
                        if ( auto& hour{ dt.at(3) }; !hour.empty() ) {
                            // hour
                            this->hour = FieldData( std::move(hour) );
                        }
                        if ( auto& minute{ dt.at(4) }; !minute.empty() ) {
                            // minute
                            this->minute = FieldData( std::move(minute) );
                        }
                        if ( auto& second{ dt.at(5) }; !second.empty() ) {
                            // second
                            this->second = FieldData( std::move(second) );
                        }


                    // process the time taken to convert to milliseconds
                    } else if ( _TIME_TAKEN & fld ) {
                        float t{ std::stof( fld_str ) };
                        if ( fld == time_taken_us ) {
                            // from microseconds
                            t /= 1000.0f;
                        } else if ( fld & time_taken_s ) {
                            // from seconds or seconds.milliseconds
                            t *= 1000.0f;
                        }
                        this->time_taken = FieldData( std::to_string( static_cast<int>( t ) ) );


                    // process the request to get the protocol, method, resource and query
                    } else if ( fld == request_full ) {
                        // check whether the request string has the proper number of spaces
                        const std::size_t n_spaces{ StringOps::count( fld_str, ' ' ) };

                        if ( n_spaces == 0ul ) [[unlikely]] {
                            // no spaces
                            if ( VecOps::contains( this->valid_methods, fld_str ) ) {
                                this->method = FieldData( std::move(fld_str) );
                            } else if ( VecOps::contains( this->valid_protocols, fld_str ) ) {
                                this->protocol = FieldData( std::move(fld_str) );
                            } else {
                                this->storeUriQuery( std::move(fld_str) );
                            }

                        } else if ( n_spaces == 1ul ) [[unlikely]] {
                            // 1 field is missing
                            this->storeMalformedRequestOneSpace( std::move(fld_str) );

                        } else if ( n_spaces > 2ul ) [[unlikely]] {
                            // most likely a malicious attempt
                            if ( sep == " " ) {
                                // hard to say how to handle it properly
                                throw LogParserException( "Malformed request string", fld_str );
                            }
                            this->storeMalformedRequestMultiSpace( std::move(fld_str) );

                        } else [[likely]] {
                            // correct amount of spaces
                            const std::size_t aux_stop1{ fld_str.find( ' ' ) },
                                              aux_start{ aux_stop1+1ul },
                                              aux_stop2{ fld_str.find( ' ', aux_start ) };

                            if ( aux_stop1 > 0ul && aux_stop2 > aux_start ) [[likely]] {
                                std::string method{ fld_str.substr( 0ul, aux_stop1 ) };
                                std::string protocol{ fld_str.substr( aux_stop2+1ul ) };
                                if ( VecOps::contains( this->valid_methods, method )
                                  && VecOps::contains( this->valid_protocols, protocol ) ) [[likely]] {
                                    this->method = FieldData( std::move(method) );
                                    this->protocol = FieldData( std::move(protocol) );
                                    this->storeUriQuery( fld_str.substr( aux_start, aux_stop2-aux_start ) );

                                } else [[unlikely]] {
                                    this->storeMalformedRequestMultiSpace( std::move(fld_str) );
                                }

                            } else [[unlikely]] {
                                this->storeMalformedRequestMultiSpace( std::move(fld_str) );
                            }
                        }


                    // process the request to get uri and query
                    } else if ( fld == request_uri_query ) {
                        // search for the query
                        std::string uri, query;
                        const std::size_t aux_{ fld_str.find( '?' ) };
                        if ( aux_ != std::string::npos ) {
                            uri   = fld_str.substr( 0ul, aux_ );
                            query = fld_str.substr( aux_+1ul  );
                        } else {
                            // query not found
                            uri = fld_str;
                        }
                        if ( ! uri.empty() ) {
                            this->uri = FieldData( std::move(uri) );
                        }
                        if ( ! query.empty() ) {
                            this->query = FieldData( std::move(query) );
                        }


                    // something went wrong
                    } else {
                        // hmmm.. no...
                        throw LogParserException( "Unexpected LogFormatField", std::to_string(fld) );
                    }
                }
            }
        }

        // update the stop for the next start
        stop += sep_size;
        ++sep_i;
        if ( stop > line_size ) {
            // this was the final separator
            break;
        }

    }
}

void LogLineData::storeUriQuery(std::string&& str) noexcept
{
    if ( ! str.empty() ) {
        if ( const auto pos{ str.find( '?' ) }; pos != std::string::npos ) {
            this->uri   = FieldData( str.substr( 0ul, pos ) );
            this->query = FieldData( str.substr( pos+1ul  ) );
        } else {
            this->uri = FieldData( std::move(str) );
        }
    }
}

void LogLineData::storeMalformedRequestOneSpace(std::string&& str) noexcept
{
    const std::size_t pos{ str.find( ' ' ) };
    std::string field1{ str.substr( 0ul, pos ) },
                field2{ str.substr( pos+1 ) };
    const bool is_method1{ VecOps::contains( this->valid_methods, field1 ) },
               is_method2{ VecOps::contains( this->valid_methods, field2 ) },
               is_protocol1{ VecOps::contains( this->valid_protocols, field1 ) },
               is_protocol2{ VecOps::contains( this->valid_protocols, field2 ) };

    /**
    ***  P\M
    ***        00   01   10   11
    ***
    ***   00   U+   UM   MU   U+
    ***
    ***   01   UP   --   MP   --
    ***
    ***   10   PU   PM   --   --
    ***
    ***   11   U+   --   --   --
    **/

    if ( is_method1 ) {
        if ( is_method2 ) {
            // uri = 12
            this->storeUriQuery( StringOps::strip( str ) );
        } else if ( is_protocol2 ) {
            // method = 1 // protocol = 2
            this->method = FieldData( std::move(field1) );
            this->protocol = FieldData( std::move(field2) );
        } else {
            // method = 1 // uri = 2
            this->method = FieldData( std::move(field1) );
            this->storeUriQuery( std::move(field2) );
        }
    } else if ( is_method2 ) {
        if ( is_protocol1 ) {
            // protocol = 1 // method = 2
            this->protocol = FieldData( std::move(field1) );
            this->method = FieldData( std::move(field2) );
        } else {
            // uri = 1 // method = 2
            this->storeUriQuery( std::move(field1) );
            this->method = FieldData( std::move(field2) );
        }
    } else if ( is_protocol1 ) {
        if ( is_protocol2 ) {
            // uri = 12
            this->storeUriQuery( StringOps::strip( str ) );
        } else {
            // protocol = 1 // uri = 2
            this->protocol = FieldData( std::move(field1) );
            this->storeUriQuery( std::move(field2) );
        }
    } else if ( is_protocol2 ) {
        // uri = 1 // protocol = 2
        this->storeUriQuery( std::move(field1) );
        this->protocol = FieldData( std::move(field2) );
    } else {
        // uri = 12
        this->storeUriQuery( StringOps::strip( str ) );
    }
}

void LogLineData::storeMalformedRequestMultiSpace(std::string&& str) noexcept
{
    const std::size_t pos1{ str.find( ' ' ) },
                      pos2{ str.rfind( ' ' ) };
    std::string field1{ str.substr( 0ul, pos1 ) };
    std::string field2{ StringOps::strip( str.substr( pos1+1ul, pos2-pos1-1ul ) ) };
    std::string field3{ str.substr( pos2+1ul ) };
    const bool is_method1{ VecOps::contains( this->valid_methods, field1 ) },
               is_method2{ VecOps::contains( this->valid_methods, field2 ) },
               is_method3{ VecOps::contains( this->valid_methods, field3 ) },
               is_protocol1{ VecOps::contains( this->valid_protocols, field1 ) },
               is_protocol2{ VecOps::contains( this->valid_protocols, field2 ) },
               is_protocol3{ VecOps::contains( this->valid_protocols, field3 ) };

    /**
    ***  P\M
    ***        000   001   010   011   100   101   110   111
    ***
    ***  000   +U+   +UM   +U+   +U+   MU+   +U+   +U+   +U+
    ***
    ***  001   +UP   ---   UMP   ---   MUP   ---   +UP   ---
    ***
    ***  010   +U+   UPM   ---   ---   MPU   +U+   ---   ---
    ***
    ***  011   +U+   ---   ---   ---   MU+   ---   ---   ---
    ***
    ***  100   PU+   PUM   PMU   PU+   ---   ---   ---   ---
    ***
    ***  101   +U+   ---   +U+   ---   ---   ---   ---   ---
    ***
    ***  110   +U+   +UM   ---   ---   ---   ---   ---   ---
    ***
    ***  111   +U+   ---   ---   ---   ---   ---   ---   ---
    **/

    if ( is_method1 && is_method3 ) {
        // uri = 123
        this->storeUriQuery( StringOps::strip( str ) );

    } else if ( is_method1 && is_method2 ) {
        if ( is_protocol3 ) {
            // uri = 12 // protocol = 3
            this->storeUriQuery( StringOps::strip( str.substr( 0ul, pos2 ) ) );
            this->protocol = FieldData( std::move(field3) );
        } else {
            // uri = 123
            this->storeUriQuery( StringOps::strip( str ) );
        }

    } else if ( is_method2 && is_method3 ) {
        if ( is_protocol1 ) {
            // protocol = 1 // uri = 23
            this->protocol = FieldData( std::move(field1) );
            this->storeUriQuery( StringOps::strip( str.substr( pos1+1ul ) ) );
        } else {
            // uri = 123
            this->storeUriQuery( StringOps::strip( str ) );
        }

    } else if ( is_method1 ) {
        if ( is_protocol2 && !is_protocol3 ) {
            // method = 1 // protocol = 2 // uri = 3
            this->method = FieldData( std::move(field1) );
            this->protocol = FieldData( std::move(field2) );
            this->storeUriQuery( std::move(field3) );
        } else if ( is_protocol3 && !is_protocol2 ) {
            // method = 1 // uri = 2 // protocol = 3
            this->method = FieldData( std::move(field1) );
            this->storeUriQuery( std::move(field2) );
            this->protocol = FieldData( std::move(field3) );
        } else {
            // method = 1 // uri = 23
            this->method = FieldData( std::move(field1) );
            this->storeUriQuery( StringOps::strip( str.substr( pos1+1ul ) ) );
        }

    } else if ( is_method2 ) {
        if ( is_protocol1 && !is_protocol3 ) {
            // protocol = 1 // method = 2 // uri = 3
            this->protocol = FieldData( std::move(field1) );
            this->method = FieldData( std::move(field2) );
            this->storeUriQuery( std::move(field3) );
        } else if ( is_protocol3 && !is_protocol1 ) {
            // uri = 1 // method = 2 // protocol = 3
            this->storeUriQuery( std::move(field1) );
            this->method = FieldData( std::move(field2) );
            this->protocol = FieldData( std::move(field3) );
        } else {
            // uri = 123
            this->storeUriQuery( StringOps::strip( str ) );
        }

    } else if ( is_method3 ) {
        if ( is_protocol1 && !is_protocol2 ) {
            // protocol = 1 // uri = 2 // method = 3
            this->protocol = FieldData( std::move(field1) );
            this->storeUriQuery( std::move(field2) );
            this->method = FieldData( std::move(field3) );
        } else if ( is_protocol2 && !is_protocol1 ) {
            // uri = 1 // protocol = 2 // method = 3
            this->storeUriQuery( std::move(field1) );
            this->protocol = FieldData( std::move(field2) );
            this->method = FieldData( std::move(field3) );
        } else {
            // uri = 12 // method = 3
            this->storeUriQuery( StringOps::strip( str.substr( 0ul, pos2 ) ) );
            this->method = FieldData( std::move(field3) );
        }

    } else if ( is_protocol1 && is_protocol3 ) {
        // uri = 123
        this->storeUriQuery( StringOps::strip( str ) );

    } else if ( is_protocol1 && is_protocol2 ) {
        if ( is_method3 ) {
            // uri = 12 // method = 3
            this->storeUriQuery( StringOps::strip( str.substr( 0ul, pos2 ) ) );
            this->method = FieldData( std::move(field3) );
        } else {
            // uri = 123
            this->storeUriQuery( StringOps::strip( str ) );
        }

    } else if ( is_protocol2 && is_protocol3 ) {
        if ( is_method1 ) {
            // method = 1 // uri = 23
            this->method = FieldData( std::move(field1) );
            this->storeUriQuery( StringOps::strip( str.substr( pos1+1ul ) ) );
        } else {
            // uri = 123
            this->storeUriQuery( StringOps::strip( str ) );
        }

    } else if ( is_protocol1 ) {
        if ( !is_method2 && !is_method3 ) {
            // protocol = 1 // uri = 23
            this->protocol = FieldData( std::move(field1) );
            this->storeUriQuery( StringOps::strip( str.substr( pos1+1ul ) ) );
        } else {
            // uri = 123
            this->storeUriQuery( StringOps::strip( str ) );
        }

    } else if ( is_protocol3 ) {
        if ( !is_method2 && !is_method1 ) {
            // uri = 12 // protocol = 3
            this->storeUriQuery( StringOps::strip( str.substr( 0ul, pos2 ) ) );
            this->protocol = FieldData( std::move(field3) );
        } else {
            // uri = 123
            this->storeUriQuery( StringOps::strip( str ) );
        }

    } else {
        // uri = 123
        this->storeUriQuery( StringOps::strip( str ) );
    }
}

std::size_t LogLineData::size() const noexcept
{
    return this->year
         + this->month
         + this->day
         + this->hour
         + this->minute
         + this->second
         + this->protocol
         + this->method
         + this->uri
         + this->query
         + this->response_code
         + this->time_taken
         + this->bytes_sent
         + this->bytes_received
         + this->referrer
         + this->client
         + this->user_agent
         + this->cookie;
}

FieldData& LogLineData::data(const LogsFormatField id)
{
    using F = LogsFormatField;

    switch (id) {
        case F::date_time_year:
            return this->year;
        case F::date_time_month:
            return this->month;
        case F::date_time_day:
            return this->day;
        case F::date_time_hour:
            return this->hour;
        case F::date_time_minute:
            return this->minute;
        case F::date_time_second:
            return this->second;
        case F::request_protocol:
            return this->protocol;
        case F::request_method:
            return this->method;
        case F::request_uri:
            return this->uri;
        case F::request_query:
            return this->query;
        case F::response_code:
            return this->response_code;
        case F::time_taken_ms:
            return this->time_taken;
        case F::bytes_sent:
            return this->bytes_sent;
        case F::bytes_received:
            return this->bytes_received;
        case F::referer:
            return this->referrer;
        case F::client:
            return this->client;
        case F::user_agent:
            return this->user_agent;
        case F::cookie:
            return this->cookie;
        default:
            throw LogParserException( "Unexpected LogFormatField", std::to_string(id) );
    }
}
