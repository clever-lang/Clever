/**
 * Clever programming language
 * Copyright (c) 2011-2012 Clever Team
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CLEVER_SCOPE_H
#define CLEVER_SCOPE_H

#ifdef CLEVER_MSVC
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#include <vector>
#include "compiler/value.h"
#include "compiler/cstring.h"

namespace clever {

class CString;
class Value;

class Symbol {
public:
	Symbol(const CString* name, size_t value_id)
		: m_name(name), m_value_id(value_id) {}

	~Symbol() {}

	const CString* getName() const { return m_name; }

	size_t getValueId() const { return m_value_id; }
private:
	const CString* m_name;
	size_t m_value_id;
};

class Scope {
public:
	typedef std::vector<Scope*> ScopeVector;
	typedef std::vector<Symbol> SymbolMap;
	typedef std::tr1::unordered_map<const CString*, size_t> SymbolTable;
	typedef SymbolTable::value_type SymbolEntry;

	Scope()
		: m_parent(NULL), m_children(), m_symbols(), m_size(0), m_id(0) {}

	explicit Scope(Scope* parent)
		: m_parent(parent), m_children(), m_symbols(), m_size(0), m_id(0) {}

	~Scope() {
		ScopeVector::const_iterator it = m_children.begin(),
			end = m_children.end();

		while (it != end) {
			delete *it;
			++it;
		}
	}

	size_t push(const CString* name, size_t value_id) {
		m_symbols.push_back(Symbol(name, value_id));
		m_symbol_table.insert(SymbolEntry(name, m_size));
		return m_size++;
	}

	Symbol& at(size_t idx) { return m_symbols[idx]; }

	void setId(size_t id) { m_id = id; }
	size_t getId() const { return m_id; }

	size_t size() const { return m_size; }

	Scope* newLexicalScope() {
		Scope* s = new Scope(this);
		m_children.push_back(s);
		return s;
	}

	Scope* getParent() const { return m_parent; }

	Symbol* getLocalSymbol(const CString*);
	Symbol* getSymbol(const CString*);
private:
	Scope* m_parent;
	ScopeVector m_children;
	SymbolMap m_symbols;
	SymbolTable m_symbol_table;
	size_t m_size;
	size_t m_id;

	DISALLOW_COPY_AND_ASSIGN(Scope);
};

inline Symbol* Scope::getLocalSymbol(const CString* name) {
	SymbolTable::iterator it = m_symbol_table.find(name);

	if (it == m_symbol_table.end()) {
		return NULL;
	}
	return &m_symbols[it->second];
}

// Resolve a symbol name recursively
inline Symbol* Scope::getSymbol(const CString* name) {
	Symbol* sym = getLocalSymbol(name);

	if (sym != NULL) {
		return sym;
	}

	if (m_parent != NULL) {
		sym = m_parent->getSymbol(name);
	}

	return sym;
}

} // clever

#endif // CLEVER_SCOPE_H
