/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_DB_PACKAGE_H
#define CLEVER_DB_PACKAGE_H

#include "core/clever.h"
#include "modules/db/db_forwarder.h"
#include "core/module.h"

namespace clever { namespace modules {

class Db : public Module {
public:
	Db()
		: Module("db") {}

	~Db() {}

	CLEVER_MODULE_VIRTUAL_METHODS_DECLARATION;
private:
	DISALLOW_COPY_AND_ASSIGN(Db);
};

}} // clever::modules

#endif // CLEVER_DB_PACKAGE_H
