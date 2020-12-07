#ifndef RHOMBUS_HPP
#define RHOMBUS_HPP

#include <tuple>

template<class T>
struct Rhombus {
    std::pair<T, T> center;
    double diag1;
    double diag2;
};

// Calculates area of rhombus
template<class T>
double CalcArea(T &r) {
    return (r.diag1 * r.diag2) * 0.5;
}

// Prints rhombus
template<class T>
void Print(T &r) {
    std::cout.precision(2);
    std::cout << "Rhombus:" << std::endl;
    std::cout << "1. Center: (" << r.center.first << ", " << r.center.second << ")" << std::endl;
    std::cout << "2. Coordinates: (";
    std::cout << r.center.first + r.diag1 * 0.5 << "; " << r.center.second << "), (";
    std::cout << r.center.first << "; " << r.center.second + r.diag2 * 0.5 << "), (";
    std::cout << r.center.first - r.diag1 * 0.5 << "; " << r.center.second << "), (";
    std::cout << r.center.first << "; " << r.center.second - r.diag2 * 0.5 << ")" << std::endl;
    std::cout << "3. Area of figure: " << CalcArea(r) << std::endl;
}

#endif /* RHOMBUS_HPP */