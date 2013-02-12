/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_DB_MYSQL_H
#define CLEVER_DB_MYSQL_H

#include <iostream>
#include "core/cstring.h"
#include "core/value.h"
#include "types/type.h"

#include <mysql/mysql.h>

namespace clever { namespace modules { namespace db {

class MysqlObject : public TypeObject {
public:
	MysqlObject() {}

	~MysqlObject() {}

private:

	DISALLOW_COPY_AND_ASSIGN(MysqlObject);
};

class Mysql : public Type {
public:
	Mysql()
		: Type("Mysql") {}

	~Mysql() {}

	void init();

	TypeObject* allocData(CLEVER_TYPE_CTOR_ARGS) const;
	void deallocData(void*);

	void dump(TypeObject*, std::ostream&) const;

	CLEVER_METHOD(ctor);
	CLEVER_METHOD(connect);

private:
	DISALLOW_COPY_AND_ASSIGN(Mysql);
};

}}} // clever::modules::db

#endif // CLEVER_DB_MYSQL_H
