#pragma once

#include <functional>
#include <Windows.h>
#include "uncopyable.h"

#include <iostream>

#include "../define/zaku_namespace_def.h"

ZAKU_LIB_BEGIN


#pragma region Resource Finalizer

template<class T>
class ResourceFinalizer_Base
{
public:
	virtual void Finalize(T &value) = 0;
};

template<class T, class _Dx>
class ResourceFinalizer : public ResourceFinalizer_Base<T>
{	
public:
	ResourceFinalizer(_Dx finalizer)
		: m_finalizer(finalizer)
	{
	}

public:
	void Finalize(T &value) override
	{
		m_finalizer(value);
	}

private:
	_Dx m_finalizer;
};

#pragma endregion


#pragma region raii_wrapper

// *********************
// raii_wrapper
// *********************
template <class T>
class raii_wrapper
{
// *********************
// The Big-four and the half
// *********************
#pragma region The Big-four and the half
public:
	friend void swap(raii_wrapper<T>& first, raii_wrapper<T>& second)
	{
		using std::swap; 

		swap(first.m_value, second.m_value);
		swap(first.m_ResourceFinalizer, second.m_ResourceFinalizer); 
	}

	void finalize() noexcept
	{
		try
		{
			if (m_ResourceFinalizer != nullptr)
			{
				m_ResourceFinalizer->Finalize(m_value);
			}

			fill_memory_zero();
		}
		catch (...)
		{

		}
	}

	~raii_wrapper(void) noexcept
	{
		finalize();
	}

	raii_wrapper()
	{
		fill_memory_zero();
	};

	template <class _Dx>
	raii_wrapper(T value, _Dx finalizer)
		: m_value(value)
		, m_ResourceFinalizer(new ResourceFinalizer<T, _Dx>(finalizer))
	{

	}

	template <class _Ux, class _Dx, class _UxRet>
	raii_wrapper(_Ux initializer, _Dx finalizer, _UxRet *initResult)
		: m_ResourceFinalizer(new ResourceFinalizer<T, _Dx>(finalizer))
	{
		ZeroMemory(&m_value, sizeof(m_value));

		if (initResult)
		{
			*initResult = initializer(&m_value);
		}
		else
		{
			initializer(&m_value);
		}
	}

	raii_wrapper(raii_wrapper<T> &other)
	{
		fill_memory_zero();
		swap(*this, other);
	}

	raii_wrapper(raii_wrapper<T> &&other)
	{
		fill_memory_zero();
		swap(*this, other);
	}

	raii_wrapper<T>& operator=(raii_wrapper<T> &other)
	{
		swap(*this, other);

		return *this;
	}

	//raii_wrapper<T>& operator=(const T &otherValue)
	//{
	//	m_value = otherValue;

	//	return *this;
	//}

private:
	raii_wrapper<T>& operator=(raii_wrapper<T> &&other)
	{
		swap(*this, other);

		return *this;
	}

	raii_wrapper<T>& operator=(const T &&otherValue)
	{
		m_value = otherValue;

		return *this;
	}

	void fill_memory_zero()
	{
		ZeroMemory(&m_value, sizeof(m_value));
		m_ResourceFinalizer = nullptr;
	}

#pragma endregion


#pragma region Overloaded Operator
// *********************
// Overloaded Operator
// *********************
public:
	bool operator==(const T& value)
	{
		return m_value == value;
	}

	bool operator!=(const T& value)
	{
		return !(this->operator==(value));
	}

#pragma endregion


#pragma region Type Converter
// *********************
// Type converter
// *********************
public:
// 	operator const T&() const
// 	{
// 		return m_value;
// 	}

	const T& const_ref() const
	{
		return m_value;
	}

	T* pointer()
	{
		return &m_value;
	}

	T value_copy()
	{
		return m_value;
	}

#pragma endregion


#pragma region Others
// *********************
// Others
// *********************
public:
	template <class _Dx>
	void reset(T value, _Dx finalizer)
	{
		swap(*this, raii_wrapper(value, finalizer));
	}

	template <class _Ux, class _Dx, class _UxRet>
	void reset(_Ux initializer, _Dx finalizer, _UxRet *initResult)
	{
		swap(*this, raii_wrapper(initializer, finalizer, initResult));
	}
#pragma endregion


// *********************
// Members
// *********************
private:
	T m_value;

	ResourceFinalizer_Base<T> *m_ResourceFinalizer;
};

#pragma endregion


template <class T>
bool operator==(const raii_wrapper<T>& first, const raii_wrapper<T>& second)
{
	return first.const_ref() == second.const_ref();
}

template <class T>
bool operator!=(const raii_wrapper<T>& first, const raii_wrapper<T>& second)
{
	return !(first == second);
}


#pragma region Win32 tools

class raii_win32_tools
{
private:
	raii_win32_tools(void) {}
	~raii_win32_tools(void) {}

public:
	static void Win32DeleteCS(CRITICAL_SECTION &cs)
	{
		DeleteCriticalSection(&cs);
	}
};

#define ZAKU_RAII_CS raii_wrapper<CRITICAL_SECTION>
#define ZAKU_RAII_INIT_CS															\
	std::bind<BOOL>(&InitializeCriticalSectionAndSpinCount, placeholders::_1, 4000),	\
	std::bind<void>(&raii_win32_tools::Win32DeleteCS, placeholders::_1),				\
	(PBOOL)NULL																			\

#define ZAKU_RAII_HANDLE raii_wrapper<HANDLE>
#define ZAKU_RAII_HANDLE_FINALIZE					\
	std::bind<BOOL>(&CloseHandle, placeholders::_1)

ZAKU_LIB_END