/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_VM_H
#define CLEVER_VM_H

#include <vector>
#include <stack>
#include <map>
#include "core/ir.h"
#include "core/cthread.h"
#include "core/codegen.h"

namespace clever {

// Helper macro for opcode handler declaration
#define VM_HANDLER_ARG IR& op
#define VM_HANDLER(name) CLEVER_FORCE_INLINE void VM::vm_##name(VM_HANDLER_ARG)
#define VM_HANDLER_D(name) void vm_##name(VM_HANDLER_ARG)

class Value;

typedef std::vector<std::pair<size_t, Value*> > FuncVars;

/*
/// Stackframe representation
struct StackFrame {
	size_t ret_addr;     // Return address
	Value* ret_val;      // Return value
	Scope* arg_vars;     // Function arguments

	StackFrame()
		: ret_addr(0), ret_val(NULL), arg_vars(NULL) {}
};
*/
class VM;

struct Thread {
	VM* vm_handler;
	CThread t_handler;
};

/// VM representation
class VM {
public:
	enum ErrorLevel {
		ERROR,
		WARNING
	};

	typedef std::vector<std::vector<Thread*> > ThreadPool;

	VM(IRVector& inst)
		: m_pc(0), m_is_main_thread(true), m_inst(inst),
		  m_const_env(NULL), m_temp_env(NULL),
		  m_call_stack(), m_call_args(),
		  m_thread_pool(), m_mutex(), f_mutex(NULL) {}
	~VM() {}

	void error(ErrorLevel, const char*) const;

	void setGlobalEnv(Environment* globals) { m_global_env = globals; }

	void setConstEnv(Environment* consts) { m_const_env = consts; }

	void setTempEnv(Environment* temps) { m_temp_env = temps; }

	void copy(const VM*);

	void setChild() { m_is_main_thread = false; }

	bool isChild() const { return !m_is_main_thread; }

	bool isMain() const { return m_is_main_thread; }

	size_t getPC() const { return m_pc; }

	void nextPC() { ++m_pc; }

	IRVector& getInst() const { return m_inst; }

	///// Helper to retrive a Value* from TypePool
	//const Type* getType(Operand&) const;

	/// Helper to retrive a Value* from ValuePool
	Value* getValue(Operand&) const;

	Mutex* getMutex() {
		return isChild() ? f_mutex : &m_mutex;
	}
	/// Start the VM execution
	void run();

	/// Wait threads
	void wait();

	/// Methods for dumping opcodes
#ifdef CLEVER_DEBUG
	void dumpOperand(Operand&) const;
	void dumpOpcodes() const;
#endif
private:
	/// VM program counter
	size_t m_pc;

	bool m_is_main_thread;

	/// Vector of instruction
	IRVector& m_inst;

	/// Constant
	Environment* m_const_env;

	/// Temporaries
	Environment* m_temp_env;

	/// Globals
	Environment* m_global_env;

	/// Stack frame
	CallStack m_call_stack;

	/// Call arguments
	std::vector<Value*> m_call_args;

	ThreadPool m_thread_pool;

	Mutex m_mutex;
	Mutex* f_mutex;

	std::stack<size_t> m_try_stack;

	DISALLOW_COPY_AND_ASSIGN(VM);
};

} // clever

#endif // CLEVER_VM_H
