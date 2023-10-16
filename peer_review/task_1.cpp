#include <iostream>
#include <vector>
#include <iomanip>

using namespace std::literals;

std::vector<unsigned> user_pages;
std::vector<unsigned> reading_pages_cnt_user;

template <typename T>
inline void CheckSizeVect(std::vector<T>& check_vect, size_t check_index)
{
    if (check_vect.size() <= check_index)
        check_vect.resize(check_index * 2, 0);
}

void ReadPageUser(unsigned user, unsigned number_page)
{
    CheckSizeVect(user_pages, user);

    CheckSizeVect(reading_pages_cnt_user, number_page);

    for (unsigned p = user_pages[user] + 1; p <= number_page; ++p)
    {
        ++reading_pages_cnt_user[p];
    }

    user_pages[user] = number_page;
}

double PartReadersLess(unsigned user)
{
    CheckSizeVect(user_pages, user);

    if (user_pages[user])
    {
        CheckSizeVect(reading_pages_cnt_user, user_pages[user]);

        if (reading_pages_cnt_user[user_pages[user]] - 1)
        {
            // ќбщее число пользователей прочиташих хот€ бы одну страницу , помимо его самого 
            double total_user = (reading_pages_cnt_user[1] - 1.0);
            return (total_user - (reading_pages_cnt_user[user_pages[user]] - 1)) / total_user;
        }
        else
            // ƒочитал только пользователь
            return 1.0;
    }
    else
    {
        // ѕользовател€ нет или он не ничего не прочитал
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