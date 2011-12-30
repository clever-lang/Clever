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
#include <cmath>
#include "compiler/cstring.h"
#include "types/typeutils.h"
#include "types/type.h"
#include "types/double.h"
#include "types/nativetypes.h"

namespace clever {

/**
 * Double::Double()
 * Constructs the object.
 */
CLEVER_TYPE_METHOD(Double::constructor) {
	if (args) {
		retval->setDouble(args->at(0)->getDouble());
	}
	else {
		retval->setDouble(0.0);
	}
}

/**
 * Double::toString()
 * Converts the number to string
 */
CLEVER_TYPE_METHOD(Double::toString) {
	retval->setString(CSTRING(value->toString()));
}

/**
 * Double::sqrt()
 * Returns the square root of the number
 */
CLEVER_TYPE_METHOD(Double::sqrt) {
	retval->setDouble(std::sqrt(value->getDouble()));
}

void Double::init() {
	addMethod(new Method(CLEVER_CTOR_NAME, (MethodPtr)&Double::constructor, CLEVER_DOUBLE));
		
	addMethod(
		(new Method(CLEVER_CTOR_NAME, (MethodPtr)&Double::constructor, CLEVER_DOUBLE))
			->addArg("value", CLEVER_DOUBLE)
	);

	addMethod(new Method("toString", (MethodPtr)&Double::toString, CLEVER_STR));
		
	addMethod(new Method("sqrt", (MethodPtr)&Double::sqrt, CLEVER_DOUBLE));
}

DataValue* Double::allocateValue() const {
	return NULL;
}

} // clever
