#include <iostream>
#include <memory>
#include <typeinfo>
#include <cxxabi.h>

template <typename T> auto type(T&& p) {

    int status;
    char* name = abi::__cxa_demangle( typeid( p ).name(), 0, 0, &status );
    std::string s(name);
    ::free(name);
    
    return s;
}

template <typename Derived> class Base {
    public:

    ~Base() {}
    
    std::unique_ptr<Derived> clone() const {
    
        return std::unique_ptr<Derived>( new Derived( *static_cast<const Derived*>( this ) ) );
    }
};

class Derived : public Base<Derived> { } ;

int main() {

    std::unique_ptr<Derived> b0( new Derived );
    std::unique_ptr<Derived> b1 = b0->clone();
    
    std::cout << type( *b0 ) << std::endl;
    std::cout << type( *b1 ) << std::endl;
}