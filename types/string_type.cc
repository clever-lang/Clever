/**
 * Clever programming language
 * Copyright (c) 2011 Clever Team
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include "compiler/cstring.h"
#include "types/type.h"
#include "types/string_type.h"
#include "types/typeutils.h"

namespace clever {

/**
 * String:replace()
 * Replace part of the string and returns the new one.
 */
CLEVER_TYPE_METHOD(String::replace) {
	CLEVER_CHECK_ARGS("String::replace", CLEVER_TYPE("String"), CLEVER_TYPE("String"), NULL);
	size_t needleLength, needlePos;
	std::string newString = value->toString();

	// Initial replace
	needlePos = newString.find(args->at(0)->getString(), 0);
	needleLength = args->at(0)->getString().length();

	do {
		// Do the replace
		newString = newString.replace(needlePos, needleLength, args->at(1)->getString());

		// Find the next one
		needlePos = newString.find(args->at(0)->getString(), 0);
		needleLength = args->at(0)->getString().length();
	} while (needlePos != std::string::npos);

	retval->setString(CSTRING(newString));
	retval->setType(Value::STRING);
}

/**
 * String::substring(Int, Int)
 * Retrieves a substring from the original one.
 */
CLEVER_TYPE_METHOD(String::substring) {
	CLEVER_CHECK_ARGS("String::substring", CLEVER_TYPE("Int"), CLEVER_TYPE("Int"), NULL);
	
	if (size_t(args->at(0)->getInteger()) >= value->toString().length()) {
		std::cerr << "Out of range: " << args->at(0)->getInteger() 
			<< " is after the end of the string." << std::endl;
		std::exit(1);
	}

	std::string substr = value->toString().substr(args->at(0)->getInteger(), args->at(1)->getInteger());
	retval->setString(CSTRING(substr));
	retval->setType(Value::STRING);
}

/**
 * String::toDouble()
 * Converts the string to a double, if possible.
 */
CLEVER_TYPE_METHOD(String::toDouble) {
	CLEVER_CHECK_ARGS("String::toDouble", NULL);
	
	double floatValue;
	std::stringstream stream(value->toString());

	if ((stream >> floatValue).fail()) {
		std::cerr << "\"" << value->toString() << "\" is not a valid float." << std::endl;
		std::exit(1);
	}

	retval->setDouble(floatValue);
	retval->setType(Value::DOUBLE);
}

/**
 * String::toInteger()
 * Converts the string to an integer, if possible.
 */
CLEVER_TYPE_METHOD(String::toInteger) {
	CLEVER_CHECK_ARGS("String::toInteger", NULL);
	
	int64_t integer;
	std::stringstream stream(value->toString());

	if ((stream >> integer).fail()) {
		std::cerr << "\"" << value->toString() << "\" is not a valid integer." << std::endl;
		std::exit(1);
	}

	retval->setInteger(integer);
	retval->setType(Value::INTEGER);
}

void String::init() {
	addMethod(new Method("replace", (MethodPtr)&String::replace));
	addMethod(new Method("substring", (MethodPtr)&String::substring));
	addMethod(new Method("toDouble", (MethodPtr)&String::toDouble));
	addMethod(new Method("toInteger", (MethodPtr)&String::toInteger));
}

DataValue* String::allocateValue() const {
	return NULL;
}

} // clever
