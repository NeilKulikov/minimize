#ifndef OPERATIONS
#define OPERATIONS

#include <utility>
#include <exception>
#include <algorithm>
#include <functional>

#include <ranges.hpp>

namespace minimize{
    namespace ranges{
        namespace ops{
            template<typename T>
            using bop = const typename std::function<T(T, T)>&;
            template<typename T1, typename T2, typename R>
            using rbop = const typename std::function<R(T1, T2)>&;

            template<typename T1, typename T2>
            constexpr auto plus = [](T1 a, T2 b) -> decltype(a + b) { return a + b; };
            template<typename T1, typename T2>
            auto sum(const T1& c1, const T2& c2){
                using ty1 = typename T1::value_type;
                using ty2 = typename T2::value_type;
                using bop = decltype(plus<ty1, ty2>);
                return bop_range<T1, T2, bop>(plus<ty1, ty2>, c1, c2);
            };

            template<typename T1, typename T2>
            constexpr auto minus = [](T1 a, T2 b) -> decltype(a - b) { return a - b; };
            template<typename T1, typename T2>
            auto sub(const T1& c1, const T2& c2){
                using ty1 = typename T1::value_type;
                using ty2 = typename T2::value_type;
                using bop = decltype(minus<ty1, ty2>);
                return bop_range<T1, T2, bop>(minus<ty1, ty2>, c1, c2);
            };

            template<typename T1, typename T2>
            constexpr auto multiply = [](T1 a, T2 b) -> decltype(a * b) { return a * b; };
            template<typename T1, typename T2>
            auto mul(const T1& c1, const T2& c2){
                using ty1 = typename T1::value_type;
                using ty2 = typename T2::value_type;
                using bop = decltype(multiply<ty1, ty2>);
                return bop_range<T1, T2, bop>(multiply<ty1, ty2>, c1, c2);
            };
        };
        namespace ops{
            template<typename Range1, typename Range2>
            auto operator*(const Range1& r1, const Range2& r2){
                return mul(r1, r2);
            };

            template<typename Range1, typename Range2>
            auto operator+(const Range1& r1, const Range2& r2){
                return sum(r1, r2);
            };

            template<typename Range1, typename Range2>
            auto operator-(const Range1& r1, const Range2& r2){
                return sub(r1, r2);
            };
        };
        namespace ops{
            template<typename T, typename Range>
            auto scalar_sum(const T& v, const Range& r){
                const auto sr = ranges::scalar_range<T>(r.size(), v);
                return sum(sr, r);
            };

            template<typename T, typename Range>
            auto scalar_mul(const T& v, const Range& r){
                const auto sr = ranges::scalar_range<T>(r.size(), v);
                return mul(sr, r);
            };
        };
    };
};


#endif
