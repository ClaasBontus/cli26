#include <print>
#include <cstdint>
#include <string>
#include <vector>
#include <complex>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct File
{
    [[=option_description("Name of the file")]]
    [[=option_name("-f,--file")]]
    std::string filename;

    [[=option_description("Indicates if the file is a text file")]]
    [[=option_name("-t,--text")]]
    bool text=false;
};

struct Dateformat
{
    [[=option_description("Date format string")]]
    [[=option_name("-d,--date")]]
    std::string format;
};

struct
[[=app_description("Options composed of inheritance")]]
Options: File, Dateformat
{ };

template <> struct std::formatter<File> : UniversalFormatter { };
template <> struct std::formatter<Dateformat> : UniversalFormatter { };
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
