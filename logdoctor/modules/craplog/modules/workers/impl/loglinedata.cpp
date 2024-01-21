
#include "../lib.h"

#include "modules/exceptions.h"

#include "modules/craplog/modules/lib.h"
#include "modules/craplog/modules/datetime.h"

#include "utilities/strings.h"
#include "utilities/vectors.h"



LogLineData::LogLineData(const std::string& line, const LogsFormat& logs_format)
{
    bool add_pm{false};
    size_t start, stop{logs_format.initial.size()},
           sep_i{0};
    const size_t line_size{ line.size()-1ul },
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
        const size_t sep_size = sep.size();

        // get the field
        const std::string& fld = logs_format.fields.at( sep_i );
        if ( fld != "NONE" ) {
            // only parse the considered fields

            std::string fld_str{ line.substr(start, stop-start) };

            if ( sep_i < n_seps ) {
                // not the last separator, check for mistakes
                size_t aux_stop = stop;

                if ( sep == " " ) {
                    // check the fields with whitespace-separated values
                    const size_t n{ fld == "request_full"   ? 2ul
                                  : fld == "date_time_ncsa" ? 1ul
                                  : fld == "date_time_mcs"  ? 4ul
                                  : fld == "date_time_gmt"  ? 3ul
                                                            : 0ul };
                    if ( n > 0ul ) {
                        size_t c{ StringOps::count( fld_str, ' ' ) };
                        if ( c < n ) {
                            // loop until the correct number of whitespaces is reached
                            size_t aux_start = line[stop+1ul] == ' ' ? stop : stop+1ul;
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
                    }

                } else if ( sep.front() == '"' && fld == "user_agent" ) {
                    // atm the only support is for escaped quotes
                    if ( fld_str.back() == '\\' ) {
                        // the found separator is not actually the separator but is part of the user-agent string
                        // keep searching until the real separator is found
                        size_t aux_start = stop + sep_size;
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
                const int& fld_id{ this->field2id.at(fld) };
                if ( fld_id > 0 ) {
                    // no need to process, append directly if non-empty
                    if ( fld_id == 13 && fld_str == "-" ) {
                        continue;
                    }
                    this->data( fld_id ) = FieldData( std::move(fld_str) );

                } else {
                    // process the field

                    // process the date to get year, month, day, hour and minute
                    if ( StringOps::startsWith( fld, "date_time" ) ) {
                        auto dt = DateTimeOps::processDateTime( fld_str, fld.substr( 10 ) ); // cut away the "date_time_" part
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
                            if ( hour == "PM" ) {
                                add_pm |= true;
                            } else {
                                this->hour = FieldData( std::move(hour) );
                            }
                        }
                        if ( auto& minute{ dt.at(4) }; !minute.empty() ) {
                            // minute
                            this->minute = FieldData( std::move(minute) );
                        }
                        if ( auto& second{ dt.at(5) }; !second.empty() ) {
                            // second
                            this->second = FieldData( std::move(second) );
                        }


                    // process the request to get the protocol, method, resource and query
                    } else if ( fld == "request_full" ) {
                        // check whether the request string has the proper number of spaces
                        const size_t n_spaces{ StringOps::count( fld_str, ' ' ) };

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
                            const size_t aux_stop1{ fld_str.find( ' ' ) },
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
                    } else if ( fld == "request_uri_query" ) {
                        // search for the query
                        std::string uri, query;
                        const size_t aux_{ fld_str.find( '?' ) };
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


                    // process the time taken to convert to milliseconds
                    } else if ( fld.rfind("time_taken_",0ul) == 0ul ) {
                        float t{ std::stof( fld_str ) };
                        const std::string u{ fld.substr( 11ul ) };
                        if ( u == "us" ) {
                            // from microseconds
                            t /= 1000.0f;
                        } else if ( u == "s" || u == "s.ms" ) {
                            // from seconds
                            t *= 1000.0f;
                        }
                        this->time_taken = FieldData( std::to_string( static_cast<int>( t ) ) );


                    // something went wrong
                    } else {
                        // hmmm.. no...
                        throw LogParserException( "Unexpected LogField", fld );
                    }
                }
            }
        }

        // update the stop for the next start
        stop += sep_size;
        sep_i++;
        if ( stop > line_size ) {
            // this was the final separator
            break;
        }

    }

    if ( add_pm ) {
        try {
            // add +12 hours for PM
            this->hour = FieldData( std::to_string( 12 + std::stoi(*this->hour) ) );
        } catch (...) {
            // no hour data
        }
    }
}

void LogLineData::storeUriQuery(std::string&& str)
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

void LogLineData::storeMalformedRequestOneSpace(std::string&& str)
{
    const size_t pos{ str.find( ' ' ) };
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

void LogLineData::storeMalformedRequestMultiSpace(std::string&& str)
{
    const size_t pos1{ str.find( ' ' ) },
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

size_t LogLineData::size() const
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

FieldData& LogLineData::data(const int& id)
{
    switch (id) {
        case 1:
            return this->year;
        case 2:
            return this->month;
        case 3:
            return this->day;
        case 4:
            return this->hour;
        case 5:
            return this->minute;
        case 6:
            return this->second;
        case 10:
            return this->protocol;
        case 11:
            return this->method;
        case 12:
            return this->uri;
        case 13:
            return this->query;
        case 14:
            return this->response_code;
        case 15:
            return this->time_taken;
        case 16:
            return this->bytes_sent;
        case 17:
            return this->bytes_received;
        case 18:
            return this->referrer;
        case 20:
            return this->client;
        case 21:
            return this->user_agent;
        case 22:
            return this->cookie;
        default:
            throw LogParserException( "Unexpected LogField ID", std::to_string(id) );
    }
}
