#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include <vector>

template <typename T> auto type(T&& p) {
    int status;
    char* name = abi::__cxa_demangle(typeid(p).name(), 0, 0, &status);
    std::string s(name);
    ::free(name);
    return s;
}

class A {
    public:
    A() { std::cout << "A::A(): " << type(*this) << std::endl; }
    virtual ~A() { std::cout << "A::~A(): " << type(*this) << std::endl; }
    void f() const { std::cout << "A::f: " << type(*this) << std::endl; }
    virtual void g() const { std::cout << "A::g: " << type(*this) << std::endl; }
};

class B : public A {
    public:
    B() { std::cout << "B::B(): " << type(*this) << std::endl; }
    ~B() { std::cout << "B::~B(): " << type(*this) << std::endl; }
};

class C : public B {
    public:
    C() { std::cout << "C::C(): " << type(*this) << std::endl; }
    ~C() { std::cout << "C::~C(): " << type(*this) << std::endl; }
};

int main() {
    C c;
    c.f();
    c.g();

    std::string str;

    std::cout << type(str) << std::endl;

    std::vector<char> chars;

    std::cout << type(chars) << std::endl;

    std::cout << typeid(chars).name() << std::endl;

}