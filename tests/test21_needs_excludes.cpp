#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
[[=app_description("Using needs and excludes annotations")]]
Options
{
    [[=option_description("A string")]]
    [[=needs("--age")]]
    std::optional<std::string> name;

    [[=option_description("An int option that requires name")]]
    [[=needs("--name")]]
    std::optional<int> age;

    [[=option_description("Just another string")]]
    std::optional<std::string> java;

    [[=option_description("A string with exclusion")]]
    [[=excludes("--java")]]
    std::optional<std::string> programmer;
};

template <> struct std::formatter<Options> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::add_content(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
