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

        /// @section Concepts and type traits

        template <typename T>
        struct is_optional : std::false_type {};

        template <typename T>
        struct is_optional<std::optional<T>> : std::true_type {};

        template <typename T>
        inline constexpr bool is_optional_v = is_optional<T>::value;

        struct Flags;

        template <typename T>
        concept HasSetFlag = std::is_class_v<T> && requires(T t, Flags &flags) {
            t.set_flag(flags);
        };


        /// @section Helper structs

        /// @brief Collection of booleans which can be set via annotations.
        /// @note The flag name usually correspond to CLI11 function names that set the respective behavior.
        struct Flags {
            bool capture_default_str = false;
            bool as_option = false;
            bool as_flag = false;
            bool ignore_case = false;
            bool ignore_underscore = false;
            bool required = false;

            /// @brief Update (member-wise logical OR) the flags with another Flags object
            /// @param other The other Flags object to update from
            /// Performs a logical OR on each boolean member of the Flags struct.
            constexpr void update(const Flags &other) {
                constexpr auto access_ctx = std::meta::access_context::current();
                template for (constexpr auto member : define_static_array(nonstatic_data_members_of(^^Flags, access_ctx))) {
                    if constexpr (!has_identifier(member) || !std::is_same_v<typename[:type_of(member):], bool>)
                        continue;
                    (*this).[:member:] |= other.[:member:];
                }
            } // update
        }; // struct Flags

        /// @brief A struct that represents either one or two values of type T
        template <typename T>
        struct OneOrTwo {
            T value_min, value_max;
            bool two_args;
        };

        /// @brief Parameters used for CLI11-function 'expected'
        struct ExpectedParams: OneOrTwo<int> {};

        /// @brief Parameters used for CLI11-function 'require_option'
        struct RequireOption: OneOrTwo<int> {};

        /// @brief Summarizes annotations specified for an option
        struct OptionAnnotations {
            std::string identifier;  ///< The name of the variable associated with the option
            std::string description;
            std::string name;
            std::string envname;
            //
            std::string label;  ///< The label used to reference the option
            //
            std::vector<std::string> needs;
            std::vector<std::string> excludes;
            //
            std::optional<char> delimiter;
            int allow_extra_args = 2; // unset
            Flags flags;
            //
            std::optional<ExpectedParams> expected_params;
            std::optional<std::string> group;
            std::optional<CLI::MultiOptionPolicy> multi_option_policy;
            //
            bool ignore = false;
            //
            CLI::Option *cli_option = nullptr;
        }; // struct OptionAnnotations

        struct StructChars {
            std::string type_name;
            std::string app_description;
            std::string app_name;
        }; // struct StructChars


        /// @section Annotation tags

        struct StrWrapper
        {
            const char *text;
        };
        struct TwoStrWrapper
        {
            const char *text1;
            const char *text2;
        };
        struct AppDescription: StrWrapper {};
        struct AppName: StrWrapper {};

        struct OptionDescription: StrWrapper {};
        struct OptionName: StrWrapper {};
        struct EnvName: StrWrapper {};
        struct Label: StrWrapper {};
        struct Needs: StrWrapper {};
        struct Excludes: StrWrapper {};
        struct Group: StrWrapper {};
        struct OptionGroup: TwoStrWrapper {};
        struct MultiOptionPolicy {
            CLI::MultiOptionPolicy policy;
        };
        struct Delimiter {
            char value;
        };
        struct AllowExtraArgs {
            bool value;
        };
        struct CaptureDefaultStr {
            constexpr void set_flag(Flags &flags) const {
                flags.capture_default_str = true;
            }
        };
        struct AsOption {
            constexpr void set_flag(Flags &flags) const {
                flags.as_option = true;
            }
        };
        struct AsFlag {
            constexpr void set_flag(Flags &flags) const {
                flags.as_flag = true;
            }
        };
        struct IgnoreCase {
            constexpr void set_flag(Flags &flags) const {
                flags.ignore_case = true;
            }
        };
        struct IgnoreUnderscore {
            constexpr void set_flag(Flags &flags) const {
                flags.ignore_underscore = true;
            }
        };
        struct Required {
            constexpr void set_flag(Flags &flags) const {
                flags.required = true;
            }
        };
        struct Ignore {};


        /// @section Functions

        /// @brief Analyzes annotations of struct T
        /// @tparam T The type of the struct to analyze annotations from
        /// @return A StructChars object containing the extracted information
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

        using MapOptT = std::map<std::string, CLI::Option*>;

        void insert_option_by_label(MapOptT &map_opt, const std::string &label, CLI::Option *opt) {
            if (auto it = map_opt.find(label); it != map_opt.end()) {
                if (it->second == opt) return;
                throw std::runtime_error("Option with label '" + label + "' already exists.");
            }
            map_opt[label] = opt;
        } // insert_option_by_label

        /// @brief Steps through all non-static data members of struct T and collects information from their annotations
        /// @tparam T The type of the struct to analyze
        /// @return A vector of OptionAnnotations containing the collected information
        template <typename T>
        std::vector<OptionAnnotations> get_all_option_annotations() {
            constexpr auto access_ctx = std::meta::access_context::current();
            std::vector<OptionAnnotations> all_annotations;
            template for (constexpr auto member : define_static_array(nonstatic_data_members_of(^^T, access_ctx))) {
                if constexpr (!has_identifier(member))
                    continue;
                OptionAnnotations o_anno;
                o_anno.identifier = identifier_of(member);
                if constexpr (std::define_static_array(annotations_of_with_type(member, ^^Ignore)).size() > 0) {
                    o_anno.ignore = true;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^Flags)))
                {
                    constexpr auto value = std::meta::extract<Flags>(anno);
                    o_anno.flags.update(value);
                }
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
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^EnvName)))
                {
                    constexpr auto value = std::meta::extract<EnvName>(anno);
                    o_anno.envname = value.text;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^Label)))
                {
                    constexpr auto value = std::meta::extract<Label>(anno);
                    o_anno.label = value.text;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^Needs)))
                {
                    constexpr auto value = std::meta::extract<Needs>(anno);
                    o_anno.needs.push_back(value.text);
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^Excludes)))
                {
                    constexpr auto value = std::meta::extract<Excludes>(anno);
                    o_anno.excludes.push_back(value.text);
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^Group)))
                {
                    constexpr auto value = std::meta::extract<Group>(anno);
                    o_anno.group = value.text;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^MultiOptionPolicy)))
                {
                    constexpr auto value = std::meta::extract<MultiOptionPolicy>(anno);
                    o_anno.multi_option_policy = value.policy;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^ExpectedParams)))
                {
                    constexpr auto value = std::meta::extract<ExpectedParams>(anno);
                    o_anno.expected_params = value;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^Delimiter)))
                {
                    constexpr auto value = std::meta::extract<Delimiter>(anno);
                    o_anno.delimiter = value.value;
                }
                template for (constexpr auto anno : std::define_static_array(annotations_of_with_type(member, ^^AllowExtraArgs)))
                {
                    constexpr auto value = std::meta::extract<AllowExtraArgs>(anno);
                    o_anno.allow_extra_args = int(value.value);
                }
                all_annotations.push_back(o_anno);
            }
            return all_annotations;
        } // get_all_option_annotations

        OptionAnnotations &
        find_option_annotation(std::string_view identifier, unsigned int index,
                               std::vector<OptionAnnotations> &all_annotations) {
            if (index < all_annotations.size() && all_annotations[index].identifier == identifier)
                return all_annotations[index];
            for (auto &anno : all_annotations) {
                if (anno.identifier == identifier) {
                    return anno;
                }
            }
            throw std::runtime_error("Option annotation not found for identifier: " + std::string(identifier));
        } // find_option_annotation

        /// @brief Calls needs and excludes functions on CLI options based on annotations
        /// @param map_opt Labels associated with CLI option pointers
        /// @param option_annotations Vector containing all option annotations
        void set_needs_excludes(MapOptT &map_opt, std::vector<OptionAnnotations> const &option_annotations) {
            for (auto const &anno : option_annotations) {
                if (anno.ignore)
                    continue;
                for (auto const &need : anno.needs) {
                    auto trgt_opt = map_opt.find(need);
                    if (trgt_opt == map_opt.end())
                        throw std::runtime_error("Needs label not found: " + need + " for option: " + anno.identifier);
                    auto ptr = anno.cli_option;
                    if (!ptr)
                        throw std::runtime_error("(needs) CLI option pointer is null for option: " + anno.identifier);
                    anno.cli_option->needs(trgt_opt->second);
                }
                for (auto const &exclude : anno.excludes) {
                    auto trgt_opt = map_opt.find(exclude);
                    if (trgt_opt == map_opt.end())
                        throw std::runtime_error("Excludes label not found: " + exclude + " for option: " + anno.identifier);
                    auto ptr = anno.cli_option;
                    if (!ptr)
                        throw std::runtime_error("(excludes) CLI option pointer is null for option: " + anno.identifier);
                    ptr->excludes(trgt_opt->second);
                }
            }
        } // set_needs_excludes

        /// @brief Steps through all non-static data members of the options struct T and configures the CLI::App instance accordingly
        /// @tparam T The type of the options struct
        /// @param app The CLI::App instance to configure
        /// @param options The options struct to populate with parsed values
        template <typename T>
        void parse_options(CLI::App &app, T &options, MapOptT &map_opt) {
            constexpr auto access_ctx = std::meta::access_context::current();
            auto all_annotations = get_all_option_annotations<T>();

            constexpr auto r = std::define_static_array(annotations_of_with_type(^^T, ^^RequireOption));
            if constexpr (r.size() > 0)
            {
                constexpr auto req_anno = r[r.size()-1];
                constexpr auto req_value = std::meta::extract<RequireOption>(req_anno);
                if (req_value.two_args)
                    app.require_option(req_value.value_min, req_value.value_max);
                else
                    app.require_option(req_value.value_min);
            }

            unsigned int index = 0;
            template for (constexpr auto member : define_static_array(nonstatic_data_members_of(^^T, access_ctx)))
            {
                if constexpr (!has_identifier(member))
                    continue;

                constexpr std::string_view identifier = identifier_of(member);
                // std::println("Processing member: {}", identifier);
                OptionAnnotations &option_annotations = find_option_annotation(identifier, index, all_annotations);
                index++;
                if (option_annotations.ignore)
                    continue;

                constexpr auto opt_group = std::define_static_array(annotations_of_with_type(member, ^^OptionGroup));
                if constexpr (opt_group.size() > 0)
                {
                    constexpr auto anno = opt_group[opt_group.size()-1];
                    constexpr auto value = std::meta::extract<OptionGroup>(anno);
                    auto *group = app.add_option_group(value.text1, value.text2);
                    details::parse_options(*group, options.[:member:], map_opt);
                    continue;
                }

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
                constexpr bool member_can_be_flag = std::is_arithmetic_v<typename[:mt:]> && requires {
                    app.add_flag(option_name, options.[:member:], option_annotations.description);
                };

                CLI::Option *opt;
                bool option_added_as_flag = false;
                if constexpr (member_can_be_flag) {
                    if ((member_is_bool && !option_annotations.flags.as_option) || option_annotations.flags.as_flag) {
                        opt = app.add_flag(option_name, options.[:member:], option_annotations.description);
                        option_added_as_flag = true;
                    }
                }
                if constexpr (opt_group.size() == 0)  // Prevent compiler from generating code for add_option on an inappropriate type
                {
                    if (!option_added_as_flag) {
                        if (option_annotations.flags.as_flag) {
                            throw std::runtime_error("Option type does not support flags: " + std::string(identifier));
                        }
                        opt = app.add_option(option_name, options.[:member:], option_annotations.description);
                        if ((!has_default_member_initializer(member) && !member_is_optional) || option_annotations.flags.required) {
                            opt->required();
                        }
                    }
                }
                option_annotations.cli_option = opt;
                if (!option_annotations.label.empty()) {
                    insert_option_by_label(map_opt, option_annotations.label, opt);
                }
                if (!option_annotations.envname.empty()) {
                    opt->envname(option_annotations.envname);
                }
                if (option_annotations.flags.capture_default_str) {
                    opt->capture_default_str();
                }
                if (option_annotations.flags.ignore_case) {
                    opt->ignore_case();
                }
                if (option_annotations.flags.ignore_underscore) {
                    opt->ignore_underscore();
                }
                if (option_annotations.delimiter.has_value()) {
                    opt->delimiter(option_annotations.delimiter.value());
                }
                if (option_annotations.allow_extra_args < 2) {
                    opt->allow_extra_args(bool(option_annotations.allow_extra_args));
                }
                if (option_annotations.group.has_value()) {
                    opt->group(option_annotations.group.value());
                }
                if (option_annotations.multi_option_policy.has_value()) {
                    opt->multi_option_policy(option_annotations.multi_option_policy.value());
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
            }
            // Now that we know all labels, we can set the needs and excludes relationships for each option
            set_needs_excludes(map_opt, all_annotations);
        } // parse_options<T>
    } // namespace details


    /// @section Annotation functions and tags

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
    consteval details::EnvName
    envname(std::string_view s)
    {
        return {std::define_static_string(s)};
    }
    consteval details::Label
    label(std::string_view s)
    {
        return {std::define_static_string(s)};
    }
    consteval details::Needs
    needs(std::string_view s)
    {
        return {std::define_static_string(s)};
    }
    consteval details::Excludes
    excludes(std::string_view s)
    {
        return {std::define_static_string(s)};
    }
    consteval details::Group
    group(std::string_view s)
    {
        return {std::define_static_string(s)};
    }
    consteval details::MultiOptionPolicy
    multi_option_policy(CLI::MultiOptionPolicy value)
    {
        return {value};
    }
    consteval details::Delimiter
    delimiter(char c)
    {
        return {c};
    }
    consteval details::AllowExtraArgs
    allow_extra_args(bool value)
    {
        return {value};
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
    consteval details::OptionGroup
    add_option_group(std::string_view name, std::string_view description="")
    {
        return {std::define_static_string(name), std::define_static_string(description)};
    }
    consteval details::RequireOption
    require_option(int value)
    {
        return {value, value, false};
    }
    consteval details::RequireOption
    require_option(int value_min, int value_max)
    {
        return {value_min, value_max, true};
    }
    inline constexpr details::CaptureDefaultStr capture_default_str{};
    inline constexpr details::AsOption as_option{};
    inline constexpr details::AsFlag as_flag{};
    inline constexpr details::IgnoreCase ignore_case{};
    inline constexpr details::IgnoreUnderscore ignore_underscore{};
    inline constexpr details::Required required{};
    inline constexpr details::Ignore ignore{};

    template <details::HasSetFlag... T>
    inline constexpr auto params(T... t) {
        details::Flags flags{};
        (t.set_flag(flags), ...);
        return flags;
    }


    /// @section Main

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
        details::MapOptT map_opt;
        details::parse_options(app, options, map_opt);
    } // get_app<T>
} // namespace CLI26
