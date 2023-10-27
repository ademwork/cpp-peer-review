#include <vector>
#include <iostream>

using namespace std::literals;

struct ParamConnectDB
{
    std::string db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;

    // string_view сохраняется в структуре как string.
    ParamConnectDB& SetDbName(std::string_view db_name) {
        this->db_name = std::move(std::string{db_name});
        return *this;
    }

    ParamConnectDB& SetDbTimeout(int db_connection_timeout) {
        this->db_connection_timeout = db_connection_timeout;
        return *this;
    }

    ParamConnectDB& SetAllowException(bool db_allow_exceptions) {
        this->db_allow_exceptions = db_allow_exceptions;
        return *this;
    }

    ParamConnectDB& SetLogLevel(DBLogLevel db_log_level) {
        this->db_log_level = db_log_level;
        return *this;
    }

};


struct ParamQuery
{
    int min_age;
    int max_age;
    std::string_view name_filter;

    ParamQuery& SetDbTimeout(int min_age) {
        this->min_age = min_age;
        return *this;
    }

    ParamQuery& SetAllowException(int max_age) {
        this->max_age = max_age;
        return *this;
    }

    ParamQuery& SetLogLevel(std::string_view name_filter) {
        this->name_filter = name_filter;
        return *this;
    }

};

DBHandler ConnectDB(ParamConnectDB param_connect)
{
    DBConnector connector(param_connect.db_allow_exceptions, param_connect.db_log_level);
    
    DBHandler db;

    if (param_connect.db_name.starts_with("tmp."s))
    {
        db = connector.ConnectTmp(param_connect.db_name, param_connect.db_connection_timeout);
    }
    else
    {
        db = connector.Connect(param_connect.db_name, param_connect.db_connection_timeout);
    }
    return db;
}

DBQuery queryDB(DBHandler &db, ParamQuery param_query)
{
    std::ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << param_query.min_age << " and "s << param_query.max_age << " "s
        << "and Name like '%"s << db.Quote(param_query.name_filter) << "%'"s;

    return query(query_str.str());    
}


std::vector<Person> ReadPersonsFromDB(DBHandler &db,const DBQuery &query)
{
    std::vector<Person> persons;
    for (auto [name, age] : db.LoadRows<std::string, int>(query))
    {
        persons.push_back({ move(name), age });
    }
    return persons;
}



std::vector<Person> LoadPersons(ParamConnectDB param_connect, ParamQuery param_query)
{
    DBHandler db = ConnectDB(param_connect);

    if (param_connect.db_allow_exceptions == false && db.IsOK() == false)
    {
        return {};
    }
    else
    {
        DBQuery query = queryDB(db, param_query);

        return ReadPersonsFromDB(db,query);
    }
}