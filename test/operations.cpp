#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Operations
#include <boost/test/unit_test.hpp>

#include <vector>

#include <operations.hpp>

BOOST_AUTO_TEST_SUITE(OperationsTests)

BOOST_AUTO_TEST_CASE(ConstRangeSum)
{
    const std::size_t len = 5;
    using vec = std::vector<int>;
    vec data1(len), data2(len);
    for(int i = 0; i < len; i++){
        data1[i] = 12 + 2 * i;
        data2[i] = 138 - i;
    }
    const auto sr = minimize::ranges::ops::sum<vec, vec>(data1, data2);
    BOOST_CHECK_EQUAL(data1.size(), sr.size());
    for(int i = 0; i < len; i++){
        std::cout << i << ' ' << data1.at(i) << ' ' << data2.at(i) << ' ' << sr.at(i) << std::endl;
        BOOST_CHECK_EQUAL(sr.at(i), 12 + 138 + i);
    };
    std::cout << "Lambda size: " << sizeof(minimize::ranges::ops::plus<int, int>) << std::endl;
    std::cout << "Sum Range size: " << sizeof(sr) << std::endl;
    std::cout << "Sum Range Iterator size: " << sizeof(sr.begin()) << std::endl;
    const auto mr = minimize::ranges::ops::sub<vec, vec>(data1, data2);
    BOOST_CHECK_EQUAL(data1.size(), sr.size());
    for(int i = 0; i < len; i++){
        std::cout << i << ' ' << data1.at(i) << ' ' << data2.at(i) << ' ' << mr.at(i) << std::endl;
        BOOST_CHECK_EQUAL(mr.at(i), 12 - 138 + 3 * i);
    };
}

BOOST_AUTO_TEST_CASE(ScalarSum)
{
    const std::size_t len = 5;
    using vec = std::vector<int>;
    vec data(len);
    for(int i = 0; i < len; i++){
        data[i] = 12 + 2 * i;
    };
    const int val = 123456;
    const auto sr = minimize::ranges::ops::scalar_sum<int, vec>(val, data);
    for(int i = 0; i < len; i++){
        BOOST_CHECK_EQUAL(sr.at(i), val + data.at(i));
    };
    const int mul = 123;
    const auto mr = minimize::ranges::ops::scalar_mul<int, vec>(mul, data);
    for(int i = 0; i < len; i++){
        BOOST_CHECK_EQUAL(mr.at(i), mul * data.at(i));
    };
}
BOOST_AUTO_TEST_SUITE_END()