#include "UName.h"
#include "UHash.h"
#include "UMemory.h"
#include "UEngineConfig.h"

#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Name)
		UCLASS_END_IMPL(Name)

	using NameHash = CRC32;

	auto NameHashStr(const char* str)
	{
		return NameHash::HashStr(str);
	}
	auto NameHashStr(const char* str, size_t length)
	{
		return NameHash::HashBuffer(str, length);
	}
	inline unsigned HashToTableIndex(Name::HashT hash) { return hash & (NameContext::TABLE_SIZE - 1); }

	NameContext::Instance::Instance()
	{
		MemZero(this, sizeof(*this));
		mHash = NameHash::GetPrime();
	}

	//////////////////////////////////////////////////////////////////////////
	bool Name::operator==(const char* str) const
	{
		if (str)
			return mInstance->mHash == NameHashStr(str);
		else
			return mInstance == &NameContext::EmptyInstance;
	}

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
		MemZero(mTable, TABLE_SIZE * sizeof(Instance*));

		///////////add EmptyInstance to table
		EmptyInstance = Instance(); // maybe this constructor was called before initialization of EmptyInstance
		auto tblIndex = HashToTableIndex(EmptyInstance.mHash);
		mTable[tblIndex] = &EmptyInstance;

		ULOG_SUCCESS("context created");


	}

	NameContext::~NameContext()
	{
		for (size_t i = 0; i < TABLE_SIZE; i++)
		{
			Instance* iter = mTable[i];
			while (iter)
			{
				auto tmp = iter;
				iter = iter->mNext;

				if (tmp != &EmptyInstance) /*note: EmptyInstance is on stack*/
					MemDelete<Instance>(tmp);
			}
			mTable[i] = nullptr;
		}
		ULOG_MESSAGE("context released");
	}

	NameContext::Instance* NameContext::GetOrSet(const char* str)
	{
		if (str == nullptr) return &NameContext::EmptyInstance;

		size_t len = Min(StrLen(str), NAME_MAX_LENGTH);
		auto hash = NameHashStr(str, len);
		auto tblIndex = HashToTableIndex(hash);

		Instance* iter = mTable[tblIndex];
		while (iter)
		{
			if (iter->mHash == hash) // found
			{
				return iter;
			}
			iter = iter->mNext;
		}

		//create new instance ------------
		Instance* newIns = MemNew<Instance>();
		newIns->mNext = mTable[tblIndex];
		newIns->mHash = hash;
		newIns->mLength = len;
		//it is better to use static copy that will be optimized by compiler rather than memcpy
		for (size_t i = 0; i < NAME_MAX_LENGTH + 1; i++) newIns->mString[i] = str[i];
		newIns->mString[len] = '\0';

		mTable[tblIndex] = newIns;

		mNumInstance++;

		return newIns;
	}




};