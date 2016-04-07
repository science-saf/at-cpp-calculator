#include "stdafx.h"
#include "calculator.h"

using namespace Calc;
using namespace std;
using string_ref = boost::string_ref;

Calculator::Calculator()
{
}


Calculator::~Calculator()
{
}

double Calculator::parseExpr(string_ref &ref)
{
	m_isAddSpaceToLog = false;
	double result = parseExprSum(ref);
	if (m_debugStream != nullptr)
	{
		while (m_operatorsStack.size() > 0)
		{
			log(m_operatorsStack.top());
			m_operatorsStack.pop();
		}
	}

	return result;
}

double Calculator::parseExprSum(string_ref &ref)
{
	double value = parseExprMul(ref);
	while (true)
	{
		skipSpaces(ref);
		if (!ref.empty() && ref[0] == '+')
		{
			if (m_operatorsStack.size() > 0)
			{
				while ((m_operatorsStack.size() > 0)
					&& ((m_operatorsStack.top() == '*') || (m_operatorsStack.top() == '/') || (m_operatorsStack.top() == '-')))
				{
					log(m_operatorsStack.top());
					m_operatorsStack.pop();
				}
				m_operatorsStack.push('+');
			}
			else
			{
				m_operatorsStack.push('+');
			}
			ref.remove_prefix(1);
			value += parseExprMul(ref);
		}
		else if (!ref.empty() && ref[0] == '-')
		{
			if (m_operatorsStack.size() > 0)
			{
				while ((m_operatorsStack.size() > 0)
					&& ((m_operatorsStack.top() == '*') || (m_operatorsStack.top() == '/') || (m_operatorsStack.top() == '+')))
				{
					log(m_operatorsStack.top());
					m_operatorsStack.pop();
				}
				m_operatorsStack.push('-');
			}
			else
			{
				m_operatorsStack.push('-');
			}
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

double Calculator::parseExprMul(string_ref &ref)
{
	double value = parseUnary(ref);
	while (true)
	{
		skipSpaces(ref);
		if (!ref.empty() && ref[0] == '*')
		{
			if (m_operatorsStack.size() > 0)
			{
				while ((m_operatorsStack.size() > 0)
					&& ((m_operatorsStack.top() == '*') || (m_operatorsStack.top() == '/')))
				{
					log(m_operatorsStack.top());
					m_operatorsStack.pop();
				}
				m_operatorsStack.push('*');
			}
			else
			{
				m_operatorsStack.push('*');
			}
			ref.remove_prefix(1);
			value *= parseDouble(ref);
		}
		else if (!ref.empty() && ref[0] == '/')
		{
			if (m_operatorsStack.size() > 0)
			{
				while ((m_operatorsStack.size() > 0)
					&& ((m_operatorsStack.top() == '*') || (m_operatorsStack.top() == '/')))
				{
					log(m_operatorsStack.top());
					m_operatorsStack.pop();
				}
				m_operatorsStack.push('/');
			}
			else
			{
				m_operatorsStack.push('/');
			}
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
		log('-');
		m_isAddSpaceToLog = false;
	}
	else if (ref[0] == '+')
	{
		ref.remove_prefix(1);
	}
	
	if (ref.empty())
	{
		return numeric_limits<double>::quiet_NaN();
	}
	double value = coefficient * parseDouble(ref);

	return value;
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
		log(value);
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
	log(value);

	return value;
}

void Calculator::setDebugStream(ostream& outStream)
{
	m_debugStream = &outStream;
}

void Calculator::skipSpaces(string_ref &ref)
{
	size_t i = 0;
	while (i < ref.size() && isspace(ref[i]))
	{
		++i;
	}
	ref.remove_prefix(i);
}

void Calc::Calculator::log(char ch)
{
	log(move(string(1, ch)));
}

void Calc::Calculator::log(double d)
{
	log(boost::lexical_cast<string>(d));
}

void Calc::Calculator::log(string str)
{
	if (m_debugStream != nullptr)
	{
		if (m_isAddSpaceToLog)
		{
			*m_debugStream << " ";
		}
		*m_debugStream << str;
		m_isAddSpaceToLog = true;
	}
}
