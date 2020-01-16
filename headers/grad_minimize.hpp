#ifndef GRAD_MINIMIZE
#define GRAD_MINIMIZE

#include <vector>

namespace minimize{
    namespace grad{
        using rv = double;
        using vec = std::vector<double>;
        struct minimizer_1d{

        };
        struct minimizer{
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
            //struct props{
            //    const rv step
            //};

        };
    };
};

#endif