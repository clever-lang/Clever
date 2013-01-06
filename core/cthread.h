/**
 * Clever programming language
 * Copyright (c) Clever Team
 *
 * This file is distributed under the MIT license. See LICENSE for details.
 */

#ifndef CLEVER_CTHREAD_H
#define CLEVER_CTHREAD_H

#ifndef CLEVER_WIN32
# include <pthread.h>
#else
# include <win32/win32.h>
#endif

namespace clever {

class Mutex {
public:
	Mutex();

	~Mutex();

	void lock();
	void unlock();

private:
#ifndef CLEVER_WIN32
	pthread_mutex_t m_mut;
#else
	HANDLE m_mut;
#endif
};

#ifndef CLEVER_WIN32
# define CLEVER_THREAD_FUNC(name) static void* name(void *arg)
typedef void* (*ThreadFunc)(void*);
#else
typedef DWORD (*ThreadFunc)(LPVOID);
# define CLEVER_THREAD_FUNC(name) static DWORD name(void *arg)
#endif

class CThread {
public:
	CThread() {}

	~CThread() {}

	void create(ThreadFunc thread_func, void* args);

	int wait();

private:
#ifndef CLEVER_WIN32
	pthread_t t_handler;
#else
	HANDLE t_handler;
#endif
};

} // clever

#endif // CLEVER_CTHREAD_H
