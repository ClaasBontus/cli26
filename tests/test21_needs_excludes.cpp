#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
[[=app_description("Using needs and excludes annotations")]]
Options
{
    [[=option_description("A string with a label")]]
    [[=label("name_label")]]
    [[=needs("age_label")]]
    std::optional<std::string> name;

    [[=option_description("An int option that requires name")]]
    [[=label("age_label")]]
    [[=needs("name_label")]]
    std::optional<int> age;

    [[=option_description("Just a string with label")]]
    [[=label("java_label")]]
    std::optional<std::string> java;

    [[=option_description("A string with exclusion")]]
    [[=excludes("java_label")]]
    std::optional<std::string> programmer;
};

template <> struct std::formatter<Options> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::get_app(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
