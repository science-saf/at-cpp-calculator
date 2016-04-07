#include "stdafx.h"
#include "at_cpp_calculator/calculator.h"

using namespace Calc;
using namespace std;
using string_ref = boost::string_ref;

const double DISPLACEMENT = 0.0001;

BOOST_AUTO_TEST_SUITE(CalculatorTestSuite)

BOOST_AUTO_TEST_CASE(TestParseUnsignedInt)
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
		BOOST_CHECK_EQUAL(it.second, c.parseDouble(s));
	}
}

BOOST_AUTO_TEST_CASE(TestParseSignedInt)
{
	Calculator c;
	map<string_ref, double> testData({
		{ string_ref("-0"), double(0) },
		{ string_ref("-7"), double(-7) }
	});
	for (auto it : testData)
	{
		string_ref s = it.first;
		BOOST_CHECK_EQUAL(it.second, c.parseUnary(s));
	}
}

BOOST_AUTO_TEST_CASE(TestParseUnsignedFloat)
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
		BOOST_CHECK_CLOSE(it.second, c.parseDouble(s), DISPLACEMENT);
	}
}

BOOST_AUTO_TEST_CASE(TestParseSignedFloat)
{
	Calculator c;
	map<string_ref, double> testData({
		{ string_ref("-7.3"), double(-7.3) },
		{ string_ref("+000000000000000000.9"), double(0.9) },
		{ string_ref("-11.0"), double(-11) },
		{ string_ref("+123.00000000000000"), double(123) },
		{ string_ref("-0.008"), double(-0.008) }
	});
	for (auto it : testData)
	{
		string_ref s = it.first;
		BOOST_CHECK_CLOSE(it.second, c.parseUnary(s), DISPLACEMENT);
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

BOOST_AUTO_TEST_CASE(TestParseExprMul)
{
	Calculator c;
	map<string_ref, double> testData({
		{ string_ref("2*3"), double(6) },
		{ string_ref("4/2"), double(2) },
		{ string_ref("2*2*2"), double(8) },
		{ string_ref("-4/2"), double(-2) },
		{ string_ref("-4/2/2"), double(-1) },
	});
	for (auto it : testData)
	{
		string_ref s = it.first;
		BOOST_CHECK_EQUAL(it.second, c.parseExprMul(s));
	}
}

BOOST_AUTO_TEST_CASE(TestParseExpr)
{
	Calculator c;
	map<string_ref, double> testData({
		{ string_ref("2*3"), double(6) },
		{ string_ref("4/2"), double(2) },
		{ string_ref("2+2*2"), double(6) },
		{ string_ref("-4/2"), double(-2) },
	});
	for (auto it : testData)
	{
		string_ref s = it.first;
		BOOST_CHECK_EQUAL(it.second, c.parseExpr(s));
	}
}

BOOST_AUTO_TEST_CASE(TestNan)
{
	Calculator c;
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("a"))));
	BOOST_CHECK_EQUAL(54, c.parseExpr(string_ref("54a")));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("--1"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("- -1"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("++1"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("+ +1"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("*2"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("/2"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("3-"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("3+"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("3*"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("3/"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("4."))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("4/-2"))));
	BOOST_ASSERT(isnan(c.parseExpr(string_ref("4/+2"))));
}

BOOST_AUTO_TEST_CASE(TestSkipSpaces)
{
	Calculator c;
	map<string_ref, double> testData1({
		{ string_ref("  7"), double(7) },
		{ string_ref("10  "), double(10) },
		{ string_ref("  45646546456  "), double(45646546456) },
		{ string_ref("0"), double(0) }
	});
	for (auto it : testData1)
	{
		string_ref s = it.first;
		BOOST_CHECK_EQUAL(it.second, c.parseDouble(s));
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

BOOST_AUTO_TEST_CASE(TestPolishNotation)
{
	map<string_ref, string> testData({
		{ string_ref("1"), string("1") },
		{ string_ref("1+2"), string("1 2 +") },
		{ string_ref("1+2*3"), string("1 2 3 * +") },
		{ string_ref("1*2+3*4"), string("1 2 * 3 4 * +") },
		{ string_ref("-1+2"), string("-1 2 +") },
		{ string_ref("-1*2+3*4"), string("-1 2 * 3 4 * +") },
		{ string_ref("+1*2+3*4"), string("1 2 * 3 4 * +") },
	});
	for (auto it : testData)
	{
		Calculator c;
		ostringstream debugStream;
		c.setDebugStream(debugStream);
		string_ref str = it.first;
		c.parseExpr(str);
		BOOST_CHECK_EQUAL(debugStream.str(), it.second);
	}
}

BOOST_AUTO_TEST_SUITE_END()

