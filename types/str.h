/**
 * Clever programming language
 * Copyright (c) 2011-2012 Clever Team
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

#ifndef CLEVER_STRING_H
#define CLEVER_STRING_H

#include "type.h"
#include "compiler/value.h"

namespace clever {

class String : public Type {
public:
	String() :
		Type(CSTRING("String"), CLEVER_OBJECT) { }

	void init();
	DataValue* allocateValue() const;

	/**
	 * Type methods
	 */
	static CLEVER_METHOD(constructor);
	static CLEVER_METHOD(ltrim);
	static CLEVER_METHOD(rtrim);
	static CLEVER_METHOD(trim);
	static CLEVER_METHOD(replace);
	static CLEVER_METHOD(substring);
	// static CLEVER_METHOD(capitalize);
	// static CLEVER_METHOD(split);
	static CLEVER_METHOD(at);
	static CLEVER_METHOD(length);
	static CLEVER_METHOD(toDouble);
	static CLEVER_METHOD(toInteger);
	static CLEVER_METHOD(toUpper);
	static CLEVER_METHOD(toLower);
	static CLEVER_METHOD(startsWith);
	static CLEVER_METHOD(endsWith);
	static CLEVER_METHOD(find);
	static CLEVER_METHOD(toByteArray);
	static CLEVER_METHOD(pad);
	static CLEVER_METHOD(padLeft);
	static CLEVER_METHOD(padRight);
	static CLEVER_METHOD(split);

	/**
	 * Type operator methods
	 */
	static CLEVER_METHOD(plus);
	static CLEVER_METHOD(not_equal);
	static CLEVER_METHOD(equal);
	static CLEVER_METHOD(less);
	static CLEVER_METHOD(greater);
	static CLEVER_METHOD(less_equal);
	static CLEVER_METHOD(greater_equal);
	static CLEVER_METHOD(do_assign);
	static CLEVER_METHOD(times);
private:
	DISALLOW_COPY_AND_ASSIGN(String);
};

} // clever

#endif // CLEVER_STRING_H
