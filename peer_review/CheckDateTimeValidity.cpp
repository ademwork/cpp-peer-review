#include <array>
#include <string>
#include <algorithm>

struct DateRestrictions
{
    unsigned year;
    unsigned month;
    unsigned day;
    unsigned hour;
    unsigned minute;
    unsigned second;
};


class CheckDate
{
    template <typename Tp>
    void CheckLowBound(Tp param, int low_bound, const std::string& msg)
    {
        if (static_cast<int>(param) < low_bound)
            throw domain_error(msg);
    }


    template <typename Tp>
    void CheckHighBound(Tp param, int high_bound, const std::string& msg)
    {
        if (static_cast<int>(param) < high_bound)
            throw domain_error(msg);
    }

    DateRestrictions min_rest_;
    DateRestrictions max_rest_;

public:
    CheckDate(const DateRestrictions &min_rest, const DateRestrictions &max_rest) : min_rest_(min_rest), max_rest_(max_rest)
    {

    }

    void CheckDateTimeValidity(const DateTime& dt)
    {
        CheckLowBound(dt.year, min_rest_.year, "year is too small"s);
        CheckHighBound(dt.year, max_rest_.year , "year is too big"s);

        CheckLowBound(dt.month, min_rest_.month , "month is too small"s);
        CheckHighBound(dt.month, max_rest_.month, "month is too big"s);

        const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
        const std::array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        CheckLowBound(dt.day, min_rest_.day, "day is too small"s);
        CheckHighBound(dt.day, std::min(max_rest_.day,month_lengths[dt.month - 1]), "day is too big"s);

        CheckLowBound(dt.hour, min_rest_.hour, "hour is too small"s);
        CheckHighBound(dt.hour, max_rest_.hour, "hour is too big"s);

        CheckLowBound(dt.minute, min_rest_.minute, "minute is too small"s);
        CheckHighBound(dt.minute, max_rest_.minute, "minute is too big"s);

        CheckLowBound(dt.second, min_rest_.second, "second is too small");
        CheckHighBound(dt.second, max_rest_.second, "second is too big"s);
    }
};


void CheckDateTimeValidity(const DateTime& dt)
{
    DateRestrictions min_res =
    {
        .year = 1,
        .month = 1,
        .day = 1,
        .hour = 0,
        .minute = 0,
        .second = 0
    };

    DateRestrictions max_res =
    {
        .year = 9999,
        .month = 12,
        .day = 31,
        .hour = 23,
        .minute = 59,
        .second = 59
    };

    CheckDate(min_res, max_res).CheckDateTimeValidity(dt);

}