#include <stdlib.h>
#include <typeinfo>
#include <iostream>

#include <cxxabi.h>
#include <type_traits>

template <typename T> auto type(T&& p) {
    std::string name;
    using TT = std::remove_reference_t<T>;
    if (std::is_const<TT>::value) name += "const ";
    if (std::is_volatile<TT>::value) name += "volatile ";
    int status;
    char* mangled_name = abi::__cxa_demangle(typeid(p).name(), 0, 0, &status);
    name += mangled_name;
    ::free(mangled_name);
 //   if (std::is_lvalue_reference<decltype(p)>::value) name += "&";
 //   if (std::is_rvalue_reference<decltype(p)>::value) name += "&&";
    return name;
}


typedef int (*Func)(int, int) ;


int sum(int x, int y) {

    return x + y;
}



int main() {


    Func f = sum;

    int m = f(2, 3) ;


    std::cout << type(f) << std::endl;
    std::cout << type(m) << std::endl;


    Func fp = &sum;

    int k = fp( 4, 5 ) ;

    std::cout << type(fp) << std::endl;

    std::cout << type( sum ) << std::endl;

    static_assert(  std::is_same_v<  std::decay_t< decltype(sum) >, decltype( fp ) > ) ; 

    static_assert(  !std::is_same_v<  decltype(sum) , decltype( fp ) > ) ; 

    return EXIT_SUCCESS;
}


