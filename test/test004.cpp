#include <numeric>
#include <algorithm>
#include <functional>
#include <type_traits>

int main(void){
    using res = std::invoke_result_t<std::plus<int>(), int, int>;
    return 0;
};