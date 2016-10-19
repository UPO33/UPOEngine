#include "UName.h"
#include "UHash.h"
#include "UMemory.h"

#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Name)
	UCLASS_END_IMPL(Name)

	//////////////////////////////////////////////////////////////////////////
	void Name::MetaSerialize(Stream& stream)
	{
		if (stream.IsReader())
		{
			uint16 length = (uint16)Length();
			stream.RW(length);
			if (length) stream.Bytes((void*)CStr(), length);
		}
		else
		{
			uint16 length = 0;
			stream.RW(length);
			char buffer[NAME_MAX_LENGTH + 1];
			if (length)
			{
				stream.Bytes(buffer, length);
				buffer[length] = 0;
				*this = buffer;
			}
			else
			{
				*this = nullptr;
			}
		}
	}

	const Name Name::Empty;
	NameContext::Instance NameContext::EmptyInstance;

	//////////////////////////////////////////////////////////////////////////
	NameContext* NameContext::Get()
	{
		static NameContext Ins;
		return &Ins;
	}
	//////////////////////////////////////////////////////////////////////////
	NameContext::NameContext()
	{
		for (size_t i = 0; i < CONTEXT_TABLE_SIZE; i++)
		{
			mTable[i] = nullptr;
		}

		///////////add EmptyInstance to table
		EmptyInstance = Instance(); // maybe this constructor was called before initialization of EmptyInstance
		auto tblIndex = EmptyInstance.mHash & (CONTEXT_TABLE_SIZE - 1);
		mTable[tblIndex] = &EmptyInstance;

		ULOG_SUCCESS("context created");
	}

	NameContext::~NameContext()
	{
		FreeUnuseds();
		ULOG_MESSAGE("context released");
	}

	NameContext::Instance* NameContext::GetOrSet(const char* str)
	{
		if (str == nullptr) return &NameContext::EmptyInstance;

		size_t len = Min(StrLen(str), NAME_MAX_LENGTH);
		uint32 hash = Hash::FNV32(str, len);
		size_t tblIndex = hash & (CONTEXT_TABLE_SIZE - 1);

		Instance* iter = mTable[tblIndex];
		while (iter)
		{
			if (iter->mHash == hash) // found
			{
				iter->mRefCount++;
				return iter;
			}
			iter = iter->mNext;
		}

		//create new instance ------------
		Instance* newIns = MemNew<Instance>();
		newIns->mNext = mTable[tblIndex];
		newIns->mHash = hash;
		newIns->mLength = len;
		newIns->mRefCount = 1;
		//it is better to use static copy that will be optimized by compiler rather than memcpy
		for (size_t i = 0; i < NAME_MAX_LENGTH + 1; i++) newIns->mString[i] = str[i];
		newIns->mString[len] = '\0';

		mTable[tblIndex] = newIns;

		mNumInstance++;

// 		ULOG_MESSAGE("new instance creataed");
		return newIns;
	}

	void NameContext::FreeUnuseds()
	{
		for (size_t i = 0; i < NameContext::CONTEXT_TABLE_SIZE; i++)
		{
			Instance* head = nullptr;
			Instance* iter = mTable[i];
			while (iter)
			{
				if (iter->mRefCount == 0 && iter != &EmptyInstance) /*note: EmptyInstance is on stack*/
				{
					auto tmp = iter;
					iter = iter->mNext;
					MemDelete<Instance>(tmp);
				}
				else
				{
					auto tmp = iter->mNext;
					iter->mNext = head;
					head = iter;
					iter = tmp;
				}

			}
			mTable[i] = head;
		}
	}
};