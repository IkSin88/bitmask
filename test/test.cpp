#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "enums.h"

#include <cstdint>
#include <unordered_map>
#include <map>

TEST_CASE( "zero_bitmask", "[]" )
{
    using intrusive::syntax_option_type;

    boost::bitmask<syntax_option_type> x;

    int i = x;

    CHECK(i == 0);
    CHECK(!x);
    CHECK_FALSE(x);

    CHECK(!(x & syntax_option_type::icase));
    CHECK((x == 0));
    CHECK((0 == x));
    CHECK((x & syntax_option_type::icase) == 0);
    CHECK(0 == (x & syntax_option_type::icase));
    CHECK(x.bits() == 0);
    CHECK(0 == x.bits());

    CHECK(((x & syntax_option_type::ECMAScript) == syntax_option_type::ECMAScript));
    CHECK((x == syntax_option_type::ECMAScript));
    CHECK((syntax_option_type::ECMAScript == x));

    boost::bitmask<syntax_option_type> y = 0;

    CHECK(!y);
    CHECK_FALSE(y);
    CHECK(y.bits() == 0);

    using nonintrusive::open_mode;

    boost::bitmask<open_mode> z;

    CHECK(!z);
    CHECK_FALSE(z);
    CHECK(0 == z.bits());

    CHECK(!(z & open_mode::app));
    CHECK((z & open_mode::app) == 0);
    CHECK(z.bits() == 0);

    boost::bitmask<open_mode> z2 = nullptr;
    CHECK(!z2);
    CHECK_FALSE(z2);
    CHECK(0 == z2.bits());
}

TEST_CASE( "bitmask_basics", "[]" )
{
    using intrusive::syntax_option_type;

    boost::bitmask<syntax_option_type> x{syntax_option_type::awk | syntax_option_type::icase};

    CHECK(x.bits() == (static_cast<int>(syntax_option_type::awk) | static_cast<int>(syntax_option_type::icase)));

    int i = x;

    CHECK(i == (static_cast<int>(syntax_option_type::awk) | static_cast<int>(syntax_option_type::icase)));
    CHECK(i == x.bits());
    CHECK(i == x);
    CHECK(x == i);

    CHECK(x);

    CHECK((x & syntax_option_type::icase));
    CHECK((x & syntax_option_type::icase) != 0);
    CHECK(0 != (x & syntax_option_type::icase));
    CHECK((x & syntax_option_type::icase) == syntax_option_type::icase);
    CHECK(syntax_option_type::icase == (x & syntax_option_type::icase));
    CHECK((x & syntax_option_type::icase) != syntax_option_type::awk);
    CHECK(syntax_option_type::awk != (x & syntax_option_type::icase));

    CHECK((x & syntax_option_type::awk));
    CHECK((x & syntax_option_type::awk) != 0);
    CHECK(0 != (x & syntax_option_type::awk));

    CHECK(!(x & syntax_option_type::nosubs));
    CHECK((x & syntax_option_type::nosubs) == 0);
    CHECK(0 == (x & syntax_option_type::nosubs));

    CHECK(x == (syntax_option_type::awk | syntax_option_type::icase));
    CHECK((syntax_option_type::awk | syntax_option_type::icase) == x);
    CHECK((syntax_option_type::awk | syntax_option_type::basic) != x);

    CHECK((x | syntax_option_type::basic) != x);
    CHECK((x | syntax_option_type::icase) == x);
    CHECK((x | syntax_option_type::basic) == (syntax_option_type::basic | x));
    CHECK(((syntax_option_type::icase ^ x) == syntax_option_type::awk));
    CHECK(((x ^ syntax_option_type::icase) == syntax_option_type::awk));
    CHECK((syntax_option_type::awk == (x ^ syntax_option_type::icase)));
    CHECK((syntax_option_type::awk == (syntax_option_type::icase ^ x)));

    auto y = syntax_option_type::awk | syntax_option_type::icase | syntax_option_type::collate;

    CHECK(y);
    CHECK(y.bits());

    CHECK_FALSE((y ^ y));
    CHECK((y ^ y) == 0);
    CHECK((y ^ y) != 1);
    CHECK(1 != (y ^ y));

    CHECK_FALSE((syntax_option_type::icase ^ syntax_option_type::icase));
    CHECK((syntax_option_type::icase ^ syntax_option_type::icase) == 0);
    CHECK(0 == (syntax_option_type::icase ^ syntax_option_type::icase));

    CHECK((syntax_option_type::icase ^ syntax_option_type::basic) == (syntax_option_type::basic ^ syntax_option_type::icase));
    CHECK((syntax_option_type::basic == (syntax_option_type::icase ^ syntax_option_type::basic ^ syntax_option_type::icase)));

    CHECK((y & syntax_option_type::icase));
    CHECK((y & syntax_option_type::icase) != 0);

    CHECK((y & syntax_option_type::awk));
    CHECK(((y & syntax_option_type::awk) != 0));

    CHECK((y & syntax_option_type::collate));
    CHECK(((y & syntax_option_type::collate) != 0));

    CHECK(!(y & syntax_option_type::nosubs));
    CHECK((y & syntax_option_type::nosubs) == 0);

    const auto y_copy = y;

    CHECK(y_copy == y);
    CHECK(y == y_copy);
}

namespace {
    enum class short_enum: short
    {
        v1 = 1,
        v2 = 2,

        _bitmask_max_element = v2
    };

    BOOST_BITMASK(short_enum)

    enum class long_enum: long
    {
        v1 = 1,
        v2 = 2,

        _bitmask_max_element = v2
    };

    BOOST_BITMASK(long_enum)

    enum class longest_enum: uint64_t
    {
        v1 = 1,
        v2 = 0x8000000000000000,

        _bitmask_max_element = v2
    };
        
    BOOST_BITMASK(longest_enum)
}

TEST_CASE( "bitmask_underlying_type", "[]" )
{
    static_assert(sizeof(boost::bitmask<short_enum>::underlying_type) == sizeof(short),
                  "sizes are equal");
    static_assert(sizeof(boost::bitmask<short_enum>::underlying_type) == sizeof(short),
                  "sizes are equal");
    static_assert(sizeof(boost::bitmask<short_enum>::underlying_type) == sizeof(decltype(boost::bitmask<short_enum>{}.bits())),
                  "sizes are equal");
    static_assert(sizeof(boost::bitmask<long_enum>::underlying_type) == sizeof(decltype(boost::bitmask<long_enum>{}.bits())),
                  "sizes are equal");

    boost::bitmask<longest_enum> x = longest_enum::v1 | longest_enum::v2;
    CHECK(x.bits() == 0x8000000000000001);
    CHECK((longest_enum::v1 | longest_enum::v2).bits() == 0x8000000000000001);
}

TEST_CASE( "bitmask_order_hash", "[]" )
{
    using intrusive::syntax_option_type;

    boost::bitmask<syntax_option_type> x1{syntax_option_type::collate ^ syntax_option_type::awk};
    boost::bitmask<syntax_option_type> x2{syntax_option_type::collate};
    boost::bitmask<syntax_option_type> x3 = x1;

    using hash = std::hash<boost::bitmask<syntax_option_type>>;

    CHECK(boost::bitmask<syntax_option_type>{} < x1);
    CHECK(x1 != x2);
    CHECK(x1 == x1);
    CHECK(x1 > x2);
    CHECK(x2 < x1);
    CHECK_FALSE(x1 < x1);
    CHECK_FALSE(x1 > x1);
    CHECK(hash{}(x1) == hash{}(x1));
    CHECK(hash{}(x1) != hash{}(x2));

    CHECK(x1 == x3);
    CHECK_FALSE(x1 < x3);
    CHECK_FALSE(x1 > x3);
    CHECK(hash{}(x1) == hash{}(x3));

    std::map<boost::bitmask<syntax_option_type>, int> map;
    map.emplace(syntax_option_type::collate, 2);
    map.emplace(boost::bitmask<syntax_option_type>{}, 3);
    CHECK(map.begin()->second == 3);
    CHECK((--map.end())->second == 2);
    CHECK(map[syntax_option_type::collate] == 2);
    CHECK(map[nullptr] == 3);

    std::unordered_map<boost::bitmask<syntax_option_type>, int> umap;
    map.emplace(nullptr, 2);
    map.emplace(syntax_option_type::collate, 3);
}

TEST_CASE( "bitmask_assign_op", "[]" )
{
    using intrusive::open_mode;
    
    boost::bitmask<open_mode> x = open_mode::app;
    CHECK((x & open_mode::app));
    CHECK(!(x & open_mode::ate));

    x |= open_mode::ate;

    CHECK((x & open_mode::app));
    CHECK((x & open_mode::ate));
    CHECK(!(x & open_mode::binary));

    x &= open_mode::binary;

    CHECK(!(x & open_mode::app));
    CHECK(!(x & open_mode::ate));
    CHECK(!(x & open_mode::binary));

    x = open_mode::ate;
    x |= open_mode::app;

    CHECK((x & open_mode::app));
    CHECK((x & open_mode::ate));

    x &= open_mode::ate;

    CHECK(!(x & open_mode::app));
    CHECK((x & open_mode::ate));

    x |= open_mode::app;

    x &= open_mode::app | open_mode::ate;

    CHECK((x & open_mode::app));
    CHECK((x & open_mode::ate));

    x &= open_mode::app | open_mode::binary;

    CHECK((x & open_mode::app));
    CHECK(!(x & open_mode::ate));
    CHECK(!(x & open_mode::binary));

    x ^= open_mode::app;

    CHECK(!(x & open_mode::app));
    CHECK(!(x & open_mode::ate));
    CHECK(!(x & open_mode::binary));

    x = open_mode::ate | open_mode::app;

    CHECK((x & open_mode::app));
    CHECK((x & open_mode::ate));

    x ^= open_mode::app | open_mode::binary;

    CHECK(!(x & open_mode::app));
    CHECK((x & open_mode::ate));
    CHECK((x & open_mode::binary));
}


TEST_CASE( "bitmask_intrusive_value_mask", "[]" )
{}

TEST_CASE( "bitmask_intrusive_max_element", "[]" )
{}

TEST_CASE( "bitmask_nonintrusive_value_mask", "[]" )
{}

TEST_CASE( "bitmask_nonintrusive_max_element", "[]" )
{}

TEST_CASE("bitmask_ones_complement", "[]")
{
}
