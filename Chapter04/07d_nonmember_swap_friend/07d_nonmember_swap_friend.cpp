#include <utility>
#include <iostream>

using std::swap;

namespace N {
class C {
    public:
    explicit C(int i) : i_(i) {}
    C(const C& rhs) : i_(rhs.i_) { std::cout << "C(const C&)" << std::endl; }
    C& operator=(const C& rhs) { i_ = rhs.i_; std::cout << "C = C&" << std::endl; return *this; }
    void swap(C& rhs) noexcept { std::cout << "C::swap(C&)" << std::endl; std::swap(i_, rhs.i_); }
    friend void swap(C& lhs, C& rhs) noexcept { std::cout << "friend C::swap" << std::endl; lhs.swap(rhs); }
    int i_;
};
}


int main() {
    N::C c1(1), c2(2);
    std::cout << "\nInitial:" << std::endl;
    std::cout << "c1: " << c1.i_ << " c2: " << c2.i_ << std::endl;

    std::swap(c1, c2);
    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "c1: " << c1.i_ << " c2: " << c2.i_ << std::endl;

    swap(c1, c2);
    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "c1: " << c1.i_ << " c2: " << c2.i_ << std::endl;
}