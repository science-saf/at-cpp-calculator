// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define BOOST_TEST_MODULE CalculatorTestsModule
#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>

#include <boost/utility/string_ref.hpp>
#include <string>
#include <map>
#include <boost/lexical_cast.hpp>