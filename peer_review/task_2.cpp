#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <cassert>

using namespace std;

class Domain
{
    std::string domain_;
public:
    template<typename Str>
    Domain(Str&& new_domen)
    {
        domain_ = "."s + forward<Str>(new_domen);
        std::reverse(domain_.begin(), domain_.end());
    }

    bool operator==(const Domain& other) const
    {
        if (other.domain_ == domain_)
            return true;
        else
            return false;
    }

    bool operator> (const Domain& other) const
    {
        if (other.domain_ > domain_)
            return true;
        else
            return false;
    }

    bool IsSubdomain(const Domain& check_domain) const
    {
        if (check_domain.domain_.size() < domain_.size())
        {
            return false;
        }
        else if (check_domain.domain_.size() == domain_.size())
        {
            if (check_domain.domain_ == domain_)
                return true;
            else
                return false;
        }
        else
        {
            auto check_beg_It = check_domain.domain_.begin();
            auto check_end_It = check_domain.domain_.end();
            auto forb_beg_It = domain_.begin();
            auto forb_end_It = domain_.end();

            while (check_beg_It != check_end_It && forb_beg_It != forb_end_It)
            {
                if (*check_beg_It++ != *forb_beg_It++)
                    return false;
            }

            return true;
        }
    }
};

class DomainChecker
{
    std::vector<Domain> reading_domain_;
public:
    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last)
    {
        auto size_vect = std::distance(first, last);
        reading_domain_.reserve(size_vect);
        while (first != last)
        {
            reading_domain_.push_back(move(*first++));
        }

        sort(reading_domain_.begin(), reading_domain_.end(), [](const auto& lhr, const auto& rhl)
            {
                return lhr > rhl;
            });
        auto erase_it = unique(reading_domain_.begin(), reading_domain_.end(), [](const auto& lhr, const auto& rhl)
            {
                if (lhr.IsSubdomain(rhl))
                    return true;
                else
                    return false;
            });
        reading_domain_.erase(erase_it, reading_domain_.end());
    }

    bool IsForbidden(const Domain& check_domain) const
    {
        if (reading_domain_.empty() == true)
        {
            return false;
        }

        auto search_it = upper_bound(reading_domain_.begin(), reading_domain_.end(), check_domain, [](const Domain& a, const Domain& b)
            {
                if (a > b)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            });

        if (search_it == reading_domain_.begin())
        {
            return false;
        }
        else
        {
            return (--search_it)->IsSubdomain(check_domain);
        }
    }
};


std::vector<Domain>ReadDomains(istream& in, size_t number)
{
    std::vector<Domain> reading_domain;

    for (size_t query_id = 0; query_id < number; ++query_id)
    {
        std::string query_type;
        while (query_type == ""s)
        {
            in >> query_type;
        }

        reading_domain.push_back(Domain(query_type));
    }


    return reading_domain;
}



template <typename Number>
Number ReadNumberOnLine(istream& input)
{
    string line;
    while (line == ""s)
    {
        getline(input, line);
    }

    Number num;
    std::istringstream(line) >> num;

    return num;
}


std::string test_input =
{
    "4\n gdz.ru maps.me m.gdz.ru com 7\n gdz.ru gdz.com m.maps.me alg.m.gdz.ru maps.com maps.ru gdz.ua"
};

std::string test_output =
{
    "Bad\nBad\nBad\nBad\nBad\nGood\nGood\n"
}; 



void UnitTest()
{
    std::stringstream s_in, s_out;
    s_in << test_input;

    const std::vector<Domain> forbidden_domains = ReadDomains(s_in, ReadNumberOnLine<size_t>(s_in));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(s_in, ReadNumberOnLine<size_t>(s_in));
    for (const Domain& domain : test_domains)
    {
        s_out << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
    assert(s_out.str() == test_output);
}



int main()
{
    UnitTest();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains)
    {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }

    return 0;
}
