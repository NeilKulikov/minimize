#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Ranges
#include <boost/test/unit_test.hpp>

#include <cmath>
#include <vector>

#include <minimize_1d.hpp>

BOOST_AUTO_TEST_SUITE(RangesTests)

struct func_1d{
    const double d;
    double min_val(void) const{
        return - 0.25 * d * d; 
    };
    double min_x(void) const{
        return std::sqrt(d) / std::sqrt(2);
    };
    double operator()(const double x) const{
        return x * x * (x * x - d);
    };
};

BOOST_AUTO_TEST_CASE(GoldenRation)
{
    const double d = 5.;
    const func_1d f{d};
    const std::pair bounds{0., 100.};
    const auto mr = minimize::D1::golden_ratio_minimize(f, bounds, 1.e-8, 1024);
    std::cout << mr.first << ' ' << mr.second << std::endl;
    BOOST_CHECK(mr.first);
    BOOST_CHECK_CLOSE(mr.second, f.min_x(), 1.e-4);
    BOOST_CHECK_CLOSE(f(mr.second), f.min_val(), 1.e-4);
}

BOOST_AUTO_TEST_CASE(AutoGoldenRation)
{
    const double d = 5.;
    const func_1d f{d};
    const std::pair bounds{0., 100.};
    const auto mr = minimize::D1::auto_golden_ratio_minimize(f, bounds);
    BOOST_CHECK_CLOSE(mr, f.min_x(), 1.e-4);
    BOOST_CHECK_CLOSE(f(mr), f.min_val(), 1.e-4);
}

BOOST_AUTO_TEST_SUITE_END()