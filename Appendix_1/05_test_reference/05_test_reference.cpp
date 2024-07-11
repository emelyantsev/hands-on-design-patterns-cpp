#include <iostream>


template<typename T>
class Holder {

    public:

        Holder(T val_) : val(val_) {}
        Holder(Holder&& other) = default ; 


    private:


        T val;
};


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

}