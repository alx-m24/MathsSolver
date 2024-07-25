#pragma once
#include <array>
#include <string>
#include <iostream>
#include <unordered_map>

using std::string;

/*
* Equation: 
*	-Has Equal sign
*	-Is made up of expression
* Expression:
*	-Has term and operator
* Term:
*	-Has a coefficient and a variable(optional)
* Plan:
*	-Expression has left and right sub-expression
*	-If this sub-expression cannot be broken down in other sub-expression
*	it is a term.
*/

struct Term {
	float coef = 0.0f;
	string var = "";

	Term() = default;
	Term(string str);
};

namespace Op {
	enum Operator {
		NONE,
		PLUS,
		MINUS,
		MUL,
		DIV,
		POW
	};
	extern std::unordered_map<char, Operator> Operators;
	extern std::unordered_map<Operator, char> RevOperators;
	extern std::array<char, 5> BOMMAS;
	bool isOperator(char c);
}

using namespace Op;

enum Side {
	LEFT,
	RIGHT
};

class Expression {
private:
	Expression* leftExpr = nullptr;
	Expression* rightExpr = nullptr;

private:
	Term left = {};
	Term right = {};

private:
	Operator op = NONE;

private:
	Term answer = {};

private:
	void NormalParse(const string& str);
	void BracketParse(const string& str, int openBrackIdx);

public:
	Expression() = default;
	Expression(string str);
	~Expression();

public:
	void Parse(const string& str);
	void Print();
	float Evaluate();
	bool isTerm(Side side);
	Term getTerm(Side side);
	Expression* getExpr(Side side);

public:
	void operator=(const string& str);
};
