
#include "store.h"

#include <QVariant>


StoreOps::StoreOps()
{

}



bool StoreOps::storeData( QSqlDatabase& db, Craplog& craplog, const std::vector<std::unordered_map<int, std::string>>& data, const int log_type )
{
    bool successful = true;

    int wsID = craplog.getCurrentWSID();
    QString db_name;
    {
        std::string db_path = craplog.getStatsDatabasePath();
        db_name.append( QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) ) );
    }

    // get whitelist/blacklist items
    bool check_bl_cli, check_bl_err=false,
         check_wl_cli, check_wl_ua=false, check_wl_met=false, check_wl_req=false, check_wl_err=false;
    check_bl_cli = craplog.isBlacklistUsed( wsID, log_type, 20 );
    check_wl_cli = craplog.isWarnlistUsed( wsID, log_type, 20 );
    if ( log_type == 1 ) {
        check_wl_ua  = craplog.isWarnlistUsed( wsID, log_type, 21 );
        check_wl_met = craplog.isWarnlistUsed( wsID, log_type, 11 );
        check_wl_req = craplog.isWarnlistUsed( wsID, log_type, 12 );
    } else {
        check_bl_err = craplog.isBlacklistUsed( wsID, log_type, 31 );
        check_wl_err = craplog.isWarnlistUsed(  wsID, log_type, 31 );
    }
    std::vector<std::string> bl_cli_list, bl_err_list, wl_cli_list, wl_ua_list, wl_met_list, wl_req_list, wl_err_list;
    if ( check_bl_cli ) {
        bl_cli_list = craplog.getBlacklist( wsID, log_type, 20 );
    }
    if ( check_wl_cli ) {
        wl_cli_list = craplog.getWarnlist( wsID, log_type, 20 );
    }
    if ( log_type == 1 ) {
        if ( check_wl_ua ) {
            wl_ua_list = craplog.getWarnlist( wsID, log_type, 21 );
        }
        if ( check_wl_met ) {
            wl_met_list = craplog.getWarnlist( wsID, log_type, 11 );
        }
        if ( check_wl_req ) {
            wl_req_list = craplog.getWarnlist( wsID, log_type, 12 );
        }
    } else {
        if ( check_bl_err ) {
            bl_err_list = craplog.getBlacklist( wsID, log_type, 31 );
        }
        if ( check_wl_err ) {
            wl_err_list = craplog.getWarnlist( wsID, log_type, 31 );
        }
    }

    // prepare the database related studd
    QString table = "";
    switch ( wsID ) {
        case 11:
            table += "apache_";
            break;
        case 12:
            table += "nginx_";
            break;
        case 13:
            table += "iis_";
            break;
        default:
            // wrong WebServerID, but should be unreachable because of the previous operations
            throw( "Unexpected WebServerID: " + std::to_string(wsID) );
    }
    switch ( log_type ) {
        case 1:
            table += "access";
            break;
        case 2:
            table += "error";
            break;
        default:
            // wrong LogType, but should be unreachable because of initial checks
            throw( "Unexpected LogType: " + std::to_string(log_type) );
    }


    int perf_size;
    bool skip = false,
         warning = false;
    QSqlQuery query = QSqlQuery( db );
    // parse every row of data
    for ( const std::unordered_map<int, std::string>& row : data ) {
        // break if failed
        if ( successful == false ) { break; }

        // check blacklisted clients
        if ( check_bl_cli == true ) {
            if ( row.find( 20 ) != row.end() ) {
                // this row do contains this row item, check if they match
                std::string target = row.at( 20 );
                for ( const auto& item : bl_cli_list ) {
                    if ( item == target ) {
                        // match found! skip this line
                        skip = true;
                        break;
                    }
                }
            }
        }
        // check blacklisted errors
        if ( check_bl_err == true && skip == false ) {
            if ( row.find( 31 ) != row.end() ) {
                // this row do contains this row item, check if they match
                std::string target = row.at( 31 );
                for ( const auto& item : bl_err_list ) {
                    if ( item == target ) {
                        // match found! skip this line
                        skip = true;
                        break;
                    }
                }
            }
        }
        if ( skip == true ) {
            // append every field to ignored size
            int ignored_size = 0;
            for ( const auto& [ id, str ] : row ) {
                ignored_size += str.size();
            }
            craplog.sumIgnoredSize( ignored_size, log_type );
            skip=false;
            continue;
        }

        // check warnlisted clients
        if ( check_wl_cli == true ) {
            if ( row.find( 20 ) != row.end() ) {
                // this row do contains this row item, check if they match
                std::string target = row.at( 20 );
                for ( const auto& item : wl_cli_list ) {
                    if ( item == target ) {
                        // match found! put a warning on this line
                        warning = true;
                        break;
                    }
                }
            }
        }
        // check warnlisted user-agents
        if ( check_wl_ua == true && warning == false ) {
            if ( row.find( 21 ) != row.end() ) {
                // this row do contains this row item, check if they match
                std::string target = row.at( 21 );
                for ( const auto& item : wl_ua_list ) {
                    if ( item == target ) {
                        // match found! skip this line
                        warning = true;
                        break;
                    }
                }
            }
        }
        // check warnlisted methods
        if ( check_wl_met == true && warning == false ) {
            if ( row.find( 11 ) != row.end() ) {
                // this row do contains this row item, check if they match
                std::string target = row.at( 11 );
                for ( const auto& item : wl_met_list ) {
                    if ( item == target ) {
                        // match found! skip this line
                        warning = true;
                        break;
                    }
                }
            }
        }
        // check warnlisted requests urls
        if ( check_wl_req == true && warning == false ) {
            if ( row.find( 12 ) != row.end() ) {
                // this row do contains this row item, check if they match
                std::string target = row.at( 12 );
                for ( const auto& item : wl_req_list ) {
                    if ( item == target ) {
                        // match found! skip this line
                        warning = true;
                        break;
                    }
                }
            }
        }
        // check warnlisted error levels
        if ( check_wl_err == true && warning == false ) {
            if ( row.find( 31 ) != row.end() ) {
                // this row do contains this row item, check if they match
                std::string target = row.at( 31 );
                for ( const auto& item : wl_err_list ) {
                    if ( item == target ) {
                        // match found! skip this line
                        warning = true;
                        break;
                    }
                }
            }
        }

        // check request and query for potential threats
        // !!! IMPLEMENT !!!


        // initialize the SQL statement
        QString query_stmt;
        if ( log_type == LogOps::LogType::Access ) {
            query_stmt = "INSERT INTO "+table+" (warning, year, month, day, hour, minute, second, protocol, method, request, query, response, time_taken, bytes_sent, bytes_received, client, user_agent, cookie, referrer) "\
                       "VALUES (";
            /*query_stmt = "INSERT INTO "+table+" (warning, year, month, day, hour, minute, second, protocol, method, request, query, response, time_taken, bytes_sent, bytes_received, client, user_agent, cookie, referrer) "\
                       "VALUES (?99, ?1, ?2, ?3, ?4, ?5, ?6, ?10, ?11, ?12, ?13, ?14, ?15, ?16, ?17, ?18, ?20, ?21, ?22);";*/

        } else {
            query_stmt = "INSERT INTO "+table+" (warning, year, month, day, hour, minute, second, level, message, source_file, client, port) "\
                       "VALUES (";
            /*query_stmt = "INSERT INTO "+table+" (warning, year, month, day, hour, minute, second, level, message, source_file, client, port) "\
                       "VALUES (?99, ?1, ?2, ?3, ?4, ?5, ?6, ?31, ?32, ?33, ?20, ?30);";*/
        }


        // complete and execute the statement
        perf_size = 0;
        // set values to NULL
        if ( row.find( 99 ) == row.end() ) {
            // no value found in the collection, bind NULL
            query_stmt += "0";
        } else {
            // value found, bind it
            perf_size += row.at( 99 ).size();
            query_stmt += QString::fromStdString( row.at( 99 ) ).replace("'","''");
        }

        for ( int i=1; i<7; i++ ) {
            query_stmt += ", ";
            if ( row.find( i ) == row.end() ) {
                // no value found in the collection, bind NULL
                query_stmt += "NULL";
            } else {
                // value found, bind it
                perf_size += row.at( i ).size();
                query_stmt += QString::fromStdString( row.at( i ) ).replace("'","''");
            }
        }

        if ( log_type == LogOps::LogType::Access ) {
            // access logs
            for ( int i=10; i<14; i++ ) {
                query_stmt += ", ";
                if ( row.find( i ) == row.end() ) {
                    // no value found in the collection, bind NULL
                    query_stmt += "NULL";
                } else {
                    // value found, bind it
                    perf_size += row.at( i ).size();
                    query_stmt += QString("'%1'").arg( QString::fromStdString( row.at( i ) ).replace("'","''") );
                }
            }

            for ( int i=14; i<18; i++ ) {
                query_stmt += ", ";
                if ( row.find( i ) == row.end() ) {
                    // no value found in the collection, bind NULL
                    query_stmt += "NULL";
                } else {
                    // value found, bind it
                    perf_size += row.at( i ).size();
                    query_stmt += QString::fromStdString( row.at( i ) ).replace("'","''");
                }
            }

            for ( int i : std::vector<int>({18,20,21,22}) ) {
                query_stmt += ", ";
                if ( row.find( i ) == row.end() ) {
                    // no value found in the collection, bind NULL
                    query_stmt += "NULL";
                } else {
                    // value found, bind it
                    perf_size += row.at( i ).size();
                    query_stmt += QString("'%1'").arg( QString::fromStdString( row.at( i ) ).replace("'","''") );
                }
            }

        } else {
            // error logs
            for ( int i : std::vector<int>({31,32,33,20}) ) {
                query_stmt += ", ";
                if ( row.find( i ) == row.end() ) {
                    // no value found in the collection, bind NULL
                    query_stmt += "NULL";
                } else {
                    // value found, bind it
                    perf_size += row.at( i ).size();
                    query_stmt += QString("'%1'").arg( QString::fromStdString( row.at( i ) ).replace("'","''") );
                }
            }

            query_stmt += ", ";
            if ( row.find( 30 ) == row.end() ) {
                // no value found in the collection, bind NULL
                query_stmt += "NULL";
            } else {
                // value found, bind it
                perf_size += row.at( 30 ).size();
                query_stmt += QString::fromStdString( row.at( 30 ) ).replace("'","''");
            }
        }


        // encode the statement
        query_stmt += ");";
        if ( query.prepare( query_stmt ) == false ) {
            // error opening database
            successful = false;
            QString query_msg="", err_msg="";
            if ( craplog.getDialogLevel() > 0 ) {
                query_msg = "query.prepare()";
                if ( craplog.getDialogLevel() == 2 ) {
                    err_msg = query.lastError().text();
                }
            }
            DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query_msg, err_msg );
        }

        // sum stored data size for the perfs
        craplog.sumPerfSize( perf_size );

        // finalize this statement
        if ( query.exec() == false ) {
            // error finalizing step
            successful = false;
            QString query_msg="", err_msg="";
            if ( craplog.getDialogLevel() > 0 ) {
                query_msg = "query.exec()";
                if ( craplog.getDialogLevel() == 2 ) {
                    err_msg = query.lastError().text();
                }
            }
            DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query_msg, err_msg );
            break;
        }

        // reset the statement to prepare for the next
        query.finish();
    }


    return successful;
}
