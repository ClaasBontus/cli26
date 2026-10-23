#include <print>
#include <cstdint>
#include <string>
#include <vector>
#include <cli26.hpp>

#include "formatter.hpp"

struct
[[=CLI26::app_description("Test various datatypes")]]
Options
{
    [[=CLI26::as_option]]
    [[=CLI26::option_description("Required boolean")]]
    bool colored;

    [[=CLI26::as_flag]]
    [[=CLI26::option_name("-v,--verbose")]]
    [[=CLI26::option_description("Int used as flag")]]
    int verbose;

    [[=CLI26::option_description("Unsigned integer 64 bit")]]
    uint64_t count;

    [[=CLI26::option_description("String")]]
    std::string name;

    [[=CLI26::option_description("Vector of integers")]]
    std::vector<int> values;

    [[=CLI26::expected(1, 3)]]
    [[=CLI26::option_description("Optional vector of strings with 1 to 3 elements")]]
    std::vector<std::string> tags = {};

    [[=CLI26::expected(2)]]
    [[=CLI26::option_description("Vector of doubles with 2 expected elements")]]
    std::vector<double> numbers;

    [[=CLI26::option_description("Optional integer value")]]
    std::optional<int> oval;
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
