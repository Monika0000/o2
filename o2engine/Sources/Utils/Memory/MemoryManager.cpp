#include "MemoryManager.h"

#include "Utils/Memory/IPtr.h"
#include "Utils/Assert.h"

void* operator new(size_t size, const char* location, int line)
{
	void* allocMemory = malloc(size + sizeof(o2::ObjectInfo*));
	void* object = (char*)allocMemory + sizeof(o2::ObjectInfo*);
	o2::ObjectInfo* info = new o2::ObjectInfo();
	*(o2::ObjectInfo**)allocMemory = info;
	o2::MemoryManager::OnObjectCreating(object, info, size, location, line);
	return object;
}

void operator delete(void* allocMemory)
{
	o2::MemoryManager::OnObjectDestroying(allocMemory);
}

void operator delete(void* allocMemory, const char* location, int line)
{
	o2::ErrorMessage("Used overrided delete operator!", "unknown", 0);
	_asm{ int 3 };
}

namespace o2
{
	void MemoryManager::OnObjectCreating(void* object, ObjectInfo* info, UInt size, const char* srcFile, int srcFileLine)
	{
		info->mObjectPtr = object;
		info->mSize = size;
		info->mMark = mInstance->mCurrentGCMark;
		info->mAllocSrcFileLine = srcFileLine;
		strncpy(info->mAllocSrcFile, srcFile, 127);

		mInstance->mObjectsInfos.Add(info);
	}

	void MemoryManager::OnObjectDestroying(void* object)
	{
		o2::ObjectInfo* info = GetObjectInfo(object);

		for (auto it = mInstance->mObjectsInfos.Begin(); it != mInstance->mObjectsInfos.End(); ++it)
		{
			if (*it == info)
			{
				info->~ObjectInfo();
				free(info);
				mInstance->mObjectsInfos.Remove(it);
				free((char*)object - sizeof(o2::ObjectInfo*));
				return;
			}
		}

		free(object);
	}

	void MemoryManager::OnPtrCreating(IPtr* ptr)
	{
		char* cptr = (char*)ptr;
		for (auto obj : mInstance->mObjectsInfos)
		{
			char* beg = (char*)obj->mObjectPtr;
			char* end = beg + obj->mSize;
			if (cptr >= beg && cptr < end)
			{
				obj->mChildPointers.Add(ptr);
				return;
			}
		}

		mInstance->mPointers.Add(ptr);
		ptr->mStatic = true;
	}

	void MemoryManager::OnPtrDestroying(IPtr* ptr)
	{
		mInstance->mPointers.Remove(ptr);
	}

	void MemoryManager::CollectGarbage()
	{
		mInstance->mCurrentGCMark = !mInstance->mCurrentGCMark;

		for (auto ptr : mInstance->mPointers)
		{
			if (ptr->mObjectInfo)
				ptr->mObjectInfo->Mark(mInstance->mCurrentGCMark);
		}

		ObjectsInfosArr freeObjects;

		for (auto obj : mInstance->mObjectsInfos)
		{
			if (obj->mMark != mInstance->mCurrentGCMark)
				freeObjects.Add(obj);
		}

		for (auto obj : freeObjects)
		{
			printf("Leaked object: %x %s:%i\n", obj, obj->mAllocSrcFile, obj->mAllocSrcFileLine);
			delete obj;
		}
	}

	ObjectInfo* MemoryManager::GetObjectInfo(void* object)
	{
		return *(ObjectInfo**)((char*)object - sizeof(o2::ObjectInfo*));
	}

	void ObjectInfo::Mark(bool mark)
	{
		mMark = mark;

		for (auto ptr : mChildPointers)
		{
			if (ptr->mObjectInfo && ptr->mObjectInfo->mMark != mark)
				ptr->mObjectInfo->Mark(mark);
		}
	}
}