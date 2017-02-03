#pragma once


#include "UHash.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	template<typename T> size_t GetHash(T x) { return std::hash<T>()(x); }

	inline size_t GetHash(const char* cstr) 
	{ 
		if (cstr)
			return CRC32::HashStr(cstr);
		else
			return CRC32::GetPrime();
	}



	////////////////////////////////////  TPair  //////////////////////////////////////
	template<typename T0, typename T1> struct TPair
	{
		T0	mFirst;
		T0  mSecond;
	};



	//////////////////////////////////  THasherBase  ////////////////////////////////////////
	template<typename TKey, typename THashType = size_t> struct TMapSetHasherBase
	{
		typedef THashType HashType;

		inline static HashType GetHash(const TKey key)
		{
			return UPO::GetHash(key);
		}
	};





	template<typename TKey, typename TValue, size_t TableSize = 8, typename THasher = TMapSetHasherBase<TKey>> class TArrayMap
	{
		static_assert(IsPow2(TableSize), "TableSize must be power of two");

	public:
		typedef typename THasher::HashType HashT;
		typedef TPair<const TKey, TValue>  PairT;

	private:
		struct Table
		{
			TArray<HashT>		mHashes;
			TArray<TValue>		mValues;
		};


		Table	mTables[TableSize];


	public:
		TArrayMap(size_t initialCapacity = 0)
		{
			if (initialCapacity == 0) return;

			for (Table& table : mTables)
			{
				table.mHashes.SetCapacity(initialCapacity);
				table.mValues.SetCapacity(initialCapacity);
			}
		}
		void Empty()
		{
			for (Table& table : mTables)
			{
				table.mHashes.Empty();
				table.mValues.Empty();
			}
		}
		void RemoveAll()
		{
			for (Table& table : mTables)
			{
				table.mHashes.RemoveAll();
				table.mValues.RemoveAll();
			}
		}
		TValue* Find(const TKey& key) const
		{
			HashT hash = THasher::GetHash(key);
			unsigned tblIndex = hash & (TABLE_SIZE - 1);
			auto foundIndex = mTables[tblIndex].mHashes.Find(hash);
			if (foundIndex != InvalidIndex)
			{
				return mTables[tblIndex].mValues.ElementAt(foundIndex);
			}
			return nullptr;
		}
		bool Remove(const TKey& key)
		{
			HashT hash = THasher::GetHash(key);
			unsigned tblIndex = hash & (TABLE_SIZE - 1);
			auto foundIndex = mTables[tblIndex].mHashes.Find(hash);
			if (foundIndex != InvalidIndex)
			{
				mTables[tblIndex].mValues.RemoveAtSwap(foundIndex);
				return true;
			}
			return false;
		}
		TValue* Put(const TKey& key, const TValue& value)
		{
			HashT hash = THasher::GetHash(key);
			unsigned tblIndex = hash & (TABLE_SIZE - 1);
			auto foundIndex = mTables[tblIndex].mHashes.Find(hash);
			if (foundIndex != InvalidIndex)
			{
				return mTables[tblIndex].mValues.ElementAt(foundIndex);
			}
			else
			{
				mTables[tblIndex].mHashes.Add(hash);
				auto index = mTables[tblIndex].mValues.Add(value);
				return mTables[tblIndex].mValues.Elements() + index;
			}
		}
		TValue& operator [] (const TKey& key)
		{
			return *Put(key, TValue(), false);
		}
		template<typename Lambda> void ForEach(Lambda proc)
		{
			for (Table& table : mTables)
				table.mValues.ForEach(proc);
		}
		size_t GetNumElement() const
		{
			size_t n = 0;
			for (Table& table : mTables) 
				n += table.mHashes.Length();
			return n;
		}
		size_t GetTableSize() const
		{
			return TableSize;
		}
	};



};