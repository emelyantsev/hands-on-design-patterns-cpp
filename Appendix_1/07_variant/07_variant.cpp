#include <variant>
#include <string>
#include <cassert>
#include <iostream>
#include <vector>

using var_t = std::variant<int, double, std::string> ;

class PrintVisitor {

public:

    void operator()(const int& val) {
        ++ic;
        std::cout << val << std::endl;
    }

    void operator()(const double& val) {
        ++dc;
        std::cout << val << std::endl;
    }

    void operator()(const std::string& val) {
        ++sc;
        std::cout << val << std::endl;
    }

    void printStat() {
        std::cout << "I've visited " << ic << " integers, " << dc << " doubles, " << sc << " strings" << std::endl;
    }

private:

    int ic = 0;
    int dc = 0;
    int sc = 0;

} ;


void print_var(const var_t& var) {

    if (var.index() == 0) {
        std::cout << std::get<int>(var) << std::endl;
    }
    else if (var.index() == 1) {
        std::cout << std::get<double>(var) << std::endl;
    }
    else if (var.index() == 2) {
        std::cout << std::get<std::string>(var) << std::endl;
    }


}


int main() {

    std::variant<int, double, std::string> v1, v2, v3;

    v1 = 845;
    v2 = 3.14;
    v3 = "INT_MAX" ;




    std::vector<var_t> vars = {v1, v2, v3 };

    for (const var_t v : vars ) {

        std::cout << v.index() << std::endl;
    }

    PrintVisitor pv;

    for (const var_t v : vars ) {

        std::visit(pv, v);
    }

    pv.printStat();

    for (const var_t v : vars ) {

        print_var(v);
    }

}