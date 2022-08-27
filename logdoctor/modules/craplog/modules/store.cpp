
#include "store.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include <QVariant>


StoreOps::StoreOps()
{

}



bool StoreOps::storeData( QSqlDatabase& db, Craplog& craplog, const std::vector<std::unordered_map<int, std::string>>& data )
{
    bool successful = true;

    int wsID = craplog.getCurrentWSID();
    QString db_name;
    {
        std::string db_path = craplog.getStatsDatabasePath();
        db_name.append( QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) ) );
    }

    // get blacklist/warnlist items
    bool check_bl_cli,
         check_wl_cli, check_wl_ua, check_wl_met, check_wl_req;
    check_bl_cli = craplog.isBlacklistUsed( wsID, 20 );
    check_wl_met = craplog.isWarnlistUsed( wsID, 11 );
    check_wl_req = craplog.isWarnlistUsed( wsID, 12 );
    check_wl_cli = craplog.isWarnlistUsed( wsID, 20 );
    check_wl_ua  = craplog.isWarnlistUsed( wsID, 21 );
    std::vector<std::string> bl_cli_list, bl_err_list, wl_cli_list, wl_ua_list, wl_met_list, wl_req_list, wl_err_list;
    if ( check_bl_cli ) {
        bl_cli_list = craplog.getBlacklist( wsID, 20 );
    }
    if ( check_wl_met ) {
        wl_met_list = craplog.getWarnlist( wsID, 11 );
    }
    if ( check_wl_req ) {
        wl_req_list = craplog.getWarnlist( wsID, 12 );
    }
    if ( check_wl_cli ) {
        wl_cli_list = craplog.getWarnlist( wsID, 20 );
    }
    if ( check_wl_ua ) {
        wl_ua_list = craplog.getWarnlist(  wsID, 21 );
    }

    // prepare the database related studd
    QString table;
    switch ( wsID ) {
        case 11:
            table = "apache";
            break;
        case 12:
            table = "nginx";
            break;
        case 13:
            table = "iis";
            break;
        default:
            // wrong WebServerID, but should be unreachable because of the previous operations
            throw WebServerException( "Unexpected WebServerID: " + std::to_string(wsID) );
    }


    int perf_size;
    bool skip = false,
         warning = false;
    QSqlQuery query = QSqlQuery( db );
    // parse every row of data
    for ( const std::unordered_map<int, std::string>& row : data ) {
        // break if failed
        if ( ! successful ) { break; }

        // check blacklisted clients
        if ( check_bl_cli ) {
            if ( row.find( 20 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target = row.at( 20 );
                for ( const auto& item : bl_cli_list ) {
                    if ( StringOps::startsWith( target, item ) ) {
                        // match found! skip this line
                        skip = true;
                        break;
                    }
                }
            }
        }
        if ( skip ) {
            // append every field to ignored size
            int blacklisted_size = 0;
            for ( const auto& [ id, str ] : row ) {
                blacklisted_size += str.size();
            }
            craplog.sumBlacklistededSize( blacklisted_size );
            skip = false;
            continue;
        }

        // check warnlisted clients
        if ( check_wl_cli ) {
            if ( row.find( 20 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target = row.at( 20 );
                for ( const auto& item : wl_cli_list ) {
                    if ( StringOps::startsWith( target, item ) ) {
                        // match found! put a warning on this line
                        warning = true;
                        craplog.sumWarningsSize( item.size() );
                        break;
                    }
                }
            }
        }
        // check warnlisted user-agents
        if ( check_wl_ua && !warning ) {
            if ( row.find( 21 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target = row.at( 21 );
                for ( const auto& item : wl_ua_list ) {
                    if ( StringOps::startsWith( target, item ) ) {
                        // match found! skip this line
                        warning = true;
                        craplog.sumWarningsSize( item.size() );
                        break;
                    }
                }
            }
        }
        // check warnlisted methods
        if ( check_wl_met && !warning ) {
            if ( row.find( 11 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target = row.at( 11 );
                for ( const auto& item : wl_met_list ) {
                    if ( item == target ) {
                        // match found! skip this line
                        warning = true;
                        craplog.sumWarningsSize( item.size() );
                        break;
                    }
                }
            }
        }
        // check warnlisted requests urls
        if ( check_wl_req && !warning ) {
            if ( row.find( 12 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target = row.at( 12 );
                for ( const auto& item : wl_req_list ) {
                    if ( StringOps::startsWith( target, item ) ) {
                        // match found! skip this line
                        warning = true;
                        craplog.sumWarningsSize( item.size() );
                        break;
                    }
                }
            }
        }


        // initialize the SQL statement
        QString query_stmt;
        query_stmt = "INSERT INTO \""+table+"\" (\"warning\", \"year\", \"month\", \"day\", \"hour\", \"minute\", \"second\", \"protocol\", \"method\", \"uri\", \"query\", \"response\", \"time_taken\", \"bytes_sent\", \"bytes_received\", \"referrer\", \"client\", \"user_agent\", \"cookie\") "\
                   "VALUES (";


        // complete and execute the statement, binding NULL if not found
        perf_size = 0;

        // warning
        if ( row.find( 99 ) == row.end() ) {
            // no value found in the collection, bind NULL
            query_stmt += "0";
        } else {
            // value found, bind it
            perf_size ++;
            if ( ! warning ) {
                query_stmt += "0";
            } else {
                query_stmt += "1";
                warning = false;
            }
        }

        // date and time
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

        // request
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

        // client data and referrer
        for ( int i : std::vector<int>({18,20,21,22}) ) {
            query_stmt += ", ";
            if ( row.find( i ) == row.end() ) {
                // no value found in the collection, bind NULL
                query_stmt += "NULL";
            } else {
                // value found, bind it
                if ( i == 21 && wsID == 13 ) {
                    // iis logs the user-agent using '+' instead of ' ' (spaces)
                    QString str = QString::fromStdString( row.at( i ) ).replace("+"," ");
                    perf_size += str.size();
                    query_stmt += QString("'%1'").arg( str.replace("'","''") );
                } else {
                    perf_size += row.at( i ).size();
                    query_stmt += QString("'%1'").arg( QString::fromStdString( row.at( i ) ).replace("'","''") );
                }
            }
        }


        // encode the statement
        query_stmt += ");";
        if ( ! query.prepare( query_stmt ) ) {
            // error opening database
            successful = false;
            QString query_msg="", err_msg="";
            if ( craplog.getDialogsLevel() > 0 ) {
                query_msg = "query.prepare()";
                if ( craplog.getDialogsLevel() == 2 ) {
                    err_msg = query.lastError().text();
                }
            }
            DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query_msg, err_msg );
        }

        // sum stored data size for the perfs
        //craplog.sumPerfSize( perf_size );

        // finalize this statement
        if ( ! query.exec() ) {
            // error finalizing step
            successful = false;
            QString query_msg="", err_msg="";
            if ( craplog.getDialogsLevel() > 0 ) {
                query_msg = "query.exec()";
                if ( craplog.getDialogsLevel() == 2 ) {
                    err_msg = query.lastError().text();
                }
            }
            DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query_msg, err_msg );
            break;
        }

        // reset the statement to prepare for the next one
        query.finish();
    }

    return successful;
}
