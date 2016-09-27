#pragma once

#include "targetver.h"

#define _AFXDLL
#include <afxwin.h>

#include <stdio.h>
#include <tchar.h>

#pragma warning(push)
#pragma warning(disable: 4996)
#include <boost/signals2.hpp>
#pragma warning(pop)

#define BOOST_TEST_INCLUDED
#pragma warning (disable: 4702)
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <iostream>
