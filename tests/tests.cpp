#include <forward_list>
#include <list>
#include <vector>

#include "gtest/gtest.h"

#include "../cartesian_product_view.h"

struct {
    template <typename R, typename T>
    requires std::ranges::input_range<R>
    void operator()(R&& actual, std::initializer_list<T>&& expected) const {
        (*this)(std::forward<R>(actual), expected);
    }

    template <typename R1, typename R2>
    requires (std::ranges::input_range<R1> && std::ranges::input_range<R2>)
    void operator()(R1&& actual, R2&& expected) const {
        auto begin0 = std::ranges::begin(actual);
        auto end0 = std::ranges::end(actual);
        auto begin1 = std::ranges::begin(expected);
        auto end1 = std::ranges::end(expected);

        while (begin0 != end0 && begin1 != end1) {
            (*this)(*begin0++, *begin1++);
        }

        EXPECT_EQ(begin0, end0);
        EXPECT_EQ(begin1, end1);
    }

    template <typename A, typename E>
    requires (!std::ranges::range<A> && !std::ranges::range<E>)
    void operator()(A&& actual, E&& expected) const {
        EXPECT_TRUE(std::forward<A>(actual) == std::forward<E>(expected));
    }
} constexpr check_equal;

std::forward_list forward_list{ '0', '1', '2' };
std::list list{ 0, 1, 2, 3 };
std::vector vector{ 0, 1, 2 };
auto empty = std::views::empty<int>;

// a cartesian_product_view<> is a bidirectional_range,
// if all of the constituent ranges are at least bidirectional_ranges
TEST(concept_checks, bidirectional_range) {
    {
        auto cpv = cartesian_product();
        static_assert(std::ranges::bidirectional_range<decltype(cpv)>);
        static_assert(!std::ranges::random_access_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(std::ranges::bidirectional_range<decltype(cpv)>);
        static_assert(!std::ranges::random_access_range<decltype(cpv)>);
    }
    {
        static_assert(std::ranges::contiguous_range<decltype(empty)>);
        auto cpv = cartesian_product(empty);
        static_assert(std::ranges::bidirectional_range<decltype(cpv)>);
        static_assert(!std::ranges::random_access_range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is never a borrowed_range
TEST(concept_checks, borrowed_range) {
    {
        auto cpv = cartesian_product();
        static_assert(!std::ranges::borrowed_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(!std::ranges::borrowed_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(!std::ranges::borrowed_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(!std::ranges::borrowed_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(!std::ranges::borrowed_range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is always a common_range
TEST(concept_checks, common_range) {
    {
        auto cpv = cartesian_product();
        static_assert(std::ranges::common_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(std::ranges::common_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(std::ranges::common_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(std::ranges::common_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(std::ranges::common_range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is never a contiguous_range
TEST(concept_checks, contiguous_range) {
    {
        auto cpv = cartesian_product();
        static_assert(!std::ranges::contiguous_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(!std::ranges::contiguous_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(!std::ranges::contiguous_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(!std::ranges::contiguous_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(!std::ranges::contiguous_range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is a forward_range,
// if all of the constituent ranges are at least forward_ranges and
// if there's at least one of them, that is not a bidirectional_range as well
TEST(concept_checks, forward_range) {
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(std::ranges::forward_range<decltype(cpv)>);
        static_assert(!std::ranges::bidirectional_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list, list);
        static_assert(std::ranges::forward_range<decltype(cpv)>);
        static_assert(!std::ranges::bidirectional_range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is always an input_range
TEST(concept_checks, input_range) {
    {
        auto cpv = cartesian_product();
        static_assert(std::ranges::input_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(std::ranges::input_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(std::ranges::input_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(std::ranges::input_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(std::ranges::input_range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is never an output_range
TEST(concept_checks, output_range) {
    {
        auto cpv = cartesian_product();
        static_assert(!std::ranges::output_range<decltype(cpv), std::ranges::range_value_t<decltype(cpv)>>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(!std::ranges::output_range<decltype(cpv), std::ranges::range_value_t<decltype(cpv)>>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(!std::ranges::output_range<decltype(cpv), std::ranges::range_value_t<decltype(cpv)>>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(!std::ranges::output_range<decltype(cpv), std::ranges::range_value_t<decltype(cpv)>>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(!std::ranges::output_range<decltype(cpv), std::ranges::range_value_t<decltype(cpv)>>);
    }
}

// a cartesian_product_view<> is never a random_access_range (at least currently)
TEST(concept_checks, random_access_range) {
    {
        auto cpv = cartesian_product();
        static_assert(!std::ranges::random_access_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(!std::ranges::random_access_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(!std::ranges::random_access_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(!std::ranges::random_access_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(!std::ranges::random_access_range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is always a range
TEST(concept_checks, range) {
    {
        auto cpv = cartesian_product();
        static_assert(std::ranges::range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(std::ranges::range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(std::ranges::range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(std::ranges::range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(std::ranges::range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is a sized_range,
// if all of the constituent ranges are sized_ranges
TEST(concept_checks, sized_range) {
    {
        auto cpv = cartesian_product();
        static_assert(std::ranges::sized_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(!std::ranges::sized_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(std::ranges::sized_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(std::ranges::sized_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(std::ranges::sized_range<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is always a view
TEST(concept_checks, view) {
    {
        auto cpv = cartesian_product();
        static_assert(std::ranges::view<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(std::ranges::view<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(std::ranges::view<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(std::ranges::view<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(std::ranges::view<decltype(cpv)>);
    }
}

// a cartesian_product_view<> is always a viewable_range
TEST(concept_checks, viewable_range) {
    {
        auto cpv = cartesian_product();
        static_assert(std::ranges::viewable_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(forward_list);
        static_assert(std::ranges::viewable_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(list);
        static_assert(std::ranges::viewable_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(vector);
        static_assert(std::ranges::viewable_range<decltype(cpv)>);
    }
    {
        auto cpv = cartesian_product(empty);
        static_assert(std::ranges::viewable_range<decltype(cpv)>);
    }
}

template <typename T>
struct range_associated_types : ::testing::Test {
    static constexpr bool deep_const = std::tuple_element_t<0, T>::value;
    using underlying_range_reference_t = std::tuple_element_t<1, T>;
    using expected_range_reference_t = std::tuple_element_t<2, T>;
    using expected_range_value_t = std::tuple_element_t<3, T>;
};

struct shallow_const : std::false_type {};
struct deep_const : std::true_type {};

using types =
    ::testing::Types<
        // T1: std::ranges::range_reference_t<> of the underlying view
        // T2: expected std::ranges::range_reference_t<> of cartesian_product_view
        // T3: expected std::ranges::range_value_t<> of cartesian_product_view
        //                        T1           T2                       T3
        std::tuple<shallow_const, int,         std::tuple<int>,         std::tuple<int>>,
        std::tuple<shallow_const, int&,        std::tuple<int&>,        std::tuple<int>>,
        std::tuple<shallow_const, int const&,  std::tuple<int const&>,  std::tuple<int>>,
        std::tuple<shallow_const, int&&,       std::tuple<int&&>,       std::tuple<int>>,
        std::tuple<shallow_const, int const&&, std::tuple<int const&&>, std::tuple<int>>,
        std::tuple<deep_const,    int,         std::tuple<int>,         std::tuple<int>>,
        std::tuple<deep_const,    int&,        std::tuple<int const&>,  std::tuple<int>>,
        std::tuple<deep_const,    int const&,  std::tuple<int const&>,  std::tuple<int>>,
        std::tuple<deep_const,    int&&,       std::tuple<int const&&>, std::tuple<int>>,
        std::tuple<deep_const,    int const&&, std::tuple<int const&&>, std::tuple<int>>
    >
;

TYPED_TEST_SUITE(range_associated_types, types);

TYPED_TEST(range_associated_types, range_reference_t) {
    int ints[]{ 1, 2, 3 };

    auto transform =
        ints
      | std::views::transform(
            [](int& i) -> decltype(auto) {
                return static_cast<typename TestFixture::underlying_range_reference_t>(i);
            }
        )
    ;

    constexpr cartesian_product_fn<TestFixture::deep_const> cartesian_product{};
    auto cpv = cartesian_product(transform);
    using type = std::conditional_t<TestFixture::deep_const, std::add_const_t<decltype(cpv)>, decltype(cpv)>;

    static_assert(std::same_as<std::ranges::range_reference_t<type>, typename TestFixture::expected_range_reference_t>);
}

TYPED_TEST(range_associated_types, range_value_t) {
    int ints[]{ 1, 2, 3 };

    auto transform =
        ints
      | std::views::transform(
            [](int& i) -> decltype(auto) {
                return static_cast<typename TestFixture::underlying_range_reference_t>(i);
            }
        )
    ;

    constexpr cartesian_product_fn<TestFixture::deep_const> cartesian_product{};
    auto cpv = cartesian_product(transform);
    using type = std::conditional_t<TestFixture::deep_const, std::add_const_t<decltype(cpv)>, decltype(cpv)>;

    static_assert(std::same_as<std::ranges::range_value_t<type>, typename TestFixture::expected_range_value_t>);
}

TEST(general, empty_set) {
    {
        auto cpv = cartesian_product();
        EXPECT_EQ(std::ranges::begin(cpv), std::ranges::end(cpv));
        EXPECT_EQ(std::ranges::size(cpv), 0);
        EXPECT_TRUE(std::ranges::empty(cpv));
        check_equal(cpv, std::initializer_list<std::tuple<>>{});
        check_equal(cpv | std::views::reverse, std::initializer_list<std::tuple<>>{});
    }
    {
        auto cpv = cartesian_product(vector, empty);
        EXPECT_EQ(std::ranges::begin(cpv), std::ranges::end(cpv));
        EXPECT_EQ(std::ranges::size(cpv), 0);
        EXPECT_TRUE(std::ranges::empty(cpv));
        check_equal(cpv, std::initializer_list<std::tuple<int&, char&>>{});
        check_equal(cpv | std::views::reverse, std::initializer_list<std::tuple<int&, char&>>{});
    }
}

TEST(general, simple_use_case) {
    check_equal(
        cartesian_product(vector, forward_list),
        {
            std::tuple{ 0, '0' },
            std::tuple{ 0, '1' },
            std::tuple{ 0, '2' },
            std::tuple{ 1, '0' },
            std::tuple{ 1, '1' },
            std::tuple{ 1, '2' },
            std::tuple{ 2, '0' },
            std::tuple{ 2, '1' },
            std::tuple{ 2, '2' }
        }
    );
}

TEST(general, not_common_range) {
    auto twv =
        list // [0, 1, 2, 3]
      | std::views::take_while(
            [](int const i) {
                return i < 3;
            }
        )
    ;

    auto cpv = cartesian_product(vector, twv);
    check_equal(
        cpv,
        {
            std::tuple{ 0, 0 },
            std::tuple{ 0, 1 },
            std::tuple{ 0, 2 },
            std::tuple{ 1, 0 },
            std::tuple{ 1, 1 },
            std::tuple{ 1, 2 },
            std::tuple{ 2, 0 },
            std::tuple{ 2, 1 },
            std::tuple{ 2, 2 }
        }
    );

    check_equal(
        cpv | std::views::reverse,
        {
            std::tuple{ 2, 2 },
            std::tuple{ 2, 1 },
            std::tuple{ 2, 0 },
            std::tuple{ 1, 2 },
            std::tuple{ 1, 1 },
            std::tuple{ 1, 0 },
            std::tuple{ 0, 2 },
            std::tuple{ 0, 1 },
            std::tuple{ 0, 0 }
        }
    );
}