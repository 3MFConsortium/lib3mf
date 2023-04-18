
#include <memory>

namespace NMR::utils
{
 namespace detail
    {
        template <int First, int Last>
        struct StaticFor
        {
            template <typename Fn>
            void operator()(Fn const & fn) const
            {
                if (First < Last)
                {
                    fn(First);
                    StaticFor<First + 1, Last>()(fn);
                }
            }
        };

        template <class Tup, class Func, std::size_t... Is>
        constexpr void staticForImpl(Tup && t, Func && f, std::index_sequence<Is...>)
        {
            (f(std::integral_constant<std::size_t, Is>{}, std::get<Is>(t)), ...);
        }
    }

    template <class... T, class Func>
    constexpr void staticFor(std::tuple<T...> & t, Func && f)
    {
        detail::staticForImpl(t, std::forward<Func>(f), std::make_index_sequence<sizeof...(T)>{});
    }
}

