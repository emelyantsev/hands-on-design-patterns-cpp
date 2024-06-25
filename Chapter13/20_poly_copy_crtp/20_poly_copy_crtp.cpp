#include <iostream>
#include <memory>
#include <typeinfo>
#include <cxxabi.h>

template <typename T> auto type(T&& p) {
    
    int status;
    char* name = abi::__cxa_demangle( typeid(p).name(), 0, 0, &status);
    std::string s(name);
    ::free(name);
    return s;
}


class Base {
    public:
    virtual ~Base() {}
    virtual Base* clone() const = 0;
};

template <typename Derived> class Cloner : public Base {
    public:
    Base* clone() const override {
        return new Derived( *static_cast<const Derived*>( this ) );
    }
};

class Derived : public Cloner<Derived> {
    public:
};

class Derived2 : public Cloner<Derived2> {
    public:
};


int main() {

    Base* b0 = new Derived;
    Base* b1 = b0->clone();
    
    std::cout << type( *b0 ) << std::endl;
    std::cout << type( *b1 ) << std::endl;

    Base* b2 = new Derived2;
    Base* b3 = b2->clone();
    
    std::cout << type( *b2 ) << std::endl;
    std::cout << type( *b3 ) << std::endl;
}
