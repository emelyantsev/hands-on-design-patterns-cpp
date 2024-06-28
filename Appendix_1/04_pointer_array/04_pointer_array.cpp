#include <stdlib.h>
#include <typeinfo>
#include <iostream>
#include <cxxabi.h>
#include <type_traits>
#include <iterator>


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


using PTR_ARR_TYPE = int (*)[3] ;

typedef int ( *PTR_ARR_TYPE2 )[3] ;

int main() {


    int arr[3] = {8, 4, 5} ;

    std::cout << sizeof(arr) << std::endl;

    std::cout << type( arr) << std::endl;

    for (auto it = std::begin(arr); it != std::end( arr ) ; ++it ) {
        std::cout << *it << " " ;
    }

    std::cout << std::endl;


    int* ptr = arr;

    for (int i = 0 ; i < 3; ++i) {

        std::cout << ptr[i] << " " ;
    }

    std::cout << std::endl;


    std::cout << sizeof( ptr ) << std::endl;

    std::cout << type( ptr ) << std::endl;



    int (* ptr2 )[3] = &arr;

    std::cout << sizeof( ptr2 ) << std::endl;

    std::cout << type( ptr2 ) << std::endl;


    std::cout << sizeof( *ptr2 ) << std::endl;

    std::cout << type( *ptr2 ) << std::endl;


    for (int i = 0; i < 3; ++i) {

        std::cout << (*ptr2 + i) << " ";
    }

    std::cout << std::endl;


    for (int i = 0; i < 3; ++i) {

        std::cout << ( arr + i ) << " ";
    }
    std::cout << std::endl;


    std::cout << arr << std::endl;


    PTR_ARR_TYPE2 ptr3 = &arr;


    for (int i = 0; i < 3; ++i) {

        std::cout << *(*ptr3 + i) << " ";
    }

    std::cout << std::endl;

    static_assert( std::is_same<PTR_ARR_TYPE, PTR_ARR_TYPE2>::value == true  ) ;


    return EXIT_SUCCESS;
}