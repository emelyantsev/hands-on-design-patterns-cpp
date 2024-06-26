#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>


int main() {

    std::vector<int> input1 = {8, 4, 5};

    std::vector<int> output1;

    auto it = std::back_inserter( output1 ) ;

    for (int i : input1) {

        it = i;
    }





    return 0;
}