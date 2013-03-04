/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_AST_DUMP_H
#define CLEVER_AST_DUMP_H

#include <iostream>
#include "core/astvisitor.h"

namespace clever { namespace ast {

#define AST_DUMP_DISPLAY_LHS_RHS(op)                           \
		std::cout << " (" << op << ')' << std::endl;           \
		m_ws = std::string(++m_level, ' ');                    \
		node->getLhs()->accept(*this);                         \
		if (node->getRhs()) { node->getRhs()->accept(*this); } \
		m_ws = std::string(--m_level, ' ');

#define AST_DUMP_DISPLAY_BLOCK              \
		m_ws = std::string(++m_level, ' '); \
		node->getBlock()->accept(*this);    \
		m_ws = std::string(--m_level, ' ');

class Dumper: public Visitor {
public:
	Dumper()
		: m_level(0) {}

	~Dumper() {}

	void visit(Node* node)         { std::cout << m_ws << "Node" << std::endl;         }
	void visit(NodeArray* node)    { std::cout << m_ws << "NodeArray" << std::endl;    }
	void visit(FunctionCall* node) {
		std::cout << m_ws << "FunctionCall" << std::endl;
		m_ws = std::string(++m_level, ' ');
		node->getCallee()->accept(*this);

		if (node->hasArgs()) {
			node->getArgs()->accept(*this);
		}

		m_ws = std::string(--m_level, ' ');

	}
	void visit(MethodCall* node)   { std::cout << m_ws << "MethodCall" << std::endl;   }
	void visit(If* node)           { std::cout << m_ws << "If" << std::endl;           }

	void visit(IntLit* node) {
		std::cout << m_ws << "IntLit (" << node->getValue() << ")" << std::endl;
	}

	void visit(DoubleLit* node) {
		std::cout << m_ws << "DoubleLit (" << node->getValue() << ")" << std::endl;
	}

	void visit(StringLit* node) {
		std::cout << m_ws << "StringLit (" << node->getValue()->substr(0, 7);

		if (node->getValue()->length() > 7) {
			std::cout << "...";
		}

		std::cout << ")" << std::endl;
	}

	void visit(Return* node)       { std::cout << m_ws << "Return" << std::endl;       }
	void visit(Import* node)       { std::cout << m_ws << "Import" << std::endl;       }
	void visit(Instantiation* node){ std::cout << m_ws << "Instantiation" << std::endl;}
	void visit(Throw* node)        { std::cout << m_ws << "Throw" << std::endl;        }
	void visit(NullLit* node)      { std::cout << m_ws << "NullLit" << std::endl;      }
	void visit(TrueLit* node)      { std::cout << m_ws << "TrueLit" << std::endl;      }
	void visit(FalseLit* node)     { std::cout << m_ws << "FalseLit" << std::endl;     }
	void visit(Array* node)        { std::cout << m_ws << "Array" << std::endl;        }
	void visit(Switch* node)       { std::cout << m_ws << "Switch" << std::endl;       }

	void visit(Try* node) {
		std::cout << m_ws << "Try" << std::endl;

		AST_DUMP_DISPLAY_BLOCK;

		node->getCatches()->accept(*this);

		if (node->hasFinally()) {
			node->getFinally()->accept(*this);
		}
	}

	void visit(Catch* node) {
		std::cout << m_ws << "Catch" << std::endl;

		AST_DUMP_DISPLAY_BLOCK;
	}

	void visit(While* node) {
		std::cout << m_ws << "While" << std::endl;

		AST_DUMP_DISPLAY_BLOCK;
	}

	void visit(For* node) {
		std::cout << m_ws << "For" << std::endl;

		AST_DUMP_DISPLAY_BLOCK;
	}

	void visit(FunctionDecl* node) {
		std::cout << m_ws << "FunctionDecl" << std::endl;

		AST_DUMP_DISPLAY_BLOCK;
	}

	void visit(Ident* node) {
		std::cout << m_ws << "Ident (" << *node->getName() << ")" << std::endl;
	}

	void visit(Assignment* node) {
		std::cout << m_ws << "Assignment";
		AST_DUMP_DISPLAY_LHS_RHS('=');
	}

	void visit(Arithmetic* node) {
		std::cout << m_ws << "Arithmetic";
		AST_DUMP_DISPLAY_LHS_RHS((char)node->getOperator());
	}

	void visit(Logic* node) {
		std::cout << m_ws << "Logic";
		switch (node->getOperator()) {
			case ast::Logic::LOP_AND:     AST_DUMP_DISPLAY_LHS_RHS("&&"); break;
			case ast::Logic::LOP_OR:      AST_DUMP_DISPLAY_LHS_RHS("||"); break;
		}
	}

	void visit(Comparison* node) {
		std::cout << m_ws << "Logic";
		switch (node->getOperator()) {
			case ast::Comparison::COP_EQUAL:   AST_DUMP_DISPLAY_LHS_RHS("=="); break;
			case ast::Comparison::COP_NEQUAL:  AST_DUMP_DISPLAY_LHS_RHS("!="); break;
			case ast::Comparison::COP_GREATER: AST_DUMP_DISPLAY_LHS_RHS(">");  break;
			case ast::Comparison::COP_GEQUAL:  AST_DUMP_DISPLAY_LHS_RHS(">="); break;
			case ast::Comparison::COP_LESS:    AST_DUMP_DISPLAY_LHS_RHS("<");  break;
			case ast::Comparison::COP_LEQUAL:  AST_DUMP_DISPLAY_LHS_RHS("<="); break;
		}
	}

	void visit(Bitwise* node) {
		std::cout << m_ws << "Bitwise";
		switch (node->getOperator()) {
			case ast::Bitwise::BOP_AND:	   AST_DUMP_DISPLAY_LHS_RHS('&');  break;
			case ast::Bitwise::BOP_OR:     AST_DUMP_DISPLAY_LHS_RHS('|');  break;
			case ast::Bitwise::BOP_XOR:    AST_DUMP_DISPLAY_LHS_RHS('^');  break;
			case ast::Bitwise::BOP_NOT:    AST_DUMP_DISPLAY_LHS_RHS('~');  break;
			case ast::Bitwise::BOP_LSHIFT: AST_DUMP_DISPLAY_LHS_RHS("<<"); break;
			case ast::Bitwise::BOP_RSHIFT: AST_DUMP_DISPLAY_LHS_RHS(">>"); break;
		}
	}

	void visit(Block* node) {
		std::cout << m_ws << "Block" << std::endl;

		m_ws = std::string(++m_level, ' ');

		std::vector<Node*> nodes = node->getNodes();
		std::vector<Node*>::const_iterator it = nodes.begin(), end = nodes.end();
		while (it != end) {
			(*it)->accept(*this);
			++it;
		}

		m_ws = std::string(--m_level, ' ');
	}
	void visit(VariableDecl* node) {
		std::cout << m_ws << "VariableDecl" << std::endl;

		m_ws = std::string(++m_level, ' ');

		node->getIdent()->accept(*this);

		if (node->hasAssignment()) {
			node->getAssignment()->accept(*this);
		}

		m_ws = std::string(--m_level, ' ');
	}
private:
	size_t m_level;
	std::string m_ws;

	DISALLOW_COPY_AND_ASSIGN(Dumper);
};

}} // clever::ast

#endif // CLEVER_AST_DUMP_H
