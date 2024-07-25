#include "Equation.hpp"

std::unordered_map<char, Operator> Op::Operators = {
		{'+', PLUS},
		{'-', MINUS},
		{'*', MUL},
		{'/', DIV},
		{'^', POW}
};
std::unordered_map<Operator, char> Op::RevOperators = {
		{PLUS, '+'},
		{MINUS, '-'},
		{MUL, '*'},
		{DIV, '/'},
		{POW, '^'}
};
std::array<char, 5> Op::BOMMAS = { '^', '/', '*', '-', '+'};

using namespace Op;

void Expression::NormalParse(const string& str)
{
	int opIdx = 0;
	for (int i = BOMMAS.size() - 1; i >= 0; --i) {
		const char op = BOMMAS[i];

		bool exitLoop = false;
		for (opIdx = 0; opIdx < str.size(); ++opIdx) {
			if (str[opIdx] == op) {
				exitLoop = true;
				break;
			}
		}
		if (exitLoop) break;
	}

	string leftStr = str.substr(0, opIdx);
	op = Operators[str[opIdx]];
	string rightStr = str.substr(opIdx + 1, str.size() - opIdx);

	for (const char& c : leftStr) {
		if (isOperator(c)) {
			leftExpr = new Expression(leftStr);
			break;
		}
	}
	if (leftExpr == nullptr) left = Term(leftStr);

	for (const char& c : rightStr) {
		if (isOperator(c)) {
			rightExpr = new Expression(rightStr);
			break;
		}
	}
	if (rightExpr == nullptr) right = Term(rightStr);
}

void Expression::BracketParse(const string& str, int openBrackIdx)
{
	int open = 1, closeBrackIdx = -1;
	for (int i = openBrackIdx + 1; i < str.length(); ++i) {
		if (str[i] == '(') ++open;
		else if (str[i] == ')') {
			--open;
			if (open == 0) {
				closeBrackIdx = i;
				break;
			}
		}
	}

	string sub = str.substr(openBrackIdx + 1, closeBrackIdx - 1);

	if (openBrackIdx <= 0 && closeBrackIdx >= str.length() - 1) {
		Parse(sub);
		return;
	}
	// Bracket expr is on the right
	int opIdx = 0;
	if (openBrackIdx > 0) {
		char c = str[openBrackIdx - 1];
		if (isOperator(c)) {
			op = Operators[c];
			opIdx = openBrackIdx - 1;

			string leftStr = str.substr(0, opIdx);

			for (const char& c : leftStr) {
				if (isOperator(c)) {
					leftExpr = new Expression(leftStr);
					break;
				}
			}
			if (leftExpr == nullptr) left = Term(leftStr);

			rightExpr = new Expression(sub);
		}
	}
	// Bracket expr is on the left
	else {
		char c = str[closeBrackIdx + 1];
		if (!isOperator(c)) return;
		op = Operators[c];
		opIdx = closeBrackIdx + 1;

		string rightStr = str.substr(opIdx + 1, str.size() - opIdx);
		for (const char& c : rightStr) {
			if (isOperator(c)) {
				rightExpr = new Expression(rightStr);
				break;
			}
		}
		if (rightExpr == nullptr) right = Term(rightStr);

		leftExpr = new Expression(sub);
	}
}

Expression::Expression(string str)
{
	this->Parse(str);
}

Expression::~Expression()
{
	if (leftExpr != nullptr) delete leftExpr;
	if (rightExpr != nullptr) delete rightExpr;
}

void Expression::Parse(const string& str)
{	
	int openBrackIdx = -1;

	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == '(') {
			openBrackIdx = i;
			break;
		}
	}

	if (openBrackIdx == -1) NormalParse(str);
	else BracketParse(str, openBrackIdx);
}

void Expression::Print()
{
	if (leftExpr != nullptr) {
		std::cout << "(";
		leftExpr->Print();
		std::cout << ")";
	}
	else std::cout << left.coef << left.var;

	std::cout << " ";

	std::cout << RevOperators[op];

	std::cout << " ";

	if (rightExpr != nullptr) {
		std::cout << "(";
		rightExpr->Print();
		std::cout << ")";
	}
	else std::cout << right.coef << right.var;
}

float Expression::Evaluate()
{
	float ans = 0.0f;
	float a = (leftExpr == nullptr) ? left.coef : leftExpr->Evaluate();
	float b = (rightExpr == nullptr) ? right.coef : rightExpr->Evaluate();

	switch (op)
	{
	case Op::NONE:
		break;
	case Op::PLUS:
		ans = a + b;
		break;
	case Op::MINUS:
		ans = a - b;
		break;
	case Op::MUL:
		ans = a * b;
		break;
	case Op::DIV:
		ans = a / b;
		break;
	case Op::POW:
		ans = pow(a, b);
		break;
	default:
		break;
	}

	answer.coef = ans;

	return ans;
}

bool Expression::isTerm(Side side)
{
	switch (side)
	{
	case LEFT:
		return leftExpr == nullptr;
		break;
	case RIGHT:
		return rightExpr == nullptr;
		break;
	default:
		break;
	}
	return false;
}

Term Expression::getTerm(Side side)
{
	switch (side)
	{
	case LEFT:
		return left;
		break;
	case RIGHT:
		return right;
		break;
	default:
		break;
	}
	return Term();
}

Expression* Expression::getExpr(Side side)
{
	switch (side)
	{
	case LEFT:
		return leftExpr;
		break;
	case RIGHT:
		return rightExpr;
		break;
	default:
		break;
	}
	return nullptr;
}

void Expression::operator=(const string& str)
{
	this->Parse(str);
}

Term::Term(std::string str)
{
	string newCoef = "";
	for (const char& c : str) {
		if (c >= '0' && c <= '9') newCoef += c;
		else if (c == ' ') continue;
		else break;
	}
	this->coef = std::atof(newCoef.c_str());
}

bool Op::isOperator(char c)
{
	return Operators.find(c) != Operators.end();
}
