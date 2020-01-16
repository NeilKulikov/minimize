#include <array>
#include <iostream>

int main(void){
    std::array a{0., 2., 3.};
    auto&& b = std::forward<std::array<double, 3> >(a);
    std::array c(b);
    for(std::size_t i = 0; i < 3; i++){
        std::cout << c[i] << ' ' << a[i] << std::endl;
    }
    return 0;
};