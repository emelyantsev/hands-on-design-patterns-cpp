#include <iostream>
#include <memory>
#include <typeinfo>
#include <cxxabi.h>

template <typename T> auto type(T&& p) {

    int status;
    char* name = abi::__cxa_demangle(typeid(p).name(), 0, 0, &status);
    std::string s(name);
    ::free(name);
    return s;
}

class Base {
    
    public:
    virtual ~Base() {}
    virtual std::unique_ptr<Base> clone() const = 0;
};

class Derived : public Base {
    public:
    std::unique_ptr<Base> clone() const override { return std::unique_ptr<Base>(new Derived(*this)); }
};

int main() {

    std::unique_ptr<Base> b0( new Derived );
    std::unique_ptr<Base> b1 = b0->clone();
    
    std::cout << type( *b0 ) << std::endl;
    std::cout << type( *b1 ) << std::endl;
}