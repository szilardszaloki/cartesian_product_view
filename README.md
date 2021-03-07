## A compact, `const`-propagating C++20 `cartesian_product_view<>`.
[![Windows Build Status](https://github.com/szilardszaloki/cartesian_product_view/actions/workflows/windows.yml/badge.svg)](https://github.com/szilardszaloki/cartesian_product_view/actions?query=workflow%3AWindows)
[![macOS Build Status](https://github.com/szilardszaloki/cartesian_product_view/actions/workflows/macos.yml/badge.svg)](https://github.com/szilardszaloki/cartesian_product_view/actions?query=workflow%3AmacOS)
[![Linux Build Status](https://github.com/szilardszaloki/cartesian_product_view/actions/workflows/linux.yml/badge.svg)](https://github.com/szilardszaloki/cartesian_product_view/actions?query=workflow%3ALinux)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/szilardszaloki/cartesian_product_view/blob/main/LICENSE)

First of all, check out [range-v3](https://github.com/ericniebler/range-v3), if you haven't already. It's an awesome library that was the basis of a proposal to add [ranges](https://en.cppreference.com/w/cpp/ranges) as an extension of the Standard Template Library with which you can write algorithms in a pure functional, declarative style.  
range-v3 already has a [`cartesian_product_view<>` implementation](https://github.com/ericniebler/range-v3/blob/master/include/range/v3/view/cartesian_product.hpp). While it does differentiate between shallow-`const` and deep-`const` views and propagate `const` to a certain extent, e.g.:
```cpp
auto sv = ranges::views::single(42); // ranges::single_view<> is deep-const
{
    auto cpv = ranges::views::cartesian_product(sv);
    static_assert(std::same_as<ranges::range_reference_t<decltype(cpv)>, ranges::common_tuple<int&>>);
}
{
    auto const cpv = ranges::views::cartesian_product(sv);
    static_assert(std::same_as<ranges::range_reference_t<decltype(cpv)>, ranges::common_tuple<int const&>>);
}
```
it also lets you write quite counter-intuitive code to say the least:
```cpp
int ints[]{ 1, 2, 3 };
{
    auto cpv = ranges::views::cartesian_product(ints);
    auto [first] = *ranges::cbegin(cpv);
    first = -1;
    assert(ints[0] == -1);
}
{
    auto const cpv = ranges::views::cartesian_product(ints);
    auto [first] = *ranges::begin(cpv);
    first = -2;
    assert(ints[0] == -2);
}
```
This is what my implementation is intended to protect from.  
  
As you iterate over a `cartesian_product_view<>`, it generates its elements on demand. These elements are tuples of the (`const`-qualified) reference types of the underlying ranges. The tuples themselves don't exist in memory, only the elements they refer to.  
  
**A note on proxy iterators:** `std::tuple<>` lacks some of the conversions that are necessary for a proxy reference type. This is actually one of the reasons range-v3 introduced [`ranges::common_tuple<>`](https://github.com/ericniebler/range-v3/blob/master/include/range/v3/utility/common_tuple.hpp). There's a good chance that I'll add an extracted C++20-only version of `ranges::common_tuple<>` shortly, but if you find your compiler complaining about `cartesian_product_view<>` not being able to satisfy e.g. `std::ranges::input_range` (because of `std::indirectly_readable`), consider using `ranges::common_tuple<>` in `cartesian_iterator<>`'s [`reference` type](https://github.com/szilardszaloki/cartesian_product_view/blob/002f36299b0d1c372b6c623d714a2257e4ebea63/cartesian_product_view.h#L218) in the meantime.  
  
Currently, the ceiling for the traversal category that `cartesian_product_view<>` can pass through is bidirectional. Random access support might be added later.

Known to work on the following compilers:
- GNU C++ 10.1.0 (tested on *Ubuntu 18.04.5 LTS* and *Ubuntu 20.04.2 LTS*)
- GNU C++ 10.2.0 (tested on *macOS 10.15.7 (19H512)*)
- MSVC 19.28.29337.0 (tested on *Windows 10 Version 1809 (OS Build 17763.1757)*)

but any C++20-conformant compiler should suffice.