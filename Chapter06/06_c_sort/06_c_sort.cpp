#include <stdlib.h>
#include <iostream>

int less(const void* a, const void* b) {
    return *(const int*) a - * (const int*) b;
}

int cmp_double(const void *a, const void *b) {

    double l = *(double *) a; 
    double r = *(double *) b; 

    return l < r ? -1 : ( l == r ? 0 : 1 ) ;
}

int main() {

    int a[10] = { 1, 10, 2, 9, 3, 8, 4, 7, 5, 0 };
    
    qsort(a, 10, sizeof(int), less);
    
    for (int x : a) std::cout << x << " ";
    std::cout << std::endl;


    double b[4] = {3.14, 8.45, 1.23, -2.999} ;

    std::cout << sizeof(b) << std::endl;

    qsort(b, sizeof(b) / sizeof(double), sizeof(double) ,  cmp_double  ) ;

    for (double x : b) std::cout << x << " ";
    std::cout << std::endl;

}