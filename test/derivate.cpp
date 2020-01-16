#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Derivate
#include <boost/test/unit_test.hpp>

#include <cmath>
#include <iostream>
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

struct functor_nd{
    template<typename Range>
    double operator()(const Range& r) const{
        return std::pow(r.at(0), 2.)  + (std::pow(r.at(1), 4.) - std::pow(r.at(1), 2.));
    };
};

BOOST_AUTO_TEST_CASE(Shifted2D)
{
    std::vector<double> x0{1., 1.};
    auto xr0 = minimize::ranges::const_range(x0);
    auto sr0 = minimize::derivate::shifted_x(xr0, 1, 277.);
    BOOST_CHECK_CLOSE(sr0.at(0), 1., 1.e-4);
    BOOST_CHECK_CLOSE(sr0.at(1), 278., 1.e-4);
}

BOOST_AUTO_TEST_CASE(DerivateByAxisND)
{
    const functor_nd func;
    std::vector<double> x0{1., 2.};
    auto    xr0 = minimize::ranges::const_range(x0);
    auto    der0 = minimize::derivate::derive_by_axis(func, xr0, 0),
            der1 = minimize::derivate::derive_by_axis(func, xr0, 1);
    BOOST_CHECK_CLOSE(der0, 2., 1.e-4);
    BOOST_CHECK_CLOSE(der1, 28., 1.e-4);
}

BOOST_AUTO_TEST_CASE(Derivate2D)
{
    const functor_nd func;
    std::vector<double> x0{1., 1.}, x1{3., 2.};
    auto    xr0 = minimize::ranges::const_range(x0),
            xr1 = minimize::ranges::const_range(x1);
    auto    der0 = minimize::derivate::auto_grad(func, xr0),
            der1 = minimize::derivate::auto_grad(func, xr1);
    BOOST_CHECK_CLOSE(der0.at(0), 2., 1.e-4);
    BOOST_CHECK_CLOSE(der0.at(1), 2., 1.e-4);
    BOOST_CHECK_CLOSE(der1.at(0), 6., 1.e-4);
    BOOST_CHECK_CLOSE(der1.at(1), 28., 1.e-4);
}

BOOST_AUTO_TEST_SUITE_END()