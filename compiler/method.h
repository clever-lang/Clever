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

#ifndef CLEVER_METHOD_H
#define CLEVER_METHOD_H

#include <string>
#include <vector>
#include "global.h"

namespace clever {

class Value;
class Type;
class Method;

typedef std::vector<Value*> ValueVector;
typedef std::vector<const Type*> TypeVector;

#define CLEVER_METHOD_ARGS const ValueVector* args, Value* retval, Value* value, Method* clv_method_
#define CLEVER_METHOD(name) void CLEVER_FASTCALL name(CLEVER_METHOD_ARGS) throw()

typedef void (CLEVER_FASTCALL *MethodPtr)(CLEVER_METHOD_ARGS);

/**
 * Method representation
 */
class Method {
public:
	enum MethodType { INTERNAL, USER };

	Method(std::string name, MethodPtr ptr)
		: m_name(name), m_type(INTERNAL), m_args(NULL), m_is_initialized(false) { m_info.ptr = ptr; }

	~Method() { if (m_args) delete m_args; }

	const std::string& getName() const throw() { return m_name; }
	MethodPtr getPtr() const throw() { return m_info.ptr; }

	void setInternal() throw() { m_type = INTERNAL; }
	void setUserDefined() throw() { m_type = USER; }

	bool isUserDefined() const throw() { return m_type == USER; }
	bool isInternal() const throw() { return m_type == INTERNAL; }

	void call(const ValueVector* args, Value* result, Value* context) const throw() {
		m_info.ptr(args, result, context, const_cast<Method*>(this));
	}

	long call() const throw() { return m_info.offset; }
	
	const TypeVector* getArgs() const throw() { return m_args; }
	void setArgs(const TypeVector* const args) { m_args = args; m_is_initialized = true; }
	
	/**
	 * Checks if the m_args has been initialized
	 */
	bool isArgsInitialized() const { return m_is_initialized; }
	
private:
	union {
		MethodPtr ptr;
		long offset;
	} m_info;

	std::string m_name;
	MethodType m_type;
	
	/**
	 * Method arguments. (Lazy) 
	 * Check with isArgsInitialized() before using it.
	 */
	const TypeVector* m_args;
	
	/**
	 * True if the m_args pointer is initialized
	 */
	bool m_is_initialized;
};

} // clever

#endif /* CLEVER_METHOD_H */
