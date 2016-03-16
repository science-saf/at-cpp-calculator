#pragma once

namespace Calc
{

class Calculator
{
public:
	Calculator();
	~Calculator();
	double parseFloat(boost::string_ref &ref);
	double parseExprMul(boost::string_ref &ref);
	double parseExprSum(boost::string_ref &ref);
private:
	void skipSpaces(boost::string_ref &ref);
};

}