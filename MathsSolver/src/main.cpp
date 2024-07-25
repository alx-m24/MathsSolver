#include <string>
#include <iostream>
#include "Headers/Equation.hpp"

int main() {
	string equ = "";
	std::cout << "Enter equation: ";
	std::getline(std::cin, equ);

	Expression expr(equ);
	expr.Print();
	std::cout << std::endl;
	std::cout << expr.Evaluate() << std::endl;

	return EXIT_SUCCESS;
}