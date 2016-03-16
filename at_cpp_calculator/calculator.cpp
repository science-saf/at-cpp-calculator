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

double Calculator::parseFloat(string_ref &ref)
{
	skipSpaces(ref);
	double value = 0;
	bool parsedAny = false;
	while (!ref.empty() && isdigit(ref[0]))
	{
		parsedAny = true;
		const int digit = ref[0] - '0';
		value = value * 10.0f + double(digit);
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
	double factor = 1.f;
	while (!ref.empty() && isdigit(ref[0]))
	{
		const int digit = ref[0] - '0';
		factor *= 0.1f;
		value += factor * double(digit);
		ref.remove_prefix(1);
	}

	return value;
}

double Calculator::parseExprMul(boost::string_ref &ref)
{
	double value = parseFloat(ref);
	while (true)
	{
		skipSpaces(ref);
		if (!ref.empty() && ref[0] == '*')
		{
			ref.remove_prefix(1);
			value *= parseFloat(ref);
		}
		else if (!ref.empty() && ref[0] == '/')
		{
			ref.remove_prefix(1);
			value /= parseFloat(ref);
		}
		else
		{
			break;
		}
	}

	return value;
}

double Calculator::parseExprSum(boost::string_ref &ref)
{
	double value = parseFloat(ref);
	while (true)
	{
		skipSpaces(ref);
		if (!ref.empty() && ref[0] == '+')
		{
			ref.remove_prefix(1);
			value += parseFloat(ref);
		}
		else if (!ref.empty() && ref[0] == '-')
		{
			ref.remove_prefix(1);
			value -= parseFloat(ref);
		}
		else
		{
			break;
		}
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
