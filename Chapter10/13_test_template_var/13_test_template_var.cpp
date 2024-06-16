// type-erased pointer holder;

#include <iostream>
#include <typeinfo>

class PtrHolder {

        using FuncType = void (*)() ;

        template<typename T> 
        static void print_func() {
            std::cout << typeid( T ).name() << " : " << counter<T> << std::endl; 
        }

        template <typename T>
        static int counter;

    public:

        template <typename T>
        PtrHolder(T * ptr) : ptr_( ptr ), p_func( print_func<T>  ) {
            
            ++counter<T>;
            std::cout << typeid( T ).name() << " : " << counter<T> << std::endl; 
        }

        template <typename T>
        T* as() {

            return reinterpret_cast<T*>( ptr_ ) ;
        }

        void PrintTypeAndCount() {
            p_func();
        }

    private:

        void* ptr_;
        FuncType p_func;

};


template <typename T>
int PtrHolder::counter = 0;

int main() {


    int x = 845;

    PtrHolder h1(&x);

    int y = *h1.as<int>();

    std::cout << y << std::endl;


    PtrHolder h2(&x);

    h1.PrintTypeAndCount();

    std::cout << "---------------" << std::endl;

    double d1 = 3.14;

    PtrHolder h3( &d1) ;

    std::cout << *h3.as<double>() << std::endl;
    std::cout << *h3.as<int>() << std::endl;

    h3.PrintTypeAndCount();

    std::cout << "---------------" << std::endl;


    std::string str("hello") ;

    PtrHolder h4(&str) ;


    std::string str2("world") ;

    PtrHolder h5(&str2);

    h5.PrintTypeAndCount();
    h4.PrintTypeAndCount();

}