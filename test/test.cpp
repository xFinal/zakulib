// test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../define/zaku_namespace_def.h"
#include "../utility/singleton.h"
#include "../utility/singleton_interface_base.h"
#include "../utility/interface_base.h"
#include "../utility/raii_wrapper.h"
#include "../utility/thread_wrapper.h"

#include <iostream>
using namespace std;

ZAKU_LIB_USING_NAMESPACE


class singletonTest : public interface_base
{
	void Dispose() noexcept override {};

public:
	singletonTest()
	{
		cout << "singletonTest Created" << endl;
	}
};

void threadFunc()
{
	cout << "thread Created" << endl;
}


int main()
{
	// singleton
	singleton_interface_base<singletonTest, interface_base>::instance();

	// raii wrapper
	if (true)
	{
		ZAKU_RAII_HANDLE raii_handle = ZAKU_RAII_HANDLE(
			CreateFile(_T("d:\\1.txt"), FILE_GENERIC_READ | FILE_GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL),
			ZAKU_RAII_HANDLE_FINALIZE);

		ZAKU_RAII_HANDLE raii_handle2;
		raii_handle2 = raii_handle;

		ZAKU_RAII_HANDLE raii_handle3(raii_handle2);
	}

	// thread wrapper
	if (true)
	{
		shared_ptr<thread_wrapper> thread(new thread_wrapper(threadFunc));
		thread->Run();
	}

	system("PAUSE");

    return 0;
}

