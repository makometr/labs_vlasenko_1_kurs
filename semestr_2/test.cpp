#include <iostream>
#include <string>

int main(){
    std::string a = "Hello";
    std::string b;

    std::cout << "A: " << a << "\n";
    std::cout << "B: " << b << "\n";

    b = std::move(a);

    std::cout << "A: " << a << "\n";
    std::cout << "B: " << b << "\n";
}