#include "stdafx.h"
#include "at_cpp_calculator/calculator.h"

using namespace Calc;
using namespace std;
using string_ref = boost::string_ref;

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

BOOST_AUTO_TEST_SUITE(CalculatorTestSuite)

BOOST_AUTO_TEST_CASE(TestParseInteger)
{
	Calculator c;
	map<string_ref, double> testData({
		{ string_ref("7"), double(7) },
		{ string_ref("10"), double(10) },
		{ string_ref("45646546456"), double(45646546456) },
		{ string_ref("0"), double(0) }
	});
	for (auto it : testData)
	{
		string_ref s = it.first;
		BOOST_CHECK_EQUAL(it.second, c.parseFloat(s));
	}
}

BOOST_AUTO_TEST_CASE(TestParseFloat)
{
	Calculator c;
	map<string_ref, double> testData({
		{ string_ref("7.3"), double(7.3) },
		{ string_ref("000000000000000000.9"), double(0.9) },
		{ string_ref("11.0"), double(11) },
		{ string_ref("123.00000000000000"), double(123) },
		{ string_ref("0.008"), double(0.008) }
	});
	for (auto it : testData)
	{
		string_ref s = it.first;
		BOOST_CHECK_CLOSE(it.second, c.parseFloat(s), 0.0001f);
	}
}

BOOST_AUTO_TEST_CASE(TestParseExprSum)
{
	Calculator c;
	map<string_ref, double> testData({
		{ string_ref("2+3"), double(5) },
		{ string_ref("2-3"), double(-1) },
		{ string_ref("1+2+3-4-5-6-7"), double(-16) }
	});
	for (auto it : testData)
	{
		string_ref s = it.first;
		BOOST_CHECK_EQUAL(it.second, c.parseExprSum(s));
	}
}

BOOST_AUTO_TEST_CASE(TestNan)
{
	Calculator c;
	BOOST_ASSERT(isnan(c.parseFloat(string_ref(string("a")))));
	BOOST_CHECK_EQUAL(54, c.parseFloat(string_ref(string("54a"))));
}

BOOST_AUTO_TEST_CASE(TestSkipSpaces)
{
	Calculator c;
	map<string_ref, double> testData1({
		{ string_ref("7"), double(7) },
		{ string_ref("10"), double(10) },
		{ string_ref("45646546456"), double(45646546456) },
		{ string_ref("0"), double(0) }
	});
	for (auto it : testData1)
	{
		string_ref s = it.first;
		BOOST_CHECK_EQUAL(it.second, c.parseFloat(s));
	}
	map<string_ref, double> testData2({
		{ string_ref("2   +		3"), double(5) },
		{ string_ref("2		-3                 "), double(-1) },
		{ string_ref("      1 +2 +3    -4 -5- 6- 7     "), double(-16) }
	});
	for (auto it : testData2)
	{
		string_ref s = it.first;
		BOOST_CHECK_EQUAL(it.second, c.parseExprSum(s));
	}
}

BOOST_AUTO_TEST_SUITE_END()

