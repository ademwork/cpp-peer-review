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
        return (other.domain_ == domain_);
    }

    bool operator < (const Domain& rhs) const
    {
        return (domain_ <  rhs.domain_);
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
            size_t check_index = 0 , forbed_index = 0;

            while (check_index < check_domain.domain_.size() && forbed_index < domain_.size())
            {
                if (check_domain.domain_[check_index++] != domain_[forbed_index++])
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
    DomainChecker(InputIt first, InputIt last) : reading_domain_(first, last)
    {
        sort(reading_domain_.begin(), reading_domain_.end());
        auto erase_it = unique(reading_domain_.begin(), reading_domain_.end(), [](const auto& lhr, const auto& rhl)
            {
                return lhr.IsSubdomain(rhl);
            });
        reading_domain_.erase(erase_it,reading_domain_.end());
    }

    bool IsForbidden(const Domain &check_domain) const
    {
        if (reading_domain_.empty() == true)
        {
            return false;
        }

        auto search_it = upper_bound(reading_domain_.begin(), reading_domain_.end(), check_domain);

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

void UnitTestDomen()
{
    Domain dom_test("com"s);

    assert(dom_test.IsSubdomain("abc.com"s) == true);
    assert(dom_test.IsSubdomain("com.ru"s) == false);
    assert(dom_test.IsSubdomain(""s) == false);
    assert(dom_test.IsSubdomain("com.com.com"s) == true);
}

void UnitTestChecker()
{
    const std::vector<Domain> forbidden_domains = {"com"s, "opq.ru"s};
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    assert(checker.IsForbidden("abc.com"s) == true);
    assert(checker.IsForbidden("abc.def.com"s) == true);
    assert(checker.IsForbidden("abc.ru"s) == false);
    assert(checker.IsForbidden("com.opq.ru"s) == true);
}


void UnitTest()
{
    std::stringstream s_in,s_out;
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
    UnitTestDomen();
    UnitTestChecker();
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
