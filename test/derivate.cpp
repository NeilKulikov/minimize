#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Derivate
#include <boost/test/unit_test.hpp>

#include <vector>

#include <derivate.hpp>

BOOST_AUTO_TEST_SUITE(DerivateTests)

struct functor{
    double x0;
    template<typename Range>
    double operator()(const Range& r) const{
        return r.at(0) * (r.at(0) - x0);
    };
};

BOOST_AUTO_TEST_CASE(Derivate1D)
{
    const functor func{2.};
    std::vector<double> x0{0.}, x1{1.}, x2{10.};
    auto    xr0 = minimize::ranges::const_range(x0),
            xr1 = minimize::ranges::const_range(x1),
            xr2 = minimize::ranges::const_range(x2);
    auto    der0 = minimize::derivate::derive_by_axis(func, xr0, 0),
            der1 = minimize::derivate::derive_by_axis(func, xr1, 0),
            der2 = minimize::derivate::derive_by_axis(func, xr2, 0);
    BOOST_CHECK_CLOSE(der0, -2., 1.e-4);
    BOOST_CHECK_LE(std::abs(der1), 1.e-4);
    BOOST_CHECK_CLOSE(der2, 18., 1.e-4);
}

BOOST_AUTO_TEST_SUITE_END()