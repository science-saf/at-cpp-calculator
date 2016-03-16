#include "stdafx.h"
#include "calculator.h"

using namespace Calc;
using namespace std;
using namespace boost;

Calculator::Calculator()
{
}


Calculator::~Calculator()
{
}

double Calculator::parseExpr(string_ref &ref)
{
	return parseExprSum(ref);
}

double Calculator::parseExprSum(boost::string_ref &ref)
{
	double value = parseExprMul(ref);
	while (true)
	{
		skipSpaces(ref);
		if (!ref.empty() && ref[0] == '+')
		{
			ref.remove_prefix(1);
			value += parseExprMul(ref);
		}
		else if (!ref.empty() && ref[0] == '-')
		{
			ref.remove_prefix(1);
			value -= parseExprMul(ref);
		}
		else
		{
			break;
		}
	}

	return value;
}

double Calculator::parseExprMul(boost::string_ref &ref)
{
	double value = parseUnary(ref);
	while (true)
	{
		skipSpaces(ref);
		if (!ref.empty() && ref[0] == '*')
		{
			ref.remove_prefix(1);
			value *= parseDouble(ref);
		}
		else if (!ref.empty() && ref[0] == '/')
		{
			ref.remove_prefix(1);
			value /= parseDouble(ref);
		}
		else
		{
			break;
		}
	}

	return value;
}

double Calculator::parseUnary(string_ref &ref)
{
	skipSpaces(ref);
	int coefficient = 1;
	if (ref[0] == '-')
	{
		ref.remove_prefix(1);
		coefficient = -1;
	}
	else if (ref[0] == '+')
	{
		ref.remove_prefix(1);
	}
	
	if (ref.empty())
	{
		return numeric_limits<double>::quiet_NaN();
	}
	return coefficient * parseDouble(ref);
}

double Calculator::parseDouble(string_ref &ref)
{
	skipSpaces(ref);
	double value = 0;
	bool parsedAny = false;
	while (!ref.empty() && isdigit(ref[0]))
	{
		parsedAny = true;
		const int digit = ref[0] - '0';
		value = value * 10.0 + double(digit);
		ref.remove_prefix(1);
	}
	if (!parsedAny)
	{
		return numeric_limits<double>::quiet_NaN();
	}

	if (ref.empty() || (ref[0] != '.'))
	{
		return value;
	}
	ref.remove_prefix(1);
	double factor = 1;
	if (ref.empty()) {
		return numeric_limits<double>::quiet_NaN();
	}
	while (!ref.empty() && isdigit(ref[0]))
	{
		const int digit = ref[0] - '0';
		factor *= 0.1;
		value += factor * double(digit);
		ref.remove_prefix(1);
	}

	return value;
}

void Calculator::skipSpaces(boost::string_ref &ref)
{
	size_t i = 0;
	while (i < ref.size() && isspace(ref[i]))
		++i;
	ref.remove_prefix(i);
}
