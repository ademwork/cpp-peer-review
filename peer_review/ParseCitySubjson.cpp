#include <string>
#include <vector>


#define PARAM_COUNTRY(key) country_obj[key].AsString()
#define PARAM_CITY(key) city_obj[key].AsString()



struct ParamCountry
{
    ParamCountry(Country& param) : name(param.country.name), iso_code(param.iso_code), phone_code(param.phone_code) , time_zone(param.phone_code), languages(param.languages)
    {

    }
    std::string &name;
    std::string &iso_code;
    std::string &phone_code;
    std::string &time_zone;
    std::vector<Language> &languages;
};



// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const ParamCountry&  param_country) 
{
    for (const auto& city_json : json.AsList()) 
    {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({ PARAM_CITY("name"s), PARAM_CITY("iso_code"s),param_country.phone_code + PARAM_CITY("phone_code"s), param_country.name, param_country.iso_code,
                          param_country.time_zone,param_country.languages});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) 
{
    for (const auto& country_json : json.AsList()) 
    {
        const auto& country_obj = country_json.AsObject();

        countries.push_back({
                    PARAM_COUNTRY("name"s),
                    PARAM_COUNTRY("iso_code"s),
                    PARAM_COUNTRY("phone_code"s),
                    PARAM_COUNTRY("time_zone"s),
                });

        Country& country = countries.back();

        for (const auto& lang_obj : country_obj["languages"s].AsList()) 
        {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }

        ParamCountry param_country( country );


        ParseCitySubjson(cities, country_obj["cities"s], param_country);
    }
}