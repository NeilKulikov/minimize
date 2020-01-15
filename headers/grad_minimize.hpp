#ifndef GRAD_MINIMIZE
#define GRAD_MINIMIZE

#include <vector>

namespace minimize{
    namespace grad{
        using rv = double;
        using vec = std::vector<double>;
        class minimizer{
            struct result{
                enum flag{
                    SUCCESS,
                    FAILURE
                };
                const flag status = FAILURE;
                const vec x;
                const rv value;
                const vec grad;
            };
        };
    };
};

#endif