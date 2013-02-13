/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_USER_H
#define CLEVER_USER_H

#include "core/module.h"
#include "types/type.h"
#include "core/cstring.h"

namespace clever {

class Environment;

// User object representation
class UserObject : public TypeObject {
public:
	UserObject()
		: m_env(NULL) {}

	~UserObject() {}

	void setEnvironment(Environment* env) { m_env = env; }
	Environment* getEnvironment() const { return m_env; }
private:
	Environment* m_env;

	DISALLOW_COPY_AND_ASSIGN(UserObject);
};

// User type representation
class UserType : public Type {
public:
	UserType(const CString* name)
		: Type(*name, USER_TYPE), m_env(NULL) {}

	~UserType() {}

	void init(CLEVER_TYPE_INIT_ARGS) {
		setConstructor((MethodPtr) &UserType::ctor);
	}

	void setEnvironment(Environment* env) { m_env = env; }
	Environment* getEnvironment() const { return m_env; }

	TypeObject* allocData(CLEVER_TYPE_CTOR_ARGS) const { return new UserObject; }

	void deallocData(CLEVER_TYPE_DTOR_ARGS) { delete static_cast<UserObject*>(data); }

	virtual void dump(TypeObject*, std::ostream& out) const { out << getName(); }

	CLEVER_METHOD(ctor) {
		result->setObj(this, allocData(&args));
	}
private:
	Environment* m_env;

	DISALLOW_COPY_AND_ASSIGN(UserType);
};

// User module representation
class UserModule: public Module {
public:
	UserModule()
		: Module("_user") {}

	~UserModule() {}

	void init() {}
private:
	DISALLOW_COPY_AND_ASSIGN(UserModule);
};

} // clever

#endif // CLEVER_USER_H
