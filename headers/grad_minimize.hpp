#ifndef GRAD_MINIMIZE
#define GRAD_MINIMIZE

#define FPHI 0.618033988749894848204586834365638117720309179805762862135
#define SPHI 0.381966011250105151795413165634361882279690820194237137864

#include <vector>

#include <cmath>
#include <numeric>
#include <utility>
#include <exception>
#include <algorithm>
#include <functional>

#include <ranges.hpp>
#include <operations.hpp>
#include <derivate.hpp>

namespace minimize{
    namespace grad{
        using rv = double;
        using vec = std::vector<double>;

        template<typename Func>
        std::pair<bool, rv> golden_ratio_minimize(
                const Func& f, 
                const std::pair<rv, rv>& bounds, 
                const rv& tol,
                const std::size_t& max_steps){
            rv a = std::min(bounds.first, bounds.second), 
               b = std::max(bounds.first, bounds.second);
            rv h = b - a;
            rv c, d, vc, vd;
            {
                c = a + h * SPHI, d = a + h * FPHI;
                vc = f(c), vd = f(d);
            };
            for(std::size_t step = 0; (step < max_steps) && (tol < h); step++){
                if(vc < vd){
                    b = d; d = c;
                    vd = vc;
                    h = h * FPHI;
                    c = a + h * SPHI;
                    vc = f(c);
                }else{
                    a = c; c = d;
                    vc = vd;
                    h = h * FPHI;
                    d = a + h * FPHI;
                    vd = f(d);
                };
            };
            if(vc < vd){
                return {(tol > h), 0.5 * (a + d)};
            }else{
                return {(tol > h), 0.5 * (b + c)};
            };
            return {false, 0};
        };
        template<typename Func>
        rv auto_golden_ratio_minimize(
                const Func& f, 
                const std::pair<rv, rv> bounds, 
                const rv tol,
                const std::size_t max_steps){
            const auto h = bounds.second
        };
    };
};

#endif