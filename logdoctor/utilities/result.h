#ifndef RESULT_H
#define RESULT_H

#include <QString>

#include <unordered_map>
#include <vector>
#include <tuple>


//! Result
/*!
    Holds the result for an operation and the relative data
*/
template <typename T>
class Result
{
public:
    explicit Result();
    explicit Result( const bool ok, const T& data );

    explicit operator bool() const;

    /*
    //! Checks if the operation was successful
    const bool isOk();

    //! Checks if the operation has failed
    const bool isErr();
    */

    //! Returns the data
    const T& getData() const;

private:

    bool result;

    T data;
};


// data types
typedef std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>> stats_dates_t;
typedef std::vector<std::vector<std::vector<std::vector<QString>>>> stats_warn_items_t;
typedef std::vector<std::tuple<long long, std::vector<QString>>> stats_speed_items_t;
typedef std::unordered_map<int, std::unordered_map<int, int>> stats_day_items_t;
typedef std::vector<std::tuple<long long, int>> stats_relat_items_t;
typedef std::vector<std::tuple<QString, int>> stats_count_items_t;

#endif // RESULT_H
