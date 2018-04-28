#pragma once

#include <memory>

#include "../define/zaku_namespace_def.h"

ZAKU_LIB_BEGIN

// T must be: no-throw default constructible and no-throw destructible  
template <typename T>
class singleton
{
private:
	struct object_creator
	{
		// This constructor does nothing more than ensure that instance()  
		//  is called before main() begins, thus creating the static  
		//  T object before multithreading race issues can come up.  
		object_creator() { singleton<T>::instance(); }
		inline void do_nothing() const { }
	};
	static object_creator create_object;

	singleton();


public:
	// If, at any point (in user code), singleton_default<T>::instance()  
	//  is called, then the following function is instantiated.  
	static T& instance()
	{
		// This is the object that we return a reference to.  
		// It is guaranteed to be created before main() begins because of  
		//  the next line.  
		static T obj;

		// The following line does nothing else than force the instantiation  
		//  of singleton_default<T>::create_object, whose constructor is  
		//  called before main() begins.  
		create_object.do_nothing();

		return obj;
	}
};

template <typename T>
typename singleton<T>::object_creator singleton<T>::create_object;

ZAKU_LIB_END