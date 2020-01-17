#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Ranges
#include <boost/test/unit_test.hpp>

#include <vector>

#include <ranges.hpp>

BOOST_AUTO_TEST_SUITE(RangesTests)

/*BOOST_AUTO_TEST_CASE(StorageVector)
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
}*/

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
    std::size_t idx = 178;
    int val = 123456;
    minimize::ranges::subs_range srange(data, {idx, val});
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

BOOST_AUTO_TEST_CASE(ScalarRange)
{
    const std::size_t len = 512;
    const int body = 12345;
    minimize::ranges::scalar_range sr(len, body);
    std::cout << "Scalar Range size " << sr.size() << std::endl;
    for(std::size_t i = 0; i < len; i++)
        BOOST_CHECK_EQUAL(sr.at(i), body);
}

BOOST_AUTO_TEST_SUITE_END()