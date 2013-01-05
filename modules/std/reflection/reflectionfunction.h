
#ifndef CLEVER_STD_REFLECTION_FUNCTION_H
#define CLEVER_STD_REFLECTION_FUNCTION_H

#include <iostream>
#include "core/cstring.h"
#include "types/type.h"

namespace clever { namespace packages { namespace std {

class ReflectionFunction : public Type {
public:
	ReflectionFunction()
		: Type(CSTRING("ReflectionFunction")) {}

	~ReflectionFunction() {}

	virtual void dump(const void*) const {}
	virtual void dump(const void*, ::std::ostream& out) const {}

	virtual void* allocData(CLEVER_TYPE_CTOR_ARGS) const;
	virtual void deallocData(void* data) {}

	virtual void increment(Value*) const {}
	virtual void decrement(Value*) const {}

	CLEVER_TYPE_VIRTUAL_METHOD_DECLARATIONS;
};

}}} // clever::packages::std

#endif // CLEVER_STD_REFLECTION_FUNCTION_H
