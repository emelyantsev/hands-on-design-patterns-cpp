#include <iostream>

template <size_t N> void f(char(*)[N % 2] = NULL) { std::cout << "N=" << N << " is odd" << std::endl; }
template <size_t N> void f(char(*)[1 - N % 2] = NULL) { std::cout << "N=" << N << " is even" << std::endl; }

int main() {
    f<5>();
    f<8>();


   // char arr_even[10];

  //  f<10>(arr_even);
}