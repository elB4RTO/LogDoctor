
#include "store.h"

#include <thread>


StoreOps::StoreOps()
{

}



bool StoreOps::storeData( sqlite3* db, Craplog& craplog, const std::vector<std::unordered_map<int, std::string>>& data, const int log_type )
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
    std::string table = "";
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

    // initialize the SQL statement
    int rc,
        tries = 0;
    sqlite3_stmt *stmt;
    std::string stmt_str;
    if ( log_type == LogOps::LogType::Access ) {
        stmt_str = "INSERT INTO "+table+" (warning, year, month, day, hour, minute, second, protocol, method, request, query, response, time_taken, bytes_sent, bytes_received, client, user_agent, cookie, referrer) "\
                   "VALUES (?99, ?1, ?2, ?3, ?4, ?5, ?6, ?10, ?11, ?12, ?13, ?14, ?15, ?16, ?17, ?18, ?20, ?21, ?22);";
    } else {
        stmt_str = "INSERT INTO "+table+" (warning, year, month, day, hour, minute, second, level, message, source_file, client, port) "\
                   "VALUES (?99, ?1, ?2, ?3, ?4, ?5, ?6, ?31, ?32, ?33, ?20, ?30);";
    }
    // encode the statement
    rc = sqlite3_prepare_v2( db, stmt_str.c_str(), -1, &stmt, NULL ); // set NULL as default value for unused columns
    if ( rc != SQLITE_OK ) {
        // error opening database
        successful = false;
        QString stmt_msg="", err_msg="";
        if ( craplog.getDialogLevel() > 0 ) {
            stmt_msg = "sqlite3_prepare_v2()";
            if ( craplog.getDialogLevel() == 2 ) {
                err_msg.append( sqlite3_errmsg(db) );
            }
        }
        DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
    }

    if ( successful == true ) {
        // parse every row of data
        int perf_size;
        bool skip = false,
             warning = false;
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


            // complete and execute the statement
            perf_size = 0;
            for ( const auto& [ id, str ] : row ) {
                perf_size += str.size();
                if ( id == 99 ) {
                    // bind BOOLEAN value
                    if ( warning == true ) {
                        rc = sqlite3_bind_int( stmt, id, 1 );
                    } else {
                        rc = sqlite3_bind_int( stmt, id, 0 );
                    }

                } else if ( (id > 0 && id < 7) || (id > 13 && id < 18) ) {
                    // bind as INTEGER
                    rc = sqlite3_bind_int( stmt, id, std::stoi( str ) );

                } else {
                    // bind everything else as TEXT
                    const char* txt = str.c_str();
                    rc = sqlite3_bind_text( stmt, id, txt, sizeof(txt), NULL );
                }

                if ( rc != SQLITE_OK ) {
                    // error binding value
                    successful = false;
                    QString stmt_msg="", err_msg="";
                    if ( craplog.getDialogLevel() > 0 ) {
                        stmt_msg = "sqlite3_bind_*()";
                        if ( craplog.getDialogLevel() == 2 ) {
                            err_msg.append( sqlite3_errmsg(db) );
                        }
                    }
                    DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
                    break;
                }

            }

            if ( successful == true ) {
                // sum stored data size for the perfs
                craplog.sumPerfSize( perf_size );

                // finalize this statement
                tries = 0;
                while (true) {
                    tries ++;
                    if ( tries > 3 ) {
                        break;
                    }
                    rc = sqlite3_step(stmt);
                    if ( rc == SQLITE_BUSY ) {
                        // database busy, wait 1 sec and retry
                        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
                    } else {
                        break;
                    }
                }
                if ( rc != SQLITE_DONE ) {
                    // error finalizing step
                    successful = false;
                    QString stmt_msg="", err_msg="";
                    if ( craplog.getDialogLevel() > 0 ) {
                        stmt_msg = "sqlite3_step()";
                        if ( craplog.getDialogLevel() == 2 ) {
                            err_msg.append( sqlite3_errmsg(db) );
                        }
                    }
                    DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
                    break;
                }

                // reset the statement to prepare for the next
                rc = sqlite3_reset(stmt);
                if ( rc != SQLITE_DONE && rc != SQLITE_OK ) {
                    // error resetting statement
                    successful = false;
                    QString stmt_msg="", err_msg="";
                    if ( craplog.getDialogLevel() > 0 ) {
                        stmt_msg = "sqlite3_reset()";
                        if ( craplog.getDialogLevel() == 2 ) {
                            err_msg.append( sqlite3_errmsg(db) );
                        }
                    }
                    DialogSec::errDatabaseFailedExecuting( nullptr, "statistics.db", stmt_msg, err_msg );
                    break;
                }
            }
        }
    }

    // destroy the statement to avoid leaks, says SQLite doc
    sqlite3_finalize(stmt);

    return successful;
}
