
#include "result.h"
#include "modules/exceptions.h"


template <typename T>
Result<T>::Result()
{
    this->result = false;
}

template <typename T>
Result<T>::Result( const bool ok, const T& data )
{
    this->result = ok;
    this->data   = std::move(data);
}

template <typename T>
Result<T>::operator bool() const
{
    return this->result;
}

/*
template <typename T>
const bool Result<T>::isOk()
{
    return this->result == true;
}

template <typename T>
const bool Result<T>::isErr()
{
    return this->result == false;
}
*/

template <typename T>
const T& Result<T>::getData() const
{
    if ( this->result ) {
        return this->data;
    } else {
        // unexpected operation
        throw GenericException( "Result is Err, no data to be returned", true );
    }
}


template class Result< stats_dates_t       >; // std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>
template class Result< stats_warn_items_t  >; // std::vector<std::vector<std::vector<std::vector<QString>>>>
template class Result< stats_speed_items_t >; // std::vector<std::tuple<long long, std::vector<QString>>>
template class Result< stats_day_items_t   >; // std::unordered_map<int, std::unordered_map<int, int>>
template class Result< stats_relat_items_t >; // std::vector<std::tuple<long long, int>>
template class Result< stats_count_items_t >; // std::vector<std::tuple<QString, int>>
