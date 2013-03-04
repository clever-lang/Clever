/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_ASTVISITOR_H
#define CLEVER_ASTVISITOR_H

#include "core/ast.h"

namespace clever { namespace ast {

class Visitor {
public:
	Visitor() {}
	virtual ~Visitor() {}

	virtual void visit(Node* node) {}

	virtual void visit(NodeArray* node);
	virtual void visit(Block* node);
	virtual void visit(CriticalBlock* node);
	virtual void visit(Assignment* node);
	virtual void visit(VariableDecl* node);
	virtual void visit(Arithmetic* node);
	virtual void visit(FunctionDecl* node);
	virtual void visit(FunctionCall* node);
	virtual void visit(MethodCall* node);
	virtual void visit(IncDec* node);

	virtual void visit(While* node);
	virtual void visit(For* node);
	virtual void visit(If* node);

	virtual void visit(IntLit* node) {}
	virtual void visit(DoubleLit* node) {}
	virtual void visit(StringLit* node) {}
	virtual void visit(Ident* node) {}
	virtual void visit(Type* node) {}
	virtual void visit(NullLit*) {}
	virtual void visit(TrueLit*) {}
	virtual void visit(FalseLit*) {}
	virtual void visit(Array*) {}
	virtual void visit(Break*) {}
	virtual void visit(Continue*) {}

	virtual void visit(Return* node);
	virtual void visit(Logic* node);
	virtual void visit(Bitwise* node);
	virtual void visit(Boolean* node);
	virtual void visit(Comparison* node);
	virtual void visit(Import* node);
	virtual void visit(Instantiation* node);
	virtual void visit(Property* node);
	virtual void visit(Try* node);
	virtual void visit(Catch* node);
	virtual void visit(Throw* node);

	virtual void visit(AttrDecl* node);
	virtual void visit(ClassDef* node);
	virtual void visit(Subscript* node);
	virtual void visit(Switch* node);
private:
	DISALLOW_COPY_AND_ASSIGN(Visitor);
};

}} // clever::ast

#endif // CLEVER_ASTVISITOR_H
