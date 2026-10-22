#pragma once

#include <string>
#include <string_view>
#include <meta>
#include <print>
#include <CLI11.hpp>

namespace CLI26 {
    using App = CLI::App;

    
    namespace details {
        struct StrWrapper
        {
            const char *text;
        }; // struct StrWrapper
        struct AppDescription: StrWrapper {};
        struct AppName: StrWrapper {};

        struct StructChars {
            std::string type_name;
            std::string app_description;
            std::string app_name;
        }; // struct StructChars

        template <typename T>
        StructChars get_struct_chars() {
            StructChars chars;
            if constexpr (has_identifier(^^T))
            {
                chars.type_name = identifier_of(^^T);
            }
            template for (constexpr auto anno : define_static_array(annotations_of_with_type(^^T, ^^AppDescription)))
            {
                constexpr auto value = std::meta::extract<AppDescription>(anno);
                chars.app_description = value.text;
            }
            template for (constexpr auto anno : define_static_array(annotations_of_with_type(^^T, ^^AppName)))
            {
                constexpr auto value = std::meta::extract<AppName>(anno);
                chars.app_name = value.text;
            }

            return chars;
        }
    } // namespace details

    consteval details::AppDescription
    app_description(std::string_view s)
    {
        return {std::define_static_string(s)};
    }
    consteval details::AppName
    app_name(std::string_view s)
    {
        return {std::define_static_string(s)};
    }

    template <typename T>
    void get_app(CLI::App &app, T &options)
    {
        details::StructChars chars = details::get_struct_chars<T>();
        if (!chars.app_description.empty()) {
            app.description(chars.app_description);
        }
        if (!chars.app_name.empty()) {
            app.name(chars.app_name);
        }
    } // get_app
} // namespace CLI26
