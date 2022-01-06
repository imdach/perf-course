#include <iostream>

namespace A {
    int a = 100;
}
namespace B {
    int a = 200;
}

int main() {
    std::cout << "In A, a = " << A::a << std::endl;//100
    std::cout << "In B, a = " << B::a << std::endl;//200
}