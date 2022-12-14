#include "pch.h"
#include "Allocator.h"
#include "Memory.h"

/*-----------------
	BaseAllocator
------------------*/

void* BaseAllocator::Alloc(int32 size)
{
	return malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}

/*-----------------
	StompAllocator
------------------*/

void* StompAllocator::Alloc(int32 size)
{
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int64 dataOffset = pageCount * PAGE_SIZE - size;

	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
	// 필요한 메모리보다 더 할당되기 때문에
	// 뒤에 메모리에 접근할 수 있는 오버플로우 문제가 발생할 수 있다.
	// 그래서 뒤로 메모리를 옮긴다(언더플로우가 발생되지만 오버플로우보다는 일어날 확률이 적다)
}

void StompAllocator::Release(void* ptr)
{
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}

/*-----------------
	PoolAllocator
------------------*/

void* PoolAllocator::Alloc(int32 size)
{
	return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	GMemory->Release(ptr);
}