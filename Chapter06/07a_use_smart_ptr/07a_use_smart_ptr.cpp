#include "07_smartptr.h"
#include <iostream>
#include <vector>

class Printer {

    public:

        Printer() {
            std::cout << "I'm consructed" << std::endl;
        }

        ~Printer( ) {
            std::cout << "I'm destructed" << std::endl;
        }
};





int main() {

    // {
    //     smartptr<Printer, deleter > p1 ( new Printer, deleter{} ) ;
    // }

    // {
    //     smartptr<Printer, deleter1<Printer> > p1 ( new Printer, deleter1<Printer>{} ) ;
    // }

    // {
    //     smartptr_te<Printer> ptr(new Printer, deleter() ) ;
    // }
    
    // {
    //     deleter1<Printer> d1;
    //     smartptr_te<Printer> ptr(new Printer, d1 ) ;
    // }

    // {
    //     smartptr_te_lb<Printer> ptr( new Printer, deleter{} ) ;  
    //     std::cout << sizeof(deleter) << std::endl;
    // }

    // {
    //     smartptr_te_lb0<Printer> ptr( new Printer, deleter1<Printer>{} ) ;
    // }

    // {
    //     smartptr_te_static ptr( new Printer, deleter{} ) ;        
    // }

    {

        smartptr_te_vtable ptr1( new Printer, deleter{} );
        smartptr_te_vtable ptr2( new Printer, deleter1<Printer>{} ) ;

        static_assert( std::is_same_v< decltype( ptr1 ), decltype( ptr2 ) > == true );

    }


    return 0;
}