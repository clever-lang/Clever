/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_STD_DATE_DATE_H
#define CLEVER_STD_DATE_DATE_H

#include "core/cstring.h"
#include "types/type.h"

namespace clever { namespace modules { namespace std {

struct DateObject : public TypeObject {
	DateObject()
		: intern(new time_t) {}

	~DateObject() {
		delete intern;
	}

	time_t* intern;
};

class Date : public Type {
public:
	Date()
		: Type(CSTRING("std.date")) {}

	~Date() {}

	void dump(const void* data) const;
	void dump(const void* data, ::std::ostream& out) const;

	virtual void increment(Value*, const VM*, CException*) const {}
	virtual void decrement(Value*, const VM*, CException*) const {}

	void init();

	virtual TypeObject* allocData(CLEVER_TYPE_CTOR_ARGS) const;
	virtual void deallocData(void* data);

	CLEVER_METHOD(ctor);
	CLEVER_METHOD(format);
	CLEVER_METHOD(uformat);
	CLEVER_METHOD(getTime);
};

}}} // clever::modules::std

#endif // CLEVER_STD_DATE_DATE_H
