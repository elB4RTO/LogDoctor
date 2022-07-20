#ifndef CRAPVIEW_H
#define CRAPVIEW_H

#include <string>


class Crapview
{
public:
    Crapview();


private:

    bool daytime_period;

    int count_ws, count_type, count_field,
        daytime_ws, daytime_type, daytime_field,
        relational_ws, relational_field_1, relational_field_2,
        speed_ws;

    std::string daytime_filter,
                relational_field_1_filter, relational_field_2_filter,
                speed_uri_filter;
};

#endif // CRAPVIEW_H
