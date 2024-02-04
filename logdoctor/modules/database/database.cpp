
#include "database.h"

#include "globals/db_names.h"

#include "modules/dialogs.h"

#include "utilities/checks.h"

#include <iostream>

#include <QSqlError>


//////////////////////////
//// DATABASE WRAPPER ////
//////////////////////////

DatabaseWrapper::DatabaseWrapper( const QString& database, const QString& name, const bool readonly )
    : db{ QSqlDatabase::database( database, false ) }
    , db_name{ name }
    , ongoing_transaction{ false }
{
    if ( readonly ) {
        this->db.setConnectOptions( QStringLiteral("QSQLITE_OPEN_READONLY=1") );
    }
}

DatabaseWrapper::~DatabaseWrapper()
{
    if ( this->ongoing_transaction ) {
        std::cerr << "LogDoctor: DatabaseWrapper: Instance destroyed whithin ongoing transaction" << std::endl;
        if ( ! this->db.rollback() ) {
            std::cerr << "LogDoctor: DatabaseWrapper: Failed to rollback ongoing transaction" << std::endl;
        }
    }
    if ( this->db.isOpen() ) {
        this->db.close();
    }
}

void DatabaseWrapper::open( const std::string& path, const bool explain_err )
{
    this->db.setDatabaseName( QString::fromStdString( path ));
    if ( ! CheckSec::checkDatabaseFile( path, this->db_name ) ) {
        throw LogDoctorException();
    } else if ( ! this->db.open() ) {
        DialogSec::errDatabaseFailedOpening( this->db_name, explain_err ? this->db.lastError().text() : QString{} );
        throw LogDoctorException();
    }
}

void DatabaseWrapper::openNew( const std::string& path )
{
    this->db.setDatabaseName( QString::fromStdString( path ));
    if ( ! this->db.open() ) {
        DialogSec::errDatabaseFailedOpening( this->db_name, db.lastError().text() );
        throw LogDoctorException();
    }
}

void DatabaseWrapper::startTransaction(const bool explain_msg, const bool explain_err )
{
    if ( ! this->db.transaction() ) {
        DialogSec::errDatabaseFailedExecuting(
            db_name,
            explain_msg ? QStringLiteral("db.transaction()") : QString(),
            explain_err ? this->db.lastError().text() : QString() );
        throw LogDoctorException();
    }
    this->ongoing_transaction |= true;
}

void DatabaseWrapper::commitTransaction( const bool explain_msg, const bool explain_err )
{
    if ( ! db.commit() ) {
        DialogSec::errDatabaseFailedExecuting(
            db_name,
            explain_msg ? QStringLiteral("db.commit()") : QString(),
            explain_err ? this->db.lastError().text() : QString() );
        throw LogDoctorException();
    }
    this->ongoing_transaction &= false;
}

void DatabaseWrapper::rollbackTransaction( const bool explain_msg, const bool explain_err )
{
    if ( ! db.rollback() ) {
        DialogSec::errDatabaseFailedExecuting(
            db_name,
            explain_msg ? QStringLiteral("db.rollback()") : QString(),
            explain_err ? this->db.lastError().text() : QString() );
        throw LogDoctorException();
    }
    this->ongoing_transaction &= false;
}

QueryWrapper DatabaseWrapper::getQuery() noexcept
{
    return QueryWrapper( *this );
}


///////////////////////
//// QUERY WRAPPER ////
///////////////////////

void QueryWrapper::operator()( const QString& text )
{
    if ( !query.exec( text ) ) {
        DialogSec::errDatabaseFailedExecuting( db_name, query.lastQuery(), query.lastError().text() );
        throw LogDoctorException();
    }
}

size_t QueryWrapper::size()
{
    if ( !query.last() ) {
        return 0ul;
    }
    const auto sz{ static_cast<size_t>( query.at()+1 ) };
    if ( !query.first() ) {
        throw DatabaseException( QStringLiteral("Failed to move back to first record") );
    }
    query.previous();
    return sz;
}


//////////////////////////
//// DATABASE HANDLER ////
//////////////////////////

DatabaseHandler::DatabaseHandler()
{
    QSqlDatabase::addDatabase( "QSQLITE", DatabasesConnections::data );
    QSqlDatabase::addDatabase( "QSQLITE", DatabasesConnections::hashes );
}

DatabaseHandler::~DatabaseHandler()
{
    QSqlDatabase::removeDatabase( DatabasesConnections::data );
    QSqlDatabase::removeDatabase( DatabasesConnections::hashes );
}

DatabaseWrapper DatabaseHandler::get( const DatabaseType db_type, const bool readonly )
{
    switch ( db_type ) {
        case DatabaseType::Data:
            return DatabaseWrapper( DatabasesConnections::data, QString(DatabasesNames::data), readonly );
        case DatabaseType::Hashes:
            return DatabaseWrapper( DatabasesConnections::hashes, QString(DatabasesNames::hashes), readonly );
        default:
            throw DoNotCatchException( "Unexpected DatabaseType" );
    }

}
