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
    void checkLowBound(Tp param, int low_bound, const std::string& msg)
    {
        if (param < low_bound)
            throw domain_error(msg);
    }


    template <typename Tp>
    void checkHighBound(Tp param, int low_bound, const std::string& msg)
    {
        if (param < low_bound)
            throw domain_error(msg);
    }

    DateRestrictions min_rest_;
    DateRestrictions max_rest_;

public:
    CheckDate(DateRestrictions min_rest, DateRestrictions max_rest) : min_rest_(min_rest), max_rest_(max_rest)
    {

    }

    void CheckDateTimeValidity(const DateTime& dt)
    {
        checkLowBound(dt.year, min_rest_.year, "year is too small"s);
        checkHighBound(dt.year, max_rest_.year , "year is too big"s);

        checkLowBound(dt.month, min_rest_.month , "month is too small"s);
        checkHighBound(dt.month, max_rest_.month, "month is too big"s);

        const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
        const std::array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        checkLowBound(dt.day, min_rest_.day, "day is too small"s);
        checkHighBound(dt.day, std::min(max_rest_.day,month_lengths[dt.month - 1]), "day is too big"s);

        checkLowBound(dt.hour, min_rest_.hour, "hour is too small"s);
        checkHighBound(dt.hour, max_rest_.hour, "hour is too big"s);

        checkLowBound(dt.minute, min_rest_.minute, "minute is too small"s);
        checkHighBound(dt.minute, max_rest_.minute, "minute is too big"s);

        checkLowBound(dt.second, min_rest_.second, "second is too small");
        checkHighBound(dt.second, max_rest_.second, "second is too big"s);
    }
};
