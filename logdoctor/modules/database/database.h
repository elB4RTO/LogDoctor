#ifndef LOGDOCTOR__DATABASE_H
#define LOGDOCTOR__DATABASE_H


#include "modules/exceptions.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>


class QueryWrapper;


//! Represents a database
enum class DatabaseType
{
    Data,
    Hashes
};



//! DatabaseWrapper
/*!
    A wrapper around a QSqlDatabase
*/
class DatabaseWrapper final
{
    QSqlDatabase db;
    QString db_name;
    bool ongoing_transaction;
public:
    explicit DatabaseWrapper( const QString& database, const QString& name, const bool readonly );
    ~DatabaseWrapper();
    Q_DISABLE_COPY_MOVE(DatabaseWrapper)

    //! Returns a pointer to the underlying QSqlDatabase instance
    inline QSqlDatabase* operator ->() noexcept
    { return &db; }

    //! Returns a reference to the underlying QSqlDatabase instance
    inline QSqlDatabase& operator *() noexcept
    { return db; }

    //! Returns the name of the database
    inline const QString& name() const noexcept
    { return db_name; }

    //! Opens the database file at the given path
    /*!
        Throws if the file cannot be opened or if opening fails
        \throw VoidException
    */
    void open( const std::string& path, const bool explain_err );

    //! Opens the database file at the given path
    /*!
        Used when creating a new database file.
        Throws if opening fails.
        \throw VoidException
    */
    void openNew( const std::string& path );

    //! Starts an ACID transaction on the database
    /*!
        Throws in case of failure
        \throw VoidException
    */
    void startTransaction( const bool explain_msg, const bool explain_err );

    //! Commits an ongoing transaction
    /*!
        Throws in case of failure
        \throw VoidException
    */
    void commitTransaction( const bool explain_msg, const bool explain_err );

    //! Rolls back an ongoing transaction
    /*!
        Throws in case of failure
        \throw VoidException
    */
    void rollbackTransaction( const bool explain_msg, const bool explain_err );

    //! Returns an instance of a QueryWrapper associated with the database
    QueryWrapper getQuery() noexcept;
};



//! QueryWrapper
/*!
    A wrapper around a QSqlQuery, associated with a database
*/
class QueryWrapper final
{
    QSqlQuery query;
    QString stmt;
    QString db_name;
public:
    explicit QueryWrapper( DatabaseWrapper& db )
        : query{ *db }, db_name{ db.name() } {}
    ~QueryWrapper() = default;
    Q_DISABLE_COPY_MOVE(QueryWrapper)

    //! Returns a pointer to the underlying QSqlQuery instance
    inline QSqlQuery* operator ->() noexcept
    { return &query; }

    //! Appends the given text to the internal statement
    inline void operator <<( const char* text ) noexcept
    { stmt.append(text); }

    //! Appends the given text to the internal statement
    inline void operator <<( const QString& text ) noexcept
    { stmt.append(text); }

    //! Appends the given text to the internal statement
    inline void operator <<( const QStringView text ) noexcept
    { stmt.append(text); }

    //! Executes the query using the internal statement
    /*!
        Throws in case of failure
        \throw VoidException
    */
    inline void operator ()()
    { operator()(stmt); }

    //! Executes the query using the given statement
    /*!
        Throws in case of failure
        \throw VoidException
    */
    void operator ()( const QString& text );

    //! Returns the query value at the given index of the current record
    inline QVariant operator []( const int idx ) const noexcept
    { return query.value(idx); }

    //! Returns the number of records in the query result
    /*!
        Throws if the query cannot be restored at the initial position
        \throw DatabaseException
    */
    size_t size();

    //! Returns the name of the associated database
    const QString& dbName() const noexcept
    { return db_name; }
};



struct DatabaseHandler
{
    DatabaseHandler();
    ~DatabaseHandler();
    Q_DISABLE_COPY_MOVE(DatabaseHandler)

    //! Checks the presence of the QSQLITE driver
    inline bool checkDriver() const
    { return QSqlDatabase::drivers().contains("QSQLITE"); }

    //! Returns an instance of the requested database
    /*!
        \param readonly Whether the database whould be initialized in readonly mode
        \throw DoNotCatchException
    */
    static DatabaseWrapper get( const DatabaseType db_type, const bool readonly=false );
};


#define DB_READONLY true


#endif // LOGDOCTOR__DATABASE_H
