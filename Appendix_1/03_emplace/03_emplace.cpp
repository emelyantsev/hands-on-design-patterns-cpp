#include <vector>
#include <iostream>

class Item {

    public:

        Item(int i, double d) : i_(i), d_(d) {
            std::cout << "Item constructed with " << i << " " << d << std::endl;
        }   

        Item(const Item& other) : i_(other.i_), d_(other.d_) {
            std::cout << "Item copy constructed with " << i_ << " " << d_ << std::endl;
        }
 

    private:

        int i_;
        double d_;

};


int main() {


    Item item1(8, 3.14) ;

    Item item2 = item1;

    std::vector<Item> items;

    items.reserve(5);

    items.push_back( item1 ) ;

    items.emplace_back( 8, 3.14 ) ;


    return 0;
}

