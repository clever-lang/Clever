/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#include "core/value.h"
#include "types/native_types.h"
#include "modules/db/mysql/mysql.h"
#include "modules/db/mysql/cmysql.h"


namespace clever { namespace modules { namespace db {

// Simple constructor for now
// TODO: Build a constructor that can accept the host, port, user and password
TypeObject* Mysql::allocData(CLEVER_TYPE_CTOR_ARGS) const
{
	MysqlObject* obj = new MysqlObject();

	return obj;
}

void Mysql::deallocData(void *data)
{
	MysqlObject* intern = static_cast<MysqlObject*>(data);

	if (intern) {
		delete intern;
	}
}

void Mysql::dump(TypeObject* data, ::std::ostream& out) const
{
	MysqlObject* uvalue = static_cast<MysqlObject*>(data);
	std::string str = uvalue->getMysql().dump();

	if (uvalue) {
		out << str;
	}
}

CLEVER_METHOD(Mysql::ctor)
{
	result->setObj(this, allocData(&args));
}

CLEVER_METHOD(Mysql::connect)
{
	bool ret = false;
	MysqlObject* mo = CLEVER_GET_OBJECT(MysqlObject*, CLEVER_THIS());
	CMysql& cmysql = mo->getMysql();

	if(!clever_check_args("ssss|i")) {
		return;
	}

	switch(args.size()) {
		case 5:
			cmysql.setPort(args[4]->getInt());
		case 4:
			cmysql.setHost(*args[0]->getStr());
			cmysql.setUser(*args[1]->getStr());
			cmysql.setPasswd(*args[2]->getStr());
			cmysql.setDb(*args[3]->getStr());

			ret = cmysql.connect();

			break;
	}

	result->setBool(ret);

}

CLEVER_METHOD(Mysql::query)
{

	int ret;

	MysqlObject* mo = CLEVER_GET_OBJECT(MysqlObject*, CLEVER_THIS());
	CMysql& cmysql = mo->getMysql();

	if(!clever_check_args("s")) {
		return;
	}


	ret = cmysql.query(args[0]->getStr()->c_str());

	result->setInt(ret);
}

CLEVER_METHOD(Mysql::fetchArray)
{
	
}

// Type initialization
CLEVER_TYPE_INIT(Mysql::init)
{
	setConstructor((MethodPtr) &Mysql::ctor);

	addMethod(new Function("connect",    (MethodPtr) &Mysql::connect));
	addMethod(new Function("query",      (MethodPtr) &Mysql::query));
	addMethod(new Function("fetchArray", (MethodPtr) &Mysql::fetchArray));
}

}}} // clever::modules::db
