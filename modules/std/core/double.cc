/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#include <math.h>
#include "modules/std/core/double.h"
#include "modules/std/core/function.h"

namespace clever {

CLEVER_TYPE_OPERATOR(DoubleType::add)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setDouble(lhs->getDouble() + rhs->getDouble());
	}
}

CLEVER_TYPE_OPERATOR(DoubleType::sub)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setDouble(lhs->getDouble() - rhs->getDouble());
	}
}

CLEVER_TYPE_OPERATOR(DoubleType::mul)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setDouble(lhs->getDouble() * rhs->getDouble());
	}
}

CLEVER_TYPE_OPERATOR(DoubleType::div)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setDouble(lhs->getDouble() / rhs->getDouble());
	}
}
CLEVER_TYPE_OPERATOR(DoubleType::mod)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setDouble(fmod(lhs->getDouble(), rhs->getDouble()));
	}
}


CLEVER_TYPE_OPERATOR(DoubleType::greater)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setBool(lhs->getDouble() > rhs->getDouble());
	}
}

CLEVER_TYPE_OPERATOR(DoubleType::greater_equal)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setBool(lhs->getDouble() >= rhs->getDouble());
	}
}

CLEVER_TYPE_OPERATOR(DoubleType::less)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setBool(lhs->getDouble() < rhs->getDouble());
	}
}

CLEVER_TYPE_OPERATOR(DoubleType::less_equal)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setBool(lhs->getDouble() <= rhs->getDouble());
	}
}

CLEVER_TYPE_OPERATOR(DoubleType::equal)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setBool(lhs->getDouble() == rhs->getDouble());
	}
}

CLEVER_TYPE_OPERATOR(DoubleType::not_equal)
{
	if (EXPECTED(rhs->isDouble())) {
		result->setBool(lhs->getDouble() != rhs->getDouble());
	}
}

CLEVER_TYPE_UNARY_OPERATOR(DoubleType::not_op)
{
	result->setBool(!lhs->asBool());
}

// Double::Double()
CLEVER_METHOD(DoubleType::ctor)
{
	result->setDouble(0.0);
}

CLEVER_TYPE_INIT(DoubleType::init)
{
	setConstructor((MethodPtr) &DoubleType::ctor);
}

} // clever
