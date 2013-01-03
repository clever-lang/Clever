/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_SCOPE_H
#define CLEVER_SCOPE_H

#ifdef CLEVER_MSVC
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#include <vector>
#include <algorithm>
#include "core/value.h"

namespace clever {

class Value;
class Scope;

typedef std::vector<Type*> TypePool;
typedef std::vector<Value*> ValuePool;

/// Symbol representation
struct Symbol {
	enum SymbolType { TYPE, VAR };

	Symbol() {}
	Symbol(SymbolType type, const CString *name_, size_t value_id_, Scope *scope_ = NULL)
		: m_type(type), name(name_), value_id(value_id_), scope(scope_) {}

	~Symbol() {}

	bool isType() const { return m_type == TYPE; }
	bool isVar() const { return m_type == VAR; }

	SymbolType m_type;
	const CString* name;
	size_t value_id;
	const Scope *scope;
};

/// Scope representation
class Scope {
public:
	typedef std::vector<Scope*> ScopeVector;
	typedef std::vector<Symbol*> SymbolMap;
	typedef std::tr1::unordered_map<const CString*, size_t> SymbolTable;
	typedef SymbolTable::value_type SymbolEntry;

	Scope()
		: m_parent(NULL), m_children(), m_symbols(), m_size(0), m_id(0),
			m_value_id(0), m_type_id(0), m_value_pool() {}

	explicit Scope(Scope* parent)
		: m_parent(parent), m_children(), m_symbols(), m_size(0), m_id(0),
			m_value_id(0), m_type_id(0), m_value_pool() {}

	~Scope() {
		ValuePool::const_iterator itv = m_value_pool.begin(),
			endv = m_value_pool.end();

		while (itv != endv) {
			CLEVER_SAFE_DELREF(*itv);
			++itv;
		}

		ScopeVector::const_iterator it = m_children.begin(),
			end = m_children.end();

		while (it != end) {
			delete *it;
			++it;
		}

		SymbolMap::const_iterator its = m_symbols.begin(),
			ends = m_symbols.end();

		while (its != ends) {
			delete *its;
			++its;
		}

		TypePool::const_iterator itt = m_type_pool.begin(),
			endt = m_type_pool.end();

		while (itt != endt) {
			delete *itt;
			++itt;
		}
	}

	size_t pushType(const CString* name, Type* type) {
		m_symbols.push_back(new Symbol(Symbol::TYPE, name, m_type_id, this));
		m_symbol_table.insert(SymbolEntry(name, m_size++));
		m_type_pool.push_back(type);

		return m_type_id++;
	}

	size_t pushValue(const CString* name, Value* value) {
		m_symbols.push_back(new Symbol(Symbol::VAR, name, m_value_id, this));
		m_symbol_table.insert(SymbolEntry(name, m_size++));
		m_value_pool.push_back(value);

		return m_value_id++;
	}

	Value* getValue(size_t idx) const { return m_value_pool[idx]; }

	Symbol& at(size_t idx) const { return *m_symbols[idx]; }

	void setId(size_t id) { m_id = id; }

	size_t getId() const { return m_id; }

	size_t size() const { return m_size; }

	void copy(Scope* s) {
		for (size_t id = 0; id < s->m_value_pool.size(); ++id) {
			Value* u = s->m_value_pool[id];
			Value* v = new Value;

			m_value_pool.push_back(v);
			v->copy(u);
		}
	}

	Scope* enter() {
		Scope* s = new Scope(this);
		m_children.push_back(s);
		return s;
	}

	Scope* leave() {
		return m_parent;
	}

	std::vector<Scope*> flatten() {
		std::vector<Scope*> scopes;
		std::vector<Scope*>::iterator it(m_children.begin()), end(m_children.end());

		scopes.push_back(this);

		for (; it != end; it++) {
			std::vector<Scope*> flattened = (*it)->flatten();
			scopes.insert(scopes.end(), flattened.begin(), flattened.end());
		}

		return scopes;
	}

	Scope* getParent() const { return m_parent; }

	Symbol* getLocal(const CString*);
	Symbol* getAny(const CString*);
private:
	Scope* m_parent;
	ScopeVector m_children;
	SymbolMap m_symbols;
	SymbolTable m_symbol_table;
	size_t m_size;
	size_t m_id;
	size_t m_value_id;
	size_t m_type_id;

	ValuePool m_value_pool;
	TypePool m_type_pool;

	DISALLOW_COPY_AND_ASSIGN(Scope);
};

inline Symbol* Scope::getLocal(const CString* name) {
	SymbolTable::iterator it = m_symbol_table.find(name);

	if (it == m_symbol_table.end()) {
		return NULL;
	}
	return m_symbols[it->second];
}

/// Resolve a symbol name recursively
inline Symbol* Scope::getAny(const CString* name) {
	Symbol* sym = getLocal(name);

	if (sym != NULL) {
		return sym;
	}

	Scope* v = m_parent;
	while (v != NULL) {
		sym = v->getLocal(name);

		if (sym != NULL) {
			return sym;
		}

		v = v->m_parent;
	}

	return sym;
}

} // clever

#endif // CLEVER_SCOPE_H
