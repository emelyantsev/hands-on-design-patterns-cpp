#include <iostream>
#include <string>
#include <utility>

template<typename T>
class Holder {

    public:

        Holder(T val_) : val(val_) {}
        Holder(Holder&& other) = default ; 


    private:


        T val;
};

void func(std::string&& str) {

    std::string temp( std::move(str) ) ;

    std::cout << temp << std::endl;
    std::cout << str << std::endl;
}


int main() {

    int x = 5;
    int& ref_x = x;
    //int& r2 = 5;
    int&& r2 = 5;
    const int& r3 = 8;
    std::cout << r2 << std::endl;
    std::cout << r3 << std::endl;


    Holder h1( 10 );

    Holder<int>& rh1 = h1;

    Holder<int>&& rh2 = Holder(10);


    std::string str1 = "Hello!" ;

    // std::string&& ref1 = str1;

    // func( str1 ) ;

    func( std::move(str1) ) ;

}