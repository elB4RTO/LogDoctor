#ifndef RESULT_H
#define RESULT_H

#include "modules/exceptions.h"


//! Result
/*!
    Holds the result for an operation and the relative data
*/
template <typename T>
class Result
{
public:
    explicit Result()
    : result{ false }, data{}
    {}

    explicit Result( const bool ok, const T& data )
    : result{ ok }, data{ data }
    {}

    explicit operator bool() const
    { return this->result; }

    /*
    //! Checks if the operation was successful
    const bool isOk()
    { return this->result == true; }

    //! Checks if the operation has failed
    const bool isErr()
    { return this->result == false; }
    */

    //! Returns the data
    const T& getData() const
    {
        if ( this->result ) {
            return this->data;
        } else {
            // unexpected operation
            throw GenericException( "Result is Err, no data to be returned", true );
        }
    }

private:

    bool result;

    T data;
};


// data types
typedef std::map<int, std::map<int, std::map<int, std::vector<int>>>> stats_dates_t;
typedef std::vector<std::vector<std::vector<std::vector<QString>>>> stats_warn_items_t;
typedef std::vector<std::tuple<long long, std::vector<QString>>> stats_speed_items_t;
typedef std::unordered_map<int, std::unordered_map<int, int>> stats_day_items_t;
typedef std::vector<std::tuple<long long, int>> stats_relat_items_t;
typedef std::map<unsigned, QString> stats_count_items_t;

#endif // RESULT_H
