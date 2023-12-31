#include <iostream>
#include <vector>
#include <iomanip>

using namespace std::literals;

const int MAX_PAGES = 1000;
const int MAX_USERS = 100000;

std::vector<unsigned> user_pages(MAX_USERS + 1);
std::vector<unsigned> reading_pages_cnt_user(MAX_PAGES + 1);

void ReadPageUser(unsigned user, unsigned number_page)
{
    if (user <= MAX_USERS && number_page <= MAX_PAGES)
    {
        for (unsigned p = user_pages[user] + 1; p <= number_page; ++p)
        {
            ++reading_pages_cnt_user[p];
        }

        user_pages[user] = number_page;
    }
    else
    {
        throw std::range_error("Input data is out of range");
    }
}

double PartReadersLess(unsigned user)
{
    if (user > MAX_USERS)
    {
        throw std::range_error("Input data is out of range");
    }
    if (user_pages[user])
    {
        if (user_pages[user] > MAX_PAGES)
        {
            throw std::range_error("Data in user_pages is out of range");
        }

        if (reading_pages_cnt_user[user_pages[user]] - 1)
        {
            // Общее число пользователей прочиташих хотя бы одну страницу , помимо его самого 
            double total_user = (reading_pages_cnt_user[1] - 1.0);
            return (total_user - (reading_pages_cnt_user[user_pages[user]] - 1)) / total_user;
        }
        else
        {
            // Дочитал только пользователь
            return 1.0;
        }
    }
    else
    {
        // Пользователя нет или он не ничего не прочитал
        return 0.0;
    }
}


void LoadData(std::istream& in, std::ostream& out)
{
    std::cout << std::setprecision(6);
    int query_count;
    in >> query_count;
    for (int query_id = 0; query_id < query_count; ++query_id)
    {
        std::string query_type;
        in >> query_type;
        unsigned user_id;
        in >> user_id;
        if (query_type == "CHEER"s)
        {
            out << PartReadersLess(user_id) << std::endl;
        }
        else if (query_type == "READ"s)
        {
            unsigned read_page;
            in >> read_page;
            ReadPageUser(user_id, read_page);
        }
        else
        {
            throw std::logic_error("Unknown command recived");
        }
    }
}


int main()
{
    LoadData(std::cin, std::cout);
    return 0;
}