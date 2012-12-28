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

#ifdef CLEVER_DEBUG
#include <stdio.h>
#include "vm/opcode.h"
#endif
#include "compiler/cstring.h"
#include "compiler/scope.h"
#include "compiler/value.h"
#include "types/type.h"
#include "vm/vm.h"

namespace clever {

// VM initialization phase
inline void VM::init()
{
	// Opcode handler mapping
	m_handlers[OP_RETURN]   = &VM::ret;
	m_handlers[OP_ASSIGN]   = &VM::assignment;
	m_handlers[OP_ADD]      = &VM::add;
	m_handlers[OP_SUB]      = &VM::sub;
	m_handlers[OP_MUL]      = &VM::mul;
	m_handlers[OP_DIV]      = &VM::div;
	m_handlers[OP_MOD]      = &VM::sub;
	m_handlers[OP_PRINT]    = &VM::print;
	m_handlers[OP_HALT]     = &VM::halt;
	m_handlers[OP_JMP]      = &VM::jmp;
	m_handlers[OP_FCALL]    = &VM::fcall;
	m_handlers[OP_LEAVE]    = &VM::leave;
	m_handlers[OP_SEND_VAL] = &VM::send_val;
}

#ifdef CLEVER_DEBUG
void VM::dumpOpcodes() const
{
	const char *op_type[] = {
		"UNUSED", "FETCH_VAL", "FETCH_SCOPE", "JMP_ADDR"
	};

	for (size_t i = 0, j = m_inst.size(); i < j; ++i) {
		IR& ir = m_inst[i];
		::printf("[%03ld] %-12s | %3ld (%-9s) | %3ld (%-9s) | %p\n",
			i,
			get_opcode_name(ir.opcode),
			ir.op1, op_type[ir.op1_type],
			ir.op2, op_type[ir.op2_type],
			ir.result);
	}
}
#endif

// Return operation
VM_HANDLER(ret)
{
}

// Halt operation
VM_HANDLER(halt)
{
}

// JMP operation
VM_HANDLER(jmp)
{
	VM_GOTO(op.op1);
}

// Assignment operation
VM_HANDLER(assignment)
{
	Value* var = getValue(op.op1);
	Value* value = getValue(op.op2);

	var->copy(value);

	VM_NEXT();
}

// Math sum operation
VM_HANDLER(add)
{
	Value* lhs = getValue(op.op1);
	Value* rhs = getValue(op.op2);

	if (lhs->getType() == CLEVER_INT_TYPE
		&& rhs->getType() == CLEVER_INT_TYPE) {
		op.result->setInt(lhs->getInt() + rhs->getInt());
	}

	VM_NEXT();
}

// Math subtraction operation
VM_HANDLER(sub)
{
	Value* lhs = getValue(op.op1);
	Value* rhs = getValue(op.op2);

	if (lhs->getType() == CLEVER_INT_TYPE
		&& rhs->getType() == CLEVER_INT_TYPE) {
		op.result->setInt(lhs->getInt() - rhs->getInt());
	}

	VM_NEXT();
}

// Math multiplication operation
VM_HANDLER(mul)
{
	Value* lhs = getValue(op.op1);
	Value* rhs = getValue(op.op2);

	if (lhs->getType() == CLEVER_INT_TYPE
		&& rhs->getType() == CLEVER_INT_TYPE) {
		op.result->setInt(lhs->getInt() * rhs->getInt());
	}

	VM_NEXT();
}

// Math division operation
VM_HANDLER(div)
{
	Value* lhs = getValue(op.op1);
	Value* rhs = getValue(op.op2);

	if (lhs->getType() == CLEVER_INT_TYPE
		&& rhs->getType() == CLEVER_INT_TYPE) {
		op.result->setInt(lhs->getInt() / rhs->getInt());
	}

	VM_NEXT();
}

// Math modulus operation
VM_HANDLER(mod)
{
	Value* lhs = getValue(op.op1);
	Value* rhs = getValue(op.op2);

	if (lhs->getType() == CLEVER_INT_TYPE
		&& rhs->getType() == CLEVER_INT_TYPE) {
		op.result->setInt(lhs->getInt() % rhs->getInt());
	}

	VM_NEXT();
}

// Temporary handler for print command
VM_HANDLER(print)
{
	getValue(op.op1)->dump();
	std::cout << std::endl;

	VM_NEXT();
}

// Receives values to be used in the next function call
VM_HANDLER(send_val)
{
	m_call_args.push_back(getValue(op.op1));

	VM_NEXT();
}

// Function call operation
VM_HANDLER(fcall)
{
	Value* func = getValue(op.op1);
	FuncData* fdata = static_cast<FuncData*>(func->getObj());

	if (fdata->type == USER_FUNC) {
		// Pushs a new stack frame for the user function call on the call stack
		m_call_stack.push(StackFrame());

		// Sets the return address to the next instruction
		m_call_stack.top().ret_addr = m_pc + 1;

		if (fdata->arg_vars) {
			Scope* arg_scope = fdata->arg_vars;

			for (size_t i = 0, j = arg_scope->size(); i < j; ++i) {
				Value* arg_val = getValue(arg_scope->at(i).getValueId());
				arg_val->copy(m_call_args[i]);
			}

			m_call_args.clear();
		}

		VM_GOTO(fdata->u.addr);
	}
}

// Leave operation
VM_HANDLER(leave)
{
	StackFrame& frame = m_call_stack.top();

	m_call_stack.pop();

	// Go back to the next instruction after the caller
	VM_GOTO(frame.ret_addr);
}

// Executes the VM opcodes in a continuation-passing style
void VM::run()
{
	// Loads the opcode handlers
	init();

	// Starts the VM by running the first instruction, which keep calling
	// each other in a tail call way
	(this->*m_handlers[m_inst[0].opcode])(m_inst[0]);
}

} // clever
