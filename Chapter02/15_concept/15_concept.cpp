// 02 with concept restriction on a class template method
#include <iostream>

template <typename T> class ArrayOf2 {
    public:
    T& operator[](size_t i) { return a_[i]; }
    const T& operator[](size_t i) const { return a_[i]; }
    T sum() const requires (requires (T a, T b) { a + b; }) { return a_[0] + a_[1]; }
    private:
    T a_[2];
};


int main() {
    
    ArrayOf2<int> i; i[0] = 1; i[1] = 5;
    std::cout << i.sum() << std::endl;
    ArrayOf2<double> x; x[0] = -3.5; x[1] = 4;
    std::cout << x.sum() << std::endl;
    ArrayOf2<char*> c; char s[] = "Hello";
    c[0] = s; c[1] = s + 2;
    //std::cout << c.sum() << std::endl; // Concept requirement not met

    /*
    int x1 = 8;
    int x2 = 9;
    int* p1 = &x1;
    int* p2 = &x2;

    int* p3 = p1 + 1;
    */
}