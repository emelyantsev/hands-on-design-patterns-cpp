#include <map>
#include <iostream>

template<typename T>
class AbsLess {

    public:

    AbsLess() {
        std::cout << "AbsLess constructed " << std::endl;
    }

    bool operator()(const T& lhs, const T& rhs) const {
        return std::abs(lhs) < std::abs(rhs);
    }

} ;

template<typename KEY, typename VALUE, typename CMP>
std::ostream& operator<<(std::ostream& os, const std::map<KEY, VALUE, CMP>& m)
{
   
    std::cout << "[ " << std::endl;

    for (const auto& [key, value] : m) {

        std::cout << "{ " << key << " : " << value << " }" << std::endl;  
    }

    std::cout << " ]" << std::endl;

    return os;
}


int main() {

    std::map<int, std::string, AbsLess<int> > m1 = {
        {8, "eight"},
        {4, "four"},
        {5, "five"}, 
        {-10, "minus ten"}
    } ;


    std::map<int , std::string, AbsLess<int> > m2 = {
        {1, "Raven"},
        {2, "Pigeon"}
    } ;

    std::cout << m1 << m2 << std::endl;

    m1.swap(m2) ;

    std::cout << m1 << m2 << std::endl;


    return 0;
}