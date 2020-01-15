#ifndef DERIVATE
#define DERIVATE

#include <array>
#include <numeric>
#include <utility>
#include <exception>
#include <algorithm>
#include <functional>

#include <ranges.hpp>

namespace minimize{
    namespace derivate{
        using rv = double;
        namespace constants{
            template<std::size_t dim>
            struct derivative_props{
                const std::array<rv, dim> coeffs;
                const rv multiplier;
            };
            constexpr derivative_props<5> five{{1., -8., 0., 8., -1}, 12.};
            constexpr derivative_props<3> three{{-1., 1.}, 2.};
        };

        template<typename Range>
        ranges::subs_range<typename Range::co> shifted_x(const Range& r, 
                const std::size_t d, const rv h){
            return ranges::subs_range<typename Range::co>(r, {d, r.at(d) + h});
        };

        template<std::size_t p_num>
        std::array<rv, p_num> shifts(const rv h){
            static_assert(p_num % 2 == 1, "p_num should be even");
            constexpr int pnd = static_cast<int>(p_num / 2);
            std::array<rv, p_num> ret_val;
            for(std::size_t it = 0; it < p_num; it++){
                const int sn = static_cast<int>(it) - pnd;
                ret_val.at(it) = h * static_cast<rv>(sn);
            };
            return shifts;
        };

        template<typename Func, typename Range, std::size_t p_num = 5>
        std::array<rv, p_num> values_by_axis(
                const std::array<rv, p_num> shifts,
                const Func& func, 
                const Range& r, 
                const std::size_t d){
            std::array<rv, p_num> ret_val;
            std::transform(shifts.cbegin(), shifts.cend(), ret_val.begin(),
                [&](const double sh){ 
                    const auto nx = shifted_x(r, d, sh);
                    return func(nx);
                });
            return ret_val;
        };

        template<typename Func, typename Range>
        double derive_by_axis(const Func& func,
                const Range& r, const std::size_t d, const rv h = 1.e-8){
            const std::size_t p_num = 5;
            const std::array<rv, p_num> shifts{-2. * h, -h, 0., h, 2. * h};
            const auto vals = 
                    values_by_axis<Func, Range, p_num>(shifts, func, r, d);
            rv ret_value = std::inner_product(vals.cbegin(), vals.cend(), 
                constants::five.coeffs.cbegin(), 0.);
            return ret_value / (12. * h);
        }   

        template<typename Func, typename Range>
        double derive_by_axis_3(const Func& func,
                const Range& r, const std::size_t d, const rv h = 1.e-8){
            const std::size_t p_num = 3;
            const std::array<rv, p_num> shifts{-h, 0., h};
            const auto vals = 
                    values_by_axis<Func, Range, p_num>(shifts, func, r, d);
            rv ret_value = std::inner_product(vals.cbegin(), vals.cend(), 
                constants::five.coeffs.cbegin(), 0.);
            return ret_value / (2. * h);
        }   
    };
};

#endif