#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <meta>
#include <print>
#include <CLI11.hpp>

namespace CLI26 {
    using App = CLI::App;

    
    namespace details {
        template <typename T>
        struct is_optional : std::false_type {};

        template <typename T>
        struct is_optional<std::optional<T>> : std::true_type {};

        template <typename T>
        inline constexpr bool is_optional_v = is_optional<T>::value;

        struct StrWrapper
        {
            const char *text;
        }; // struct StrWrapper
        struct AppDescription: StrWrapper {};
        struct AppName: StrWrapper {};

        struct OptionDescription: StrWrapper {};
        struct OptionName: StrWrapper {};
        struct CaptureDefaultStr {};
        struct AsOption {};
        struct AsFlag {};

        struct ExpectedParams {
            int value_min, value_max;
            bool two_args;
        };

        struct StructChars {
            std::string type_name;
            std::string app_description;
            std::string app_name;
        }; // struct StructChars

        struct OptionAnnotations {
            std::string identifier;
            std::string description;
            std::string name;
            bool capture_default_str = false;
            bool as_option = false;
            bool as_flag = false;
            std::optional<ExpectedParams> expected_params;
        }; // struct OptionAnnotations

        template <typename T>
        StructChars get_struct_chars() {
            StructChars chars;
            if constexpr (has_identifier(^^T))
            {
                chars.type_name = identifier_of(^^T);
            }
            template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(^^T, ^^AppDescription)))
            {
                constexpr auto value = std::meta::extract<AppDescription>(anno);
                chars.app_description = value.text;
            }
            template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(^^T, ^^AppName)))
            {
                constexpr auto value = std::meta::extract<AppName>(anno);
                chars.app_name = value.text;
            }
            return chars;
        } // get_struct_chars<T>

        template <typename T>
        std::vector<OptionAnnotations> get_all_option_annotations() {
            constexpr auto access_ctx = std::meta::access_context::current();
            std::vector<OptionAnnotations> all_annotations;
            template for (constexpr auto member : define_static_array(nonstatic_data_members_of(^^T, access_ctx))) {
                if constexpr (!has_identifier(member))
                    continue;
                OptionAnnotations o_anno;
                o_anno.identifier = identifier_of(member);
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^OptionDescription)))
                {
                    constexpr auto value = std::meta::extract<OptionDescription>(anno);
                    o_anno.description = value.text;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^OptionName)))
                {
                    constexpr auto value = std::meta::extract<OptionName>(anno);
                    o_anno.name = value.text;
                }
                if constexpr (std::define_static_array(annotations_of_with_type(member, ^^CaptureDefaultStr)).size() > 0)
                {
                    o_anno.capture_default_str = true;
                }
                if constexpr (std::define_static_array(annotations_of_with_type(member, ^^AsOption)).size() > 0)
                {
                    o_anno.as_option = true;
                }
                if constexpr (std::define_static_array(annotations_of_with_type(member, ^^AsFlag)).size() > 0)
                {
                    o_anno.as_flag = true;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^ExpectedParams)))
                {
                    constexpr auto value = std::meta::extract<ExpectedParams>(anno);
                    o_anno.expected_params = value;
                }
                all_annotations.push_back(o_anno);
            }
            return all_annotations;
        } // get_all_option_annotations

        OptionAnnotations const &
        get_option_annotations(std::string_view identifier, unsigned int index,
                               std::vector<OptionAnnotations> const &all_annotations) {
            if (index < all_annotations.size() && all_annotations[index].identifier == identifier)
                return all_annotations[index];
            for (auto &anno : all_annotations) {
                if (anno.identifier == identifier) {
                    return anno;
                }
            }
            throw std::runtime_error("Option annotation not found for identifier: " + std::string(identifier));
        } // get_option_annotations

        template <typename T>
        void parse_options(CLI::App &app, T &options) {
            constexpr auto access_ctx = std::meta::access_context::current();
            auto all_annotations = get_all_option_annotations<T>();
            unsigned int index = 0;
            template for (constexpr auto member : define_static_array(nonstatic_data_members_of(^^T, access_ctx)))
            {
                if constexpr (!has_identifier(member))
                    continue;

                constexpr std::string_view identifier = identifier_of(member);
                OptionAnnotations option_annotations = get_option_annotations(identifier, index, all_annotations);
                
                constexpr auto mt = type_of(member);
                // constexpr std::string_view tname = display_string_of(mt);
                auto option_name = [&] {
                    if (!option_annotations.name.empty())
                    return std::string(option_annotations.name);
                    if constexpr (identifier.length() == 1)
                        return std::string("-") + std::string(identifier);
                    return std::string("--") + std::string(identifier);
                }();
                constexpr bool member_is_optional = is_optional_v<typename[:mt:]>;
                constexpr bool member_is_bool = std::is_same_v<typename[:mt:], bool>;
                
                CLI::Option *opt;
                if ((member_is_bool && !option_annotations.as_option) || option_annotations.as_flag) {
                    opt = app.add_flag(option_name, options.[:member:], option_annotations.description);
                } else {
                    opt = app.add_option(option_name, options.[:member:], option_annotations.description);
                    if (!has_default_member_initializer(member) && !member_is_optional) {
                        opt->required();
                    }
                }
                if (option_annotations.capture_default_str) {
                    opt->capture_default_str();
                }
                if (option_annotations.expected_params.has_value()) {
                    auto &ep = option_annotations.expected_params.value();
                    if (ep.two_args) {
                        opt->expected(ep.value_min, ep.value_max);
                    }
                    else {
                        opt->expected(ep.value_min);
                    }
                }
                ++index;
            }
        } // parse_options<T>
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

    consteval details::OptionDescription
    option_description(std::string_view s)
    {
        return {std::define_static_string(s)};
    }
    consteval details::OptionName
    option_name(std::string_view s)
    {
        return {std::define_static_string(s)};
    }
    consteval details::ExpectedParams
    expected(int value)
    {
        return {value, value, false};
    }
    consteval details::ExpectedParams
    expected(int value_min, int value_max)
    {
        return {value_min, value_max, true};
    }
    inline constexpr details::CaptureDefaultStr capture_default_str{};
    inline constexpr details::AsOption as_option{};
    inline constexpr details::AsFlag as_flag{};



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

        details::parse_options(app, options);
    } // get_app<T>
} // namespace CLI26
