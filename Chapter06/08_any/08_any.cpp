#include <any>
#include <iostream>

int main() {

    std::any a(5);

    //std::cout << std::any_cast<long>(a) << std::endl;

    std::any s = "smile" ;

    std::cout << s.type().name() << std::endl;

    std::cout << std::any_cast<const char *>(s) << std::endl;
 
}