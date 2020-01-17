#ifndef DERIVATE
#define DERIVATE

#include <array>
#include <numeric>
#include <utility>
#include <exception>
#include <algorithm>
#include <functional>

#include <iostream>

#include <ranges.hpp>
#include <operations.hpp>

namespace minimize{
    namespace derivate{
        using rv = double;
        namespace constants{
            template<std::size_t pn>
            struct derivative_props{
                const std::array<rv, pn> coeffs;
                const rv multiplier;
                static auto shifts(const rv h){
                    static_assert(pn % 2 == 1, "p_num should be even");
                    constexpr int pnd = static_cast<int>(pn / 2);
                    std::array<rv, pn> ret_val;
                    for(std::size_t it = 0; it < pn; it++){
                        const int sn = static_cast<int>(it) - pnd;
                        ret_val.at(it) = h * static_cast<rv>(sn);
                    };
                    return shifts;
                };
            };

            template<>
            auto derivative_props<3>::shifts(const rv h){
                std::array<rv, 3> ret_val{-h, 0., h};
                return ret_val;
            };
            template<>
            auto derivative_props<5>::shifts(const rv h){
                std::array<rv, 5> ret_val{-2. * h, -h, 0., h, 2. * h};
                return ret_val;
            };
            constexpr derivative_props<5> five{{1., -8., 0., 8., -1}, 12.};
            constexpr derivative_props<3> three{{-1., 1.}, 2.};
        };

        template<typename Range>
        ranges::subs_range<typename Range::container> shifted_x(const Range& r, 
                const std::size_t d, const rv h){
            return ranges::subs_range<typename Range::container>(r, {d, r.at(d) + h});
        };

        template<typename Func, typename Range, std::size_t p_num>
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

        template<typename T, typename R>
        double compute_derivation(const T& vals, const R& dpr, const rv h){
            rv ret_val = std::inner_product(vals.cbegin(), vals.cend(), 
                dpr.coeffs.cbegin(), 0.);
            return ret_val / (dpr.multiplier * h);
        };

        template<typename Func, typename Range, std::size_t p_num>
        double derive_by_axis(const Func& func, const Range& r, const std::size_t d, 
                const rv h, const constants::derivative_props<p_num>& p){
            using arr = std::array<rv, p_num>;
            const arr shifts(p.shifts(h));
            const arr vals(std::move(
                values_by_axis<Func, Range, p_num>(shifts, func, r, d)));
            return compute_derivation(vals, constants::five, h);
        };


        template<typename Func, typename Range>
        double derive_by_axis(const Func& func,
                const Range& r, const std::size_t d, const rv h = 1.e-8){
            return derive_by_axis(func, r, d, h, constants::five);
        };  

        template<typename Func, typename Range>
        double derive_by_axis_3(const Func& func,
                const Range& r, const std::size_t d, const rv h = 1.e-8){
            return derive_by_axis(func, r, d, h, constants::three);
        };   

        template<typename Func, typename Range>
        std::vector<rv> auto_grad(const Func& func, const Range& r, 
                const rv h = 1.e-8){
            std::vector<rv> ret_val(r.size());
            for(std::size_t i = 0; i < ret_val.size(); i++){
                ret_val.at(i) = derive_by_axis(func, r, i, h);
            };
            return ret_val;
        };

        template<typename Range1, typename Range2>
        auto shifted_by_direction(const Range1& r, const Range2& d, const rv& h){
            const auto dr = ranges::ops::scalar_mul(h, d);
            return ranges::ops::sum(r, dr);
        };

        template<typename Func, typename Range1, typename Range2, std::size_t p_num>
        std::array<rv, p_num> values_by_direction(
                const std::array<rv, p_num> shifts,
                const Func& func, 
                const Range1& r, 
                const Range2& d){
            std::array<rv, p_num> ret_val;
            std::transform(shifts.cbegin(), shifts.cend(), ret_val.begin(),
                [&](const double sh){ 
                    const auto nx = shifted_by_direction(r, d, sh);
                    return func(nx);
                });
            return ret_val;
        };

        template<typename Func, typename Range1, typename Range2, std::size_t p_num>
        double derive_by_directiona(const Func& func,
                const Range1& r, const Range2& d, const rv& h, const constants::derivative_props<p_num>& p){
            using arr = std::array<rv, p_num>;
            const arr shifts(p.shifts(h));
            const arr vals(std::move(
                values_by_direction<Func, Range1, Range2, p_num>(shifts, func, r, d)));
            return compute_derivation(vals, p, h);
        };

        template<typename Func, typename Range1, typename Range2>
        double derive_by_direction(const Func& func,
                const Range1& r, const Range2& d, const rv& h = 1.e-8){
            return derive_by_directiona<Func, Range1, Range2, 5>(func, r, d, h, constants::five);
        };

        template<typename Func, typename Range1, typename Range2>
        double derive_by_direction_3(const Func& func,
                const Range1& r, const Range2& d, const rv& h = 1.e-8){
            return derive_by_directiona<Func, Range1, Range2, 3>(func, r, d, h, constants::three);
        };
    };
};

#endif