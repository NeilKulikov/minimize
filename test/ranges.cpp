#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Ranges
#include <boost/test/unit_test.hpp>

#include <vector>

#include <ranges.hpp>

BOOST_AUTO_TEST_SUITE(RangesTests)

BOOST_AUTO_TEST_CASE(StorageVector)
{
    const std::size_t len = 512;
    std::vector<int> data(len);
    for(std::size_t i = 0; i < len; i++) 
        data[i] = 12 + i;
    minimize::ranges::range range(data);
    BOOST_CHECK_EQUAL(data.size(), range.size());
    for(std::size_t i = 0; i < len; i++)
        BOOST_CHECK_EQUAL(range.at(i), data.at(i));
    for(std::size_t i = 0; i < len; i++){
        const auto it = range.iterator_at(i);
        BOOST_CHECK_EQUAL(it.num(), i);
    };
    for(std::size_t i = 0; i < len; i++){
        auto it = range.iterator_at(i);
        BOOST_CHECK_EQUAL(*it, data.at(i));
    };
}

BOOST_AUTO_TEST_CASE(ConstStorageVector)
{
    const std::size_t len = 512;
    std::vector<int> data(len);
    for(std::size_t i = 0; i < len; i++) 
        data[i] = 12 + i;
    minimize::ranges::const_range range(data);
    BOOST_CHECK_EQUAL(data.size(), range.size());
    for(std::size_t i = 0; i < len; i++)
        BOOST_CHECK_EQUAL(range.at(i), data.at(i));
    for(std::size_t i = 0; i < len; i++){
        const auto it = range.iterator_at(i);
        BOOST_CHECK_EQUAL(it.num(), i);
    };
    for(std::size_t i = 0; i < len; i++){
        auto it = range.iterator_at(i);
        BOOST_CHECK_EQUAL(*it, data.at(i));
    };
}

BOOST_AUTO_TEST_CASE(ConstRangeSubs)
{
    const std::size_t len = 512;
    std::vector<int> data(len);
    for(std::size_t i = 0; i < len; i++) 
        data[i] = 12 + i;
    minimize::ranges::const_range r(data);
    std::size_t idx = 178;
    int val = 123456;
    minimize::ranges::subs_range srange(r, {idx, val});
    BOOST_CHECK_EQUAL(data.size(), srange.size());
    for(std::size_t i = 0; i < len; i++){
        if(i != idx)
            BOOST_CHECK_EQUAL(srange.at(i), data.at(i));
        else
            BOOST_CHECK_EQUAL(srange.at(i), val);
    };
    for(std::size_t i = 0; i < len; i++){
        const auto it = srange.iterator_at(i);
        BOOST_CHECK_EQUAL(it.num(), i);
    };
    for(std::size_t i = 0; i < len; i++){
        auto it = srange.iterator_at(i);
        if(i != idx)
            BOOST_CHECK_EQUAL(*it, data.at(i));
        else
            BOOST_CHECK_EQUAL(*it, val);
    };
}

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

BOOST_AUTO_TEST_SUITE_END()