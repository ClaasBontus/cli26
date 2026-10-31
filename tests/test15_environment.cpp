#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
[[=app_name("test01_hello_world")]]
[[=app_description("Hello World App")]]
Options
{
    [[=option_name("-n,--name")]]
    [[=option_description("Some name")]]
    std::string name;

    [[=option_description("Some path which can be specified via an environment variable")]]
    [[=envname("CLI26TESTPATH")]]
    std::string path="";
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
