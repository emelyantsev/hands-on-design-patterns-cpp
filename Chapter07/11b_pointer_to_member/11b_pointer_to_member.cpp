#include <iostream>

struct C { 
    int m;
    int n; 
};
 
int main()
{
    int C::* p = &C::m;          // pointer to data member m of class C
    C c = {7, 2};

    std::cout << c.*p << '\n';   
    
    p = &C::n;

    std::cout << c.*p << '\n'; 
}