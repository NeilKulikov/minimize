#include <array>
#include <iostream>

auto&& func(const int x){
    std::array rv{-x, x};
    return std::forward< std::array<int, 2> >(rv);
};

int main(void){
    std::array<int, 2> rv(func(10));
    std::cout << rv[0] << ' ' << rv[1] << std::endl;
    return 0;
};