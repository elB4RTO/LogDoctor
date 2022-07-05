
#include "store.h"


StoreOps::StoreOps()
{

}



bool StoreOps::storeData( sqlite3* db, Craplog& craplog, const std::vector<std::unordered_map<int, std::string>>& data, const int log_type )
{
    bool successful = true;

    int wsID = craplog.getCurrentWSID();

    // get whitelist/blacklist items
    bool check_bl_cli, check_bl_err=false,
         check_wl_cli, check_wl_met=false, check_wl_req=false, check_wl_err=false;
    check_bl_cli = craplog.isBlacklistUsed( wsID, log_type, 20 );
    check_wl_cli = craplog.isBlacklistUsed( wsID, log_type, 20 );
    if ( log_type == 1 ) {
        check_wl_met = craplog.isBlacklistUsed( wsID, log_type, 11 );
        check_wl_req = craplog.isBlacklistUsed( wsID, log_type, 12 );
    } else {
        check_bl_err = craplog.isBlacklistUsed( wsID, log_type, 31 );
        check_wl_err = craplog.isBlacklistUsed( wsID, log_type, 31 );
    }
    std::vector<std::string> bl_cli_list, bl_err_list, wl_cli_list, wl_met_list, wl_req_list, wl_err_list;
    if ( check_bl_cli ) {
        bl_cli_list = craplog.getBlacklist( wsID, log_type, 20 );
    }
    if ( check_wl_cli ) {
        wl_cli_list = craplog.getWarnlist( wsID, log_type, 20 );
    }
    if ( log_type == 1 ) {
        if ( check_wl_met ) {
            check_wl_met = craplog.isBlacklistUsed( wsID, log_type, 11 );
        }
        if ( check_wl_req ) {
            check_wl_req = craplog.isBlacklistUsed( wsID, log_type, 12 );
        }
    } else {
        if ( check_bl_err ) {
            check_bl_err = craplog.isBlacklistUsed( wsID, log_type, 31 );
        }
        if ( check_wl_err ) {
            check_wl_err = craplog.isBlacklistUsed( wsID, log_type, 31 );
        }
    }

    // prepare the database related studd
    char *zErrMsg = 0;
    int rc;
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
            throw ("Wrong WebServerID: "[wsID]);
    }

    sqlite3_stmt *stmt;
    std::string stmt_str;
    if ( log_type == LogOps::LogType::Access ) {
        stmt_str = "INSERT INTO ? (warning, year, month, day, hour, minute, second, protocol, method, request, query, response, time_taken, bytes_sent, bytes_received, client, user_agent, cookie, referrer) "\
                   "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    } else {
        stmt_str = "INSERT INTO ? (warning, year, month, day, hour, minute, second, level, message, source_file, client, port) "\
                   "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    }
    sqlite3_prepare_v2( db, stmt_str.c_str(), -1, &stmt, nullptr);
    // parse every data row
    bool skip = false,
         warning = false;
    for ( const std::unordered_map<int, std::string>& row : data ) {

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
                std::string target = row.at( 20 );
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

        // complete and execute the statement

        compute_result(a[i], b[i], &c, &d);

        sqlite3_bind_int(stmt, 1, a[i]);
        sqlite3_bind_int(stmt, 2, b[i]);
        sqlite3_bind_int(stmt, 3, c]);
        sqlite3_bind_int(stmt, 4, d);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);

    }

    return successful;
}
