#pragma once
#include "Allocator.h"

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xxalloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...); // placement new (생성자 호출)
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type(); // 소멸자 호출
	xrelease(obj);
}