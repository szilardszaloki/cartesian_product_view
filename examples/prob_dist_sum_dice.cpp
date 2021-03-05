#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>

#include "../cartesian_product_view.h"

struct accumulate_fn {
    template <
        std::input_iterator I,
        std::sentinel_for<I> S,
        typename T = std::iter_value_t<I>,
        typename Op = std::plus<>,
        typename Proj = std::identity
    >
    constexpr auto operator()(I first, S last, T init = {}, Op op = {}, Proj proj = {}) const {
        while (first != last) {
            init = std::invoke(op, std::move(init), std::invoke(proj, *first));
            ++first;
        }

        return init;
    }

    template <
        std::ranges::input_range R,
        typename T = std::ranges::range_value_t<R>,
        typename Op = std::plus<>,
        typename Proj = std::identity
    >
    constexpr auto operator()(R&& r, T init = {}, Op op = {}, Proj proj = {}) const {
        return (*this)(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::move(op), std::move(proj));
    }
};

inline constexpr accumulate_fn accumulate{};

template <std::size_t dice_count>
auto prob_dist_sum_dice() {
    std::size_t dice[]{ 1, 2, 3, 4, 5, 6 };
    auto tuples =
        [&]<std::size_t... indices>(std::index_sequence<indices...>) {
            return cartesian_product((static_cast<void>(indices), dice)...);
        }(std::make_index_sequence<dice_count>{})
    ;

    std::map<std::size_t, std::size_t> distribution{};
    std::ranges::for_each(
        tuples,
        [&](auto const& tuple) {
            ++distribution[
                std::apply(
                    [](auto const... elements) {
                        return (std::size_t{} + ... + elements);
                    },
                    tuple
                )
            ];
        }
    );

#ifdef __GNUC__
    return std::pair{ accumulate(distribution | std::views::values), std::move(distribution) };
#else
    return std::pair{
        std::accumulate(
            distribution.cbegin(),
            distribution.cend(),
            std::size_t{},
            [](std::size_t const acc, auto const& pair) {
                return acc + pair.second;
            }
        ),
        std::move(distribution)
    };
#endif
}

int main(int const, char const* const []) {
    auto const [denominator, distribution] = prob_dist_sum_dice<2>();
    for (auto const& [sum, nominator] : distribution) {
        std::cout << sum << ": " << nominator << '/' << denominator << '\n';
    }

    return 0;
}