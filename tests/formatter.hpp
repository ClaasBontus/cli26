#include <meta>
#include <format>
#include <optional>
#include <print>

// Taken from
// https://accu.org/journals/overload/34/193/decock/


struct EnumFormatter
{
    constexpr auto parse(auto& ctx) { return ctx.begin(); }

    template<typename E>
    requires std::is_enum_v<E>
    auto format(const E& value, auto& ctx) const
    {
        std::string_view label{"unknown"};
        template for(constexpr auto& enu : std::define_static_array(
                                std::meta::enumerators_of(^^E)))
        {
            if (value == [:enu:])
            {
                label = std::meta::identifier_of(enu);
                break;
            }
        }
        auto out = ctx.out();
        std::format_to(out, "{}", label);
        return out;
    }
};

struct UniversalFormatter
{
    constexpr auto parse(auto& ctx) { return ctx.begin(); }

    template<typename T>
    auto format(const T& t, auto& ctx) const
    {
        auto out = ctx.out();

        std::string_view typeName = "unnamed";
        if constexpr (has_identifier(^^T))
        {
            typeName = identifier_of(^^T);
        }
        std::format_to(out, "{}{{", typeName);

        auto delim = [first = true, &out]() mutable
        {
            if (!first)
            {
                std::format_to(out, ", ");
            }
            first = false;
        };

        constexpr auto access_ctx = std::meta::access_context::unchecked();

        template for (constexpr auto base : define_static_array(bases_of(^^T, access_ctx)))
        {
            delim();
            std::format_to(out, "{}", (typename [: type_of(base) :] const&)(t));
        }

        template for (constexpr auto member :
                        define_static_array(nonstatic_data_members_of(^^T, access_ctx)))
        {
            delim();
            std::format_to(out, ".{} = {}",  identifier_of(member), t.[:member:]);
        }

        std::format_to(out, "}}");
        return out;
    }
};

template<typename T>
struct std::formatter<std::optional<T>> : std::formatter<T>
{
    auto format(const std::optional<T>& value, auto& ctx) const
    {
        if (value)
        {
            return std::formatter<T>::format(*value, ctx);
        }
        return std::format_to(ctx.out(), "nullopt");
    }
};

template<typename T>
struct std::formatter<std::complex<T>> : std::formatter<T>
{
    auto format(const std::complex<T>& value, auto& ctx) const
    {
        return std::format_to(ctx.out(), "{}+i{}", value.real(), value.imag());
    }
};
