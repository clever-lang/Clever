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

#ifndef CLEVER_PKGMANAGER_H
#define CLEVER_PKGMANAGER_H

#ifdef CLEVER_MSVC
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#include "compiler/module.h"
#include "compiler/scope.h"

namespace clever {

class CString;

typedef std::tr1::unordered_map<const CString*, Package*> PackageMap;
typedef std::pair<const CString*, Package*> PackagePair;

/**
 * Package manager representation
 */
class PackageManager {
public:
	PackageManager() { }

	~PackageManager() { }

	/**
	 * Load package map
	 */
	void init();
	/**
	 * Free memory
	 */
	void shutdown();
	/**
	 * Add a package to list
	 */
	void addPackage(const CString* const name, Package* package) {
		m_packages.insert(PackagePair(name, package));
	}
	/**
	 * Load an specific package
	 */
	void loadPackage(Scope*, const CString* const);
	/**
	 * Load an specific package module
	 */
	void loadModule(Scope*, const CString* const, const CString* const, const CString* const);
	void loadModule(Scope*, const CString* const, Module* const, const CString* const);

	const PackageMap& getPackages() const { return m_packages; }
private:
	PackageMap m_packages;

	DISALLOW_COPY_AND_ASSIGN(PackageManager);
};

extern PackageManager g_pkgmanager;

} // clever

#endif // CLEVER_PKGMANAGER_H
