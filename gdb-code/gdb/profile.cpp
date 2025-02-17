#include <iostream>
#include <cmath>

double function1(double x) {
    double result = 0.0;
    for (int i = 0; i < 10000; i++) {
        result += sin(x) * cos(x);
    }
    return result;
}


double function2(double x) {
    double result = 0.0;
    for (int i = 0; i < 20000; i++) {
        result += sin(x) * cos(x);
    }
    return result;
}

int main() {
    double x = 1.0;
    int i=0;
    for(;i<0xffffff;i++);
    for (int i = 0; i < 8000; i++) {
        x = function1(x);
    }
    for (int i = 0; i < 4000; i++) {
        x = function2(x);  
    }
    std::cout << "Final result: " << x << std::endl;
    return 0;
}
