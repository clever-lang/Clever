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
		Type(CSTRING("String")) { }

	void init();
	DataValue* allocateValue() const;

	/**
	 * Type methods
	 */
	static CLEVER_TYPE_METHOD(constructor);
	static CLEVER_TYPE_METHOD(ltrim);
	static CLEVER_TYPE_METHOD(rtrim);
	static CLEVER_TYPE_METHOD(trim);
	static CLEVER_TYPE_METHOD(replace);
	static CLEVER_TYPE_METHOD(substring);
	// static CLEVER_TYPE_METHOD(capitalize);
	// static CLEVER_TYPE_METHOD(split);
	static CLEVER_TYPE_METHOD(at);
	static CLEVER_TYPE_METHOD(length);
	static CLEVER_TYPE_METHOD(toDouble);
	static CLEVER_TYPE_METHOD(toInteger);
	static CLEVER_TYPE_METHOD(toUpper);
	static CLEVER_TYPE_METHOD(toLower);

	/**
	 * Type operator methods
	 */
	static CLEVER_TYPE_METHOD(plus);
	static CLEVER_TYPE_METHOD(not_equal);
	static CLEVER_TYPE_METHOD(equal);
	static CLEVER_TYPE_METHOD(less);
	static CLEVER_TYPE_METHOD(greater);
	static CLEVER_TYPE_METHOD(less_equal);
	static CLEVER_TYPE_METHOD(greater_equal);

	/**
	 * Type handlers
	 */
	CLEVER_TYPE_ASSIGN_HANDLER_D { value->copy(newvalue); }
private:
	DISALLOW_COPY_AND_ASSIGN(String);
};

} // clever

#endif // CLEVER_STRING_H
