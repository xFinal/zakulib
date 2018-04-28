//***************************************************************
// This thread_wrapper class must be newed then put into a shared_ptr.
// Example: shared_ptr<IOL_Thread> thread_wrapper(new IOL_Thread(...))

#pragma once

#include "uncopyable.h"
#include <functional>
#include <memory>
#include <assert.h>

#include "../define/zaku_namespace_def.h"

ZAKU_LIB_BEGIN


class thread_wrapper : public std::enable_shared_from_this<thread_wrapper>,
	private uncopyable
{
	typedef std::function<void()> ThreadCallback;

public:
	thread_wrapper(ThreadCallback threadCallback)
		: m_threadCallback(threadCallback)
		, m_handle(NULL)
	{

	}

	~thread_wrapper(void)
	{
		CloseHandle(m_handle);
		m_handle = NULL;
	}

public:
	bool Run()
	{
 		// Make this object remain in memory
 		m_thisPointer = shared_from_this();

		bool result = false;

		m_handle = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
		if (m_handle != NULL)
		{
			result = true;
		}

		return result;
	}

	void Terminate()
	{
		TerminateThread(m_handle, 0);
	}

private:
	static DWORD WINAPI ThreadFunc(LPVOID param)
	{
		thread_wrapper *thread = reinterpret_cast<thread_wrapper*>(param);
		assert(thread != nullptr);

		// Make the guarantee that this object will be released
		// even exceptions happen in RunThread().
		std::shared_ptr<thread_wrapper> thisPointer = thread->shared_from_this();
		thread->m_thisPointer.reset();

		thread->RunThread();
		
		return 0;
	}

	void RunThread()
	{
		if (m_threadCallback != nullptr)
		{
			m_threadCallback();
		}
	}

private:
	ThreadCallback m_threadCallback;
	std::shared_ptr<thread_wrapper> m_thisPointer;

	HANDLE m_handle;
};

ZAKU_LIB_END