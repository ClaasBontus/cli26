#include <print>
#include <cstdint>
#include <string>
#include <vector>
#include <complex>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
[[=app_description("Test various datatypes")]]
Options
{
    [[=params(as_option, ignore_case, ignore_underscore)]]
    [[=option_description("Required boolean")]]
    bool colored;

    [[=params(as_flag)]]
    [[=option_name("-v,--verbose")]]
    [[=option_description("Int used as flag")]]
    int verbose;

    [[=option_description("Unsigned integer 64 bit")]]
    uint64_t count;

    [[=option_description("String")]]
    std::string name;

    [[=option_description("Vector of integers")]]
    std::vector<int> values;

    [[=expected(1, 3)]]
    [[=option_description("Optional vector of strings with 1 to 3 elements")]]
    std::vector<std::string> tags = {};

    [[=expected(2)]]
    [[=delimiter(',')]]
    [[=option_description("Vector of doubles with 2 expected elements separated by comma")]]
    std::vector<double> numbers;

    [[=delimiter(',')]]
    [[=allow_extra_args(false)]]
    [[=option_description("Vector of strings with comma delimiter and no extra args")]]
    std::vector<std::string> args;

    [[=option_description("Optional integer value")]]
    std::optional<int> oval;

    [[=option_name("pos1")]]
    [[=option_description("Positional argument")]]
    std::string path="";

    [[=option_name("--complex")]]
    [[=option_description("Complex number")]]
    std::complex<double> complex_value={0.0, 0.0};

    [[=ignore]]
    double ignored_value;
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
