#pragma once

#include "stdafx.h"

namespace Calc
{

class Calculator
{
public:
	Calculator();
	~Calculator();
	double parseExpr(boost::string_ref &ref);
	double parseExprSum(boost::string_ref &ref);
	double parseExprMul(boost::string_ref &ref);
	double parseUnary(boost::string_ref &ref);
	double parseDouble(boost::string_ref &ref);
	void setDebugStream(std::ostream& outStream);
private:
	void skipSpaces(boost::string_ref &ref);
	void log(char ch);
	void log(double d);
	void log(std::string str);
	std::ostream* m_debugStream = nullptr;
	bool m_isAddSpaceToLog = true;
	std::stack<char> m_operatorsStack;
};

}