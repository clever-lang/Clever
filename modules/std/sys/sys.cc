/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#include <cstdlib>

#ifdef CLEVER_WIN32
# include <direct.h>
# include <windows.h>
# define PATH_MAX MAX_PATH
#else
# include <dirent.h>
# include <unistd.h>
# include <sys/resource.h>
# include <sys/time.h>
#endif
#include "core/value.h"
#include "modules/std/sys/sys.h"
#include "types/native_types.h"
#include "core/pkgmanager.h"
#include "core/cexception.h"

#ifndef PATH_MAX
# define PATH_MAX 1024
#endif

namespace clever { namespace packages { namespace std {

namespace sys {

// Int system(String command)
// Calls a command and returns the exit code
static CLEVER_FUNCTION(system)
{
	if (!clever_static_check_args("s")) {
		return;
	}

	result->setInt(::system(args[0]->getStr()->c_str()));
}

// void putenv(String env)
// Sets an environment variable
static CLEVER_FUNCTION(putenv)
{
	if (!clever_static_check_args("s")) {
		return;
	}

    ::putenv(const_cast<char*>(args[0]->getStr()->c_str()));
}

// String getenv(String env)
// Gets an environment variable
static CLEVER_FUNCTION(getenv)
{
	if (!clever_static_check_args("s")) {
		return;
	}

	const char* ret = ::getenv(const_cast<char*>(args[0]->getStr()->c_str()));

	if (ret) {
		result->setStr(CSTRINGT(ret));
	} else {
		result->setStr(CSTRING(""));
	}
}

// String getcwd()
// Gets the current working directory
static CLEVER_FUNCTION(getcwd)
{
	if (!clever_static_check_no_args()) {
		return;
	}

	char temp[PATH_MAX];
    const char* path = ::getcwd(temp, PATH_MAX);

	result->setStr(CSTRING(path ? path : ""));
}

// String argv(Int n)
// Get n-th argv value
static CLEVER_FUNCTION(argv)
{
/*
	size_t i = static_cast<size_t>(CLEVER_ARG_INT(0));

	if (i >= (size_t)*g_clever_argc) {
		CLEVER_RETURN_EMPTY_STR();
	} else {
		CLEVER_RETURN_STR(CSTRINGT((*g_clever_argv)[i]));
	}
*/
}

// Int argc()
// Get argc value
static CLEVER_FUNCTION(argc)
{
	if (!clever_static_check_no_args()) {
		return;
	}
//	CLEVER_RETURN_INT(*g_clever_argc);
}

// Void sleep(Int time)
// Sleep for 'time' milliseconds
static CLEVER_FUNCTION(sleep)
{
	if (!clever_static_check_args("i")) {
		return;
	}

	int time = static_cast<int>(CLEVER_ARG_INT(0));

#ifdef CLEVER_WIN32
	SleepEx(time, false);
#else
	usleep(time * 1000);
#endif
}

// Int time()
// Returns the number of seconds since Unix Epoch
static CLEVER_FUNCTION(time)
{
	if (!clever_static_check_no_args()) {
		return;
	}

	result->setInt(time(NULL));
}

// Int clock()
// Returns the execution clock
static CLEVER_FUNCTION(clock)
{
	if (!clever_static_check_no_args()) {
		return;
	}

	result->setInt(clock());
}

} // clever::packages::std::os

// Initializes Standard module
CLEVER_MODULE_INIT(SYSModule)
{
	addFunction(new Function("system", &CLEVER_NS_FNAME(sys, system)));
	addFunction(new Function("putenv", &CLEVER_NS_FNAME(sys, putenv)));
	addFunction(new Function("getenv", &CLEVER_NS_FNAME(sys, getenv)));
	addFunction(new Function("getcwd", &CLEVER_NS_FNAME(sys, getcwd)));
	addFunction(new Function("argc",   &CLEVER_NS_FNAME(sys, argc)));
	addFunction(new Function("argv",   &CLEVER_NS_FNAME(sys, argv)));
	addFunction(new Function("sleep",  &CLEVER_NS_FNAME(sys, sleep)));
	addFunction(new Function("time",  &CLEVER_NS_FNAME(sys, time)));
	addFunction(new Function("clock", &CLEVER_NS_FNAME(sys, clock)));
}

}}} // clever::packages::std
