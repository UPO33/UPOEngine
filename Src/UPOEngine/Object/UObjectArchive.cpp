#include "UObjectArchive.h"

#include "../Engine/UEntity.h"
#include "../Engine/UAsset.h"
#include "../Engine/UAssetSys.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	
	struct ObjectArchiveDefault
	{

		TArray<Object*>				mObjectsList;
		TArray<const ClassInfo*>	mInvolvedClasses;
		Stream*						mStream = nullptr;
		TArray<Name>				mInvolvedClassesName;
		

		Object*				mCurLoadingObject = nullptr;	//current object that has been allocating and we r writing to

		/*
		after loading objects from Stream PointerPhase is executed to Initilize
		*/
		struct PointerPhaseItem
		{
			Object*			mOwner;	//the object that has this property. used as a reference for loading asset
			Object**		mPPObject;
			ObjectPtr*		mPObjectPtr;	//valid if mIsTObjectPtr
			AssetID			mAssigningAssetID;	//valid if mAssigningIsAsset
			uint32			mAssigningObjectIndex;
			bool			mIsTObjectPtr;
			bool			mAssigningIsAsset;
		};

		TArray<PointerPhaseItem> mPointerPhaseItems;

		//is called after loading objects to initilize Object* properties
		//////////////////////////////////////////////////////////////////////////
		void PerformPointerPhase(Object* additionalRefForAssets)
		{
			for (PointerPhaseItem& item : mPointerPhaseItems)
			{
				Object* objToAssign = nullptr;
				if (item.mAssigningIsAsset)
				{
					if (Asset* assetToAssign = GAssetSys()->LoadAsset(item.mAssigningAssetID, item.mOwner))
					{
						assetToAssign->AddRef(additionalRefForAssets);
						objToAssign = assetToAssign;
					}
				}
				else
				{
					objToAssign = mObjectsList[item.mAssigningObjectIndex];
				}

				if (item.mIsTObjectPtr)
				{
					if (*item.mPObjectPtr)
					{
						ULOG_WARN("objectPtr property currently has value, do not allocate object in constrctor");
					}

					*(item.mPObjectPtr) = objToAssign;
				}
				else
				{
					if (*item.mPPObject)
					{
						ULOG_WARN("objectPtr property currently has value, do not allocate object in constrctor");
					}

					*(item.mPPObject) = objToAssign;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		uint16 ClassToIndex(const ClassInfo* ci) const
		{
			size_t index = mInvolvedClasses.Find(ci);
			if (index != ~0) return (uint16)index;
			UASSERT(false);
			return 0;
		}
		const ClassInfo* IndexToClass(uint16 classIndex) const
		{
			if (mInvolvedClasses.IsIndexValid(classIndex)) return mInvolvedClasses[classIndex];
			return nullptr;
		}
		Name IndexToName(uint16 classIndex) const
		{
			return mInvolvedClassesName[classIndex];
		}
		//////////////////////////////////////////////////////////////////////////
		bool HasObjectInList(Object* obj) const
		{
			return mObjectsList.HasElement(obj);
		}
		//////////////////////////////////////////////////////////////////////////
		void FindInvolvedClasses()
		{
			mInvolvedClasses.RemoveAll();

			/////////////extracting all used classes in parents, properties, ...
			TArray<ClassInfo*> classes;
			for (Object* obj : mObjectsList)
			{
				obj->GetClassInfo()->GetInvolvedClasses(classes, true, true, true);
				for (ClassInfo* theClass : classes)
				{
					mInvolvedClasses.AddUnique(theClass);
				}
			}

		}
		void CollectObjectsInProperties()
		{
			TArray<ClassInfo*> classes;
			for (Object* obj : mObjectsList)
			{
				classes.RemoveAll();
				obj->GetClassInfo()->GetInheritedClasses(classes, true);
				for (ClassInfo* theClass : classes)
				{
					for (unsigned iPrp = 0; iPrp < theClass->NumProperty(); iPrp++)
					{
						const PropertyInfo* prp = theClass->GetProperty(iPrp);
						Object* prpValueAsObj = nullptr;
						if (prp->GetType() == EPropertyType::EPT_ObjectPoniter)
						{
							prpValueAsObj = *((Object**)prp->Map(obj));
						}
						else if (prp->GetType() == EPropertyType::EPT_TObjectPtr)
						{
							prpValueAsObj = ((ObjectPtr*)prp->Map(obj))->Get();
						}
						if (prpValueAsObj)
						{
							if(!prpValueAsObj->Cast<Asset>())
								mObjectsList.AddUnique(prpValueAsObj);
						}
					}
				}
			}
		}
			
		//////////////////////////////////////////////////////////////////////////
		/*
			uint16 numInvolvedClass
			Name [numInvolvedClass]
		*/
		void WriteHeader()
		{
			uint16 numInvolvedClasses = (uint16)mInvolvedClasses.Length();
			mStream->RW(numInvolvedClasses);

			for (size_t i = 0; i < numInvolvedClasses; i++)
			{
				const ClassInfo* ci = mInvolvedClasses[i];
				Name className = ci->GetName();
				className.MetaSerialize(*mStream);
			}
		}
		void ReadHeader()
		{
			uint16 numInvolvedClasses = 0;
			mStream->RW(numInvolvedClasses);
			
			mInvolvedClasses.RemoveAll();
			mInvolvedClassesName.RemoveAll();

			if (numInvolvedClasses)
			{
				mInvolvedClassesName.AddDefault(numInvolvedClasses);
				mInvolvedClasses.AddUnInit(numInvolvedClasses);

				for (size_t i = 0; i < numInvolvedClasses; i++)
				{
					mInvolvedClassesName[i].MetaSerialize(*mStream);
					mInvolvedClasses[i] = MetaSys::Get()->FindClass(mInvolvedClassesName[i]);
					UASSERT(mInvolvedClasses[i]);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		/*
			unsigned numObject
			Objects [nuumObject]
		*/
		void WriteObjects()
		{
			
			unsigned numObject = mObjectsList.Length();
			mStream->RW(numObject);

			for (size_t i = 0; i < mObjectsList.Length(); i++)
			{
				Object* obj = mObjectsList[i];
				ULOG_MESSAGE("WriteObj %", obj->GetClassInfo()->GetName());
				WriteClass(obj->GetClassInfo(), obj, *mStream);
			}
			ULOG_MESSAGE("% object was written", mObjectsList.Length());
		}
		void ReadObjects()
		{
			ULOG_MESSAGE("");
			unsigned numObject = 0;
			mStream->RW(numObject);

			mObjectsList.RemoveAll();

			for (size_t i = 0; i < numObject; i++)
			{
				Object* newObj = ReadClass(nullptr, nullptr, *mStream);
				mObjectsList.Add(newObj);
			}
			ULOG_MESSAGE("% object was read", mObjectsList.Length());
		}
		//////////////////////////////////////////////////////////////////////////
		/*
			uint16 class index
			uint16 numClass
			uint32 written size
			data...
		*/
		void WriteClass(const ClassInfo* ci, void* object, Stream& stream)
		{
			TArray<ClassInfo*> classes;
			ci->GetInheritedClasses(classes);
			classes.Add((ClassInfo*)ci);

			StreamReaderMemory memStream;

			for (size_t i = 0; i < classes.Length(); i++)
			{
				WriteAClassProperties(classes[i], object, memStream);
			}


			uint16 classIndex = ClassToIndex(ci);
			uint16 numClass = (uint16)classes.Length();
			uint32 writtenSize = memStream.GetSeek();
			stream.RW(classIndex);
			stream.RW(numClass);
			stream.RW(writtenSize);
			stream.Bytes(memStream.GetHead(), writtenSize);

		}
		//////////////////////////////////////////////////////////////////////////
		Object* ReadClass(const ClassInfo* classInfo, void* object, Stream& stream)
		{
			ULOG_MESSAGE("");
			uint16 classIndex = 0;
			uint16 numClass = 0;
			uint32 readSize = 0;
			stream.RW(classIndex);
			stream.RW(numClass);
			stream.RW(readSize);

			Buffer data = Buffer(readSize);
			stream.Bytes(data.Data(), data.Size());
			StreamWriterMemory streamClass = StreamWriterMemory(data.Data(), data.Size());
			

			const ClassInfo* foundClass = IndexToClass(classIndex);
			Object* ret = nullptr;

			if (classInfo == nullptr)
			{
				if (foundClass && foundClass->IsBaseOf<Object>())
				{
					object = ret = NewObject(foundClass);
					mCurLoadingObject = ret;
					classInfo = foundClass;
				}
				else
				{
					ULOG_ERROR("there is no such class %", IndexToName(classIndex));
					return nullptr;
				}
			}

			for (uint16 i = 0; i < numClass; i++)
			{
				ReadAClassProperties(classInfo, object, streamClass);
			}

			return ret;
		}
		/*
			uint16 classIndex
			uint16 numProperty, 0xFFff == customSerialize
			uint32 written size
			data...
		*/
		//////////////////////////////////////////////////////////////////////////
		void WriteAClassProperties(const ClassInfo* classInfo, void* object, Stream& stream)
		{
			StreamReaderMemory memStream;

			uint16 numProperty = 0;


			if (classInfo->Has_MetaSerialize())
			{
				//dbg
				{
					ULOG_MESSAGE("writing MetaSerialize %", classInfo->GetName());
				}
				classInfo->Call_MetaSerialize(object, memStream);
				numProperty = 0xFFff;
			}
			else
			{
				for (size_t i = 0; i < classInfo->NumProperty(); i++)
				{
					const PropertyInfo* prp = classInfo->GetProperty(i);

					if (prp->GetAttributes().HasAttrib(EAT_Volatile)) continue;

					//dbg
					ULOG_MESSAGE("writing  %", prp->GetName());

					WriteClassProperty(prp, prp->Map(object), memStream);
					numProperty++;
				}
			}


			uint16 classIndex = ClassToIndex(classInfo);
			uint32 writtenSize = memStream.GetSeek();

			stream.RW(classIndex);
			stream.RW(numProperty);
			stream.RW(writtenSize);
			stream.Bytes(memStream.GetHead(), writtenSize);
		}
		//////////////////////////////////////////////////////////////////////////
		void ReadAClassProperties(const ClassInfo* classInfo, void* obj, Stream& stream)
		{
			ULOG_MESSAGE("");
			uint16 classIndex = 0;
			uint16 numProperty = 0;
			uint32 readSize = 0;

			
			stream.RW(classIndex);
			stream.RW(numProperty);
			stream.RW(readSize);

			Buffer data = Buffer(readSize);
			stream.Bytes(data.Data(), data.Size());
			UASSERT(!stream.HasError());

			StreamWriterMemory streamProperties = StreamWriterMemory(data.Data(), data.Size());

			auto ci = IndexToClass(classIndex);
			
			if (numProperty == 0xFFff)	//has custom serialize ?
			{
				if(ci) ci->Call_MetaSerialize(obj, streamProperties);
			}
			else
			{
				for (uint16 i = 0; i < numProperty; i++)
				{
					UASSERT(!streamProperties.HasError());
					ReadClassProperty(classInfo, obj, streamProperties);
				}
			}

		}

		/*
		Name property name
		byte propertyType
		property value
		*/
		void WriteClassProperty(const PropertyInfo* prp, void* pValue, Stream& stream)
		{
			WritePropertyName(prp, stream);
			WritePropertyType(prp, stream);

			EPropertyType prpType = prp->GetType();

			switch (prpType)
			{
			case UPO::EPT_Unknown:
				break;
			case UPO::EPT_bool:
			case UPO::EPT_int8:
			case UPO::EPT_uint8:
			case UPO::EPT_int16:
			case UPO::EPT_uint16:
			case UPO::EPT_int32:
			case UPO::EPT_uint32:
			case UPO::EPT_int64:
			case UPO::EPT_uint64:
			case UPO::EPT_float:
			case UPO::EPT_double:
			case UPO::EPT_enum:
				WritePrpArithmeticEnum(prpType, pValue, stream);
				break;
			case UPO::EPT_TArray:
				WritePrpTArray(prp, (SerArray*)pValue, stream);
				break;
			case UPO::EPT_TObjectPtr:
				WritePrpObjectPointer(((TObjectPtr<Object>*)pValue)->Get(), stream);
				break;
			case UPO::EPT_ObjectPoniter:
				WritePrpObjectPointer(*((Object**)pValue), stream);
				break;
			case UPO::EPT_MetaClass:
				WriteClass((const ClassInfo*)prp->GetTypeInfo(), pValue, stream);
				break;
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		void IgnoreReadingPrp(EPropertyType prpType, Stream& stream)
		{
			switch (prpType)
			{
			case UPO::EPT_Unknown:
				break;
			case UPO::EPT_bool:
			case UPO::EPT_int8:
			case UPO::EPT_uint8:
			case UPO::EPT_int16:
			case UPO::EPT_uint16:
			case UPO::EPT_int32:
			case UPO::EPT_uint32:
			case UPO::EPT_int64:
			case UPO::EPT_uint64:
			case UPO::EPT_float:
			case UPO::EPT_double:
			case UPO::EPT_enum:
				stream.Ignore(PropertyType_GetTypeSize(prpType));
				break;

			case UPO::EPT_TArray:
				break;
			case UPO::EPT_TObjectPtr:
			case UPO::EPT_ObjectPoniter:
			{
				uint8 identifier = 0;
				AssetID assetID;
				uint32 objIndex;
				stream.RW(identifier);
				if (identifier == 1)
					assetID.MetaSerialize(stream);
				else if(identifier == 2) 
					stream.RW(objIndex);
			}
			break;
			case UPO::EPT_MetaClass:
			{
				uint16 classIndex = 0;
				uint16 numClass = 0;
				uint32 readSize = 0;
				stream.RW(classIndex);
				stream.RW(numClass);
				stream.RW(readSize);
				stream.Ignore(readSize);
			}
			break;
			
			}
		}
		bool PrpCanConvert(EPropertyType from, EPropertyType to)
		{
			if (from == to) return true;

		}
		//////////////////////////////////////////////////////////////////////////
		void ReadClassProperty(const ClassInfo* classInfo, void* obj, Stream& stream)
		{
			Name prpName = ReadPropertyName(stream);
			EPropertyType prpType = ReadPropertyType(stream);
			
			const PropertyInfo* prpFound = classInfo->FindPropertyByName(prpName, true);

			if (prpFound == nullptr)
			{
				ULOG_ERROR("property not found [%]", prpName);
				return IgnoreReadingPrp(prpType, stream);
			}

			switch (prpType)
			{
			case UPO::EPT_Unknown:
				break;
			case UPO::EPT_bool:
			case UPO::EPT_int8:
			case UPO::EPT_uint8:
			case UPO::EPT_int16:
			case UPO::EPT_uint16:
			case UPO::EPT_int32:
			case UPO::EPT_uint32:
			case UPO::EPT_int64:
			case UPO::EPT_uint64:
			case UPO::EPT_float:
			case UPO::EPT_double:
			case UPO::EPT_enum:
				if (prpFound->IsArithmetic() || prpFound->IsEnum())
					ReadPrpArithmeticEnum(prpType, prpFound->GetType(), prpFound->Map(obj), stream);
				else
					IgnoreReadingPrp(prpType, stream);
				break;

			case UPO::EPT_TArray:
			{
				if (prpFound->GetType() == EPropertyType::EPT_TArray)
					ReadPrpTArray(prpFound, (SerArray*)prpFound->Map(obj), stream);
				else
					IgnoreReadingPrp(prpType, stream);
				break;
			}
			case UPO::EPT_TObjectPtr:
				ReadPrpObjectPointer(prpFound->Map(obj), true, stream);
				break;
			case UPO::EPT_ObjectPoniter:
				ReadPrpObjectPointer(prpFound->Map(obj), false, stream);
				break;
			case UPO::EPT_MetaClass:
				if (prpFound->GetTypeInfo() && prpFound->GetTypeInfo()->Cast<ClassInfo>()) //is property meta class?
				{
					ReadClass(prpFound->GetTypeInfo()->Cast<ClassInfo>(), prpFound->Map(obj), stream);
				}
				else
				{
					IgnoreReadingPrp(prpType, stream);
				}
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		void WritePropertyName(const PropertyInfo* prp, Stream& stream)
		{
			Name name = prp->GetName();
			name.MetaSerialize(stream);
		}
		//////////////////////////////////////////////////////////////////////////
		Name ReadPropertyName(Stream& stream)
		{
			Name ret; 
			ret.MetaSerialize(stream);
			return ret;
		}
		
		void WritePropertyType(const PropertyInfo* prp, Stream& stream)
		{
			uint8 pt = (uint8)prp->GetType();
			stream.RW(pt);
		}
		void WritePropertyType(EPropertyType propertyType, Stream& stream)
		{
			uint8 pt = (uint8)propertyType;
			stream.RW(pt);
		}
		EPropertyType ReadPropertyType(Stream& stream)
		{
			uint8 pt = 0;
			stream.RW(pt);
			return (EPropertyType)pt;
		}

		void WritePrpArithmeticEnum(EPropertyType type, void* pValue, Stream& stream)
		{
			stream.Bytes(pValue, PropertyType_GetTypeSize(type));
		}
		void ReadPrpArithmeticEnum(EPropertyType readType, EPropertyType writeType, void* outValue, Stream& inStream)
		{
			UASSERT(PropertyType_IsArithmetic(readType) || readType == EPropertyType::EPT_enum);
			UASSERT(PropertyType_IsArithmetic(writeType) || writeType == EPropertyType::EPT_enum);
			
			inStream.Bytes(outValue, PropertyType_GetTypeSize(readType));
			//TODO converting arithmetic types
		}

		/*
		byte Array element type
		uint32 array length
		*/
		void WritePrpTArray(const PropertyInfo* prpTArray, SerArray* pArray, Stream& stream)
		{
			UASSERT(prpTArray->IsTemplate() && prpTArray->GetType() == EPropertyType::EPT_TArray);

			uint32 arrayLength = pArray->mLength;
			EPropertyType arrayElemType = prpTArray->TemplateArgType();
			const TypeInfo* arrayElemTypeInfo = prpTArray->TemplateArgTypeInfo();
			unsigned arrayElemTypeSize = prpTArray->TemplateArgTypeSize();

			WritePropertyType(arrayElemType, stream);
			stream.RW(arrayLength);

			for (size_t i = 0; i < arrayLength; i++)
			{
				void* pElement = pArray->GetElement(i, arrayElemType, arrayElemTypeInfo);

				switch (arrayElemType)
				{
				case UPO::EPT_Unknown:
					break;
				case UPO::EPT_bool:
				case UPO::EPT_int8:
				case UPO::EPT_uint8:
				case UPO::EPT_int16:
				case UPO::EPT_uint16:
				case UPO::EPT_int32:
				case UPO::EPT_uint32:
				case UPO::EPT_int64:
				case UPO::EPT_uint64:
				case UPO::EPT_float:
				case UPO::EPT_double:
				case UPO::EPT_enum:
					WritePrpArithmeticEnum(arrayElemType, pElement, stream);
					break;
				case UPO::EPT_TObjectPtr:
					WritePrpObjectPointer(((TObjectPtr<Object>*)pElement)->Get(), stream);
					break;
				case UPO::EPT_ObjectPoniter:
					WritePrpObjectPointer(*((Object**)pElement), stream);
					break;
				case UPO::EPT_MetaClass:
					WriteClass((const ClassInfo*)arrayElemTypeInfo, pElement, stream);
					break;
				}
			}
		}
		
		
		void ReadPrpTArray(const PropertyInfo* prpTArray, SerArray* pArray, Stream& stream)
		{
			UASSERT(prpTArray->GetType() == EPropertyType::EPT_TArray);

			EPropertyType arrayElemType = ReadPropertyType(stream);
			uint32 arrayLength = 0;
			stream.RW(arrayLength);

			ULOG_WARN("read Array len = %", arrayLength);


			pArray->RemoveAll(prpTArray->TemplateArgType(), prpTArray->TemplateArgTypeInfo());
			pArray->AddDefault(arrayLength, prpTArray->TemplateArgType(), prpTArray->TemplateArgTypeInfo());

			auto IgnoreReading = [&]() {
				for (uint32 i = 0; i < arrayLength; i++)
					IgnoreReadingPrp(arrayElemType, stream);
			};

			switch (arrayElemType)
			{
			case UPO::EPT_Unknown:
				break;
			case UPO::EPT_bool:
			case UPO::EPT_int8:
			case UPO::EPT_uint8:
			case UPO::EPT_int16:
			case UPO::EPT_uint16:
			case UPO::EPT_int32:
			case UPO::EPT_uint32:
			case UPO::EPT_int64:
			case UPO::EPT_uint64:
			case UPO::EPT_float:
			case UPO::EPT_double:
			case UPO::EPT_enum:
				if (PropertyType_IsArithmetic(prpTArray->TemplateArgType()) || prpTArray->TemplateArgType() == EPropertyType::EPT_enum)
				{
					for (uint32 i = 0; i < arrayLength; i++)
					{
						void* pElement = pArray->GetElement(i, prpTArray->TemplateArgType(), nullptr);
						ReadPrpArithmeticEnum(arrayElemType, prpTArray->TemplateArgType(), pElement, stream);
					}
				}
				else
				{
					IgnoreReading();
				}
				
				break;

			case UPO::EPT_TObjectPtr:
			case UPO::EPT_ObjectPoniter:
			{
				EPropertyType prpArrayType = prpTArray->TemplateArgType();
				if (prpArrayType == EPropertyType::EPT_ObjectPoniter || prpArrayType == EPropertyType::EPT_TObjectPtr)
				{
					for (uint32 i = 0; i < arrayLength; i++)
					{
						void* element = pArray->GetElement(i, prpArrayType, nullptr);
						ReadPrpObjectPointer(element, prpArrayType == EPropertyType::EPT_TObjectPtr, stream);
					}
				}
				else
				{
					IgnoreReading();
				}
			}
			break;


			case UPO::EPT_MetaClass:
			{
				if (prpTArray->TemplateArgType() == EPropertyType::EPT_MetaClass)
				{
					const ClassInfo* arrElemClassInfo = prpTArray->TemplateArgTypeInfo()->Cast<ClassInfo>();
					UASSERT(arrElemClassInfo);

					for (uint32 i = 0; i < arrayLength; i++)
					{
						void* pElement = pArray->GetElement(i, prpTArray->TemplateArgType(), arrElemClassInfo);
						ReadClass(arrElemClassInfo, pElement, stream);
					}
				}
				else
				{
					IgnoreReading();
				}
			}
			break;
			}

		}
		
		/*
		uint8 identifier	0 null, 1 asset, 2 rest
		*/
		void WritePrpObjectPointer(Object* pObject, Stream& stream)
		{

			uint8 identifier = 0;

			if (pObject)
			{
				if (Asset* pAsset = pObject->Cast<Asset>()) //is asset?
				{
					identifier = 1;
					stream.RW(identifier);
					pAsset->GetID().MetaSerialize(stream);
				}
				else
				{
					auto index = mObjectsList.Find(pObject);
					if (index != InvalidIndex)	//exist in mObjectsList ?
					{
						uint32 index32 = index;
						identifier = 2;
						stream.RW(identifier);
						stream.RW(index32);
					}
					else
					{
						//Write Null ID
						stream.RW(identifier);
					}
				}
			}
			else
			{
				//Write Null ID
				stream.RW(identifier);
			}
		}
		void ReadPrpObjectPointer(void* ptrToVar /* cast to Object** or TObjectPtr<>* */, bool isTObjectPtr, Stream& stream)
		{
			uint8 identifier = 0;
			stream.RW(identifier);
			
			if(identifier)
			{
				mPointerPhaseItems.Add();
				PointerPhaseItem& item = mPointerPhaseItems.LastElement();
				item.mOwner = mCurLoadingObject;
				item.mIsTObjectPtr = isTObjectPtr;

				if (isTObjectPtr)
				{
					item.mPObjectPtr = (ObjectPtr*)ptrToVar;
				}
				else
				{
					item.mPPObject = (Object**)ptrToVar;
				}

				if (identifier == 1)	//asset
				{
					item.mAssigningIsAsset = true;
					item.mAssigningAssetID.MetaSerialize(stream);
				}
				else if (identifier == 2) //rest types
				{
					item.mAssigningIsAsset = false;
					stream.RW(item.mAssigningObjectIndex);
				}
				else
					UASSERT(false);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		void Save(TArray<Object*>& inObjects, Stream* stream)
		{
			ULOG_MESSAGE("saving...");
			mObjectsList = inObjects;
			mStream = stream;

			CollectObjectsInProperties();
			FindInvolvedClasses();
			WriteHeader();
			WriteObjects();
		}
		void Load(TArray<Object*>& outObjects, Stream* stream, Object* additionalRefForAssets)
		{
			ULOG_MESSAGE("loading...");
			mPointerPhaseItems.RemoveAll();
			mStream = stream;
			ReadHeader();
			ReadObjects();
			PerformPointerPhase(additionalRefForAssets);
			outObjects = mObjectsList;
		}
	};


#if 0
	//////////////////////////////////////////////////////////////////////////
	void ObjectArchive::BeginWrite(Stream* out, String tag)
	{
		mStream = out;
		mTag = tag;
		
		mObjectsList.RemoveAll();
		mUnstreamedObjects.RemoveAll();
	}

	//////////////////////////////////////////////////////////////////////////
	void ObjectArchive::RegObject(Object* obj)
	{
		mObjectsList.AddUnique(obj);
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	void ObjectArchive::EndReg()
	{
		mUnstreamedObjects = mObjectsList;

		mUsedClasses.RemoveAll();
		
		/////////////extracting all used classes in parents, properties, ...
		TArray<const ClassInfo*> classes;
		for (size_t iObject = 0; iObject < mObjectsList.Length(); iObject++)
		{
			mObjectsList[iObject]->GetClassInfo()->GetInvolvedClasses(classes, true, true, true);
			for (size_t iClass = 0; iClass < classes.Length(); iClass++)
			{
				mUsedClasses.AddUnique(classes[iClass]);
			}
		}
	}
	

	void ObjectArchive::EndWrite()
	{
		ULOG_MESSAGE("");
		EndReg();
		WriteHeader();

		while (mUnstreamedObjects.Length())
		{
			WriteAnObject();
		}
	}
	/*
		String Tag
		dw numObject
		dw numInvolvedClass
		Name[] involved class
	*/
	void ObjectArchive::WriteHeader()
	{
		mTag.MetaSerialize(*mStream);	//write tag

		unsigned numObject = mUnstreamedObjects.Length();
		mStream->RW(numObject);

		unsigned numUsedClasses = mUsedClasses.Length();
		mStream->RW(numUsedClasses);


		for (size_t i = 0; i < numUsedClasses; i++)
		{
			const ClassInfo* ci = mUsedClasses[i];
			Name className = ci->GetName();
			className.MetaSerialize(*mStream);
		}
		char bn = '\n';
		mStream->RW(bn);
	}

	void ObjectArchive::WriteAnObject()
	{
		if (mUnstreamedObjects.Length())
		{
			Object* obj = nullptr;
			mUnstreamedObjects.Pop(obj);
			WriteClass(obj->GetClassInfo(), obj, *mStream);
		}
	}
	/*
	dw	class index
	dw	size
	bytes[size] data
	*/
	void ObjectArchive::WriteClass(const ClassInfo* ci, void* object, Stream& stream)
	{
		TArray<const ClassInfo*> classes;
		ci->GetInheritedClasses(classes);
		classes.Add(ci);

		StreamReaderMemory memStream;

		for (size_t i = 0; i < classes.Length(); i++)
		{
			WriteAClassProperties(classes[i], object, memStream);
		}

		
		unsigned classIndex = ClassToIndex(ci);
		unsigned writtenSize = memStream.GetSeek();
		stream.RW(classIndex);
		stream.RW(writtenSize);
		stream.Bytes(memStream.GetHead(), writtenSize);

	}
	/*
	dw class index
	bool customSerialize
	dw size
	bytes[size] data
	*/
	void ObjectArchive::WriteAClassProperties(const ClassInfo* cls, void* object, Stream& stream)
	{
		StreamReaderMemory memStream;

		if (cls->HasMetaSerialize())
		{
			cls->CallMetaSerialize(object, memStream);
		}
		else
		{
			for (size_t i = 0; i < cls->NumProperty(); i++)
			{
				const PropertyInfo* prp = cls->GetProperty(i);

				if (prp->GetAttributes().HasAttrib(EAT_Volatile)) continue;

				WriteClassProperty(prp, prp->Map(object), memStream);
			}
		}

		
		unsigned classIndex = ClassToIndex(cls);
		bool bCusotmSerialize = cls->HasMetaSerialize();
		unsigned writtenSize = memStream.GetSeek();
		
		stream.RW(classIndex);
		stream.RW(bCusotmSerialize);
		stream.RW(writtenSize);
		stream.Bytes(memStream.GetHead(), writtenSize);
	}
	/*
	Name property name
	byte propertyType
	proper value
	*/
	void ObjectArchive::WriteClassProperty(const PropertyInfo* prp, void* pValue, Stream& stream)
	{
		ULOG_MESSAGE("wp %s", prp->GetName().CStr());

		WritePropertyName(prp, stream);
		WritePropertyType(prp, stream);

		EPropertyType prpType = prp->GetType();

		switch (prp->GetType())
		{
		case UPO::EPT_Unknown:
			break;
		case UPO::EPT_bool:
		case UPO::EPT_int8:
		case UPO::EPT_uint8:
		case UPO::EPT_int16:
		case UPO::EPT_uint16:
		case UPO::EPT_int32:
		case UPO::EPT_uint32:
		case UPO::EPT_int64:
		case UPO::EPT_uint64:
		case UPO::EPT_float:
		case UPO::EPT_double:
		case UPO::EPT_enum:
			stream.Bytes(pValue, prp->GetTypeSize());
			break;

		case UPO::EPT_TArray:
			WritePrpTArray(prp, pValue, stream);
			break;
		case UPO::EPT_TObjectPtr:
			WritePrpObjectPointer(((TObjectPtr<Object>*)pValue)->Get(), stream);
			break;
		case UPO::EPT_ObjectPoniter:
			WritePrpObjectPointer(*((Object**)pValue), stream);
			break;
		case UPO::EPT_MetaClass:
			WriteClass((const ClassInfo*)prp->GetTypeInfo(), pValue, stream);
		break;
		}
	}
	void ObjectArchive::WritePropertyName(const PropertyInfo* prp, Stream& stream)
	{
		Name name = prp->GetName();
		name.MetaSerialize(stream);
	}

	void ObjectArchive::WritePropertyType(const PropertyInfo* prp, Stream& stream)
	{
		uint8 pt = (uint8)prp->GetType();
		stream.RW(pt);
	}

	void ObjectArchive::WritePrpTArray(const PropertyInfo* prp, void* ptr, Stream& stream)
	{
		TArray<char>* pTArray = (TArray<char>*)ptr;
		unsigned arrayLength = pTArray->Length();
		EPropertyType arrayElemType = prp->TemplateArgType();
		const TypeInfo* arrayElemTypeInfo = prp->TemplateArgTypeInfo();
		unsigned arrayElemTypeSize = prp->TemplateArgTypeSize();

		stream.RW(arrayElemType);
		stream.RW(arrayLength);

		for (size_t i = 0; i < arrayLength; i++)
		{
			void* pElement = pTArray->Elements() + i * arrayElemTypeSize;

			switch (arrayElemType)
			{
			case UPO::EPT_Unknown:
				break;
			case UPO::EPT_bool:
			case UPO::EPT_int8:
			case UPO::EPT_uint8:
			case UPO::EPT_int16:
			case UPO::EPT_uint16:
			case UPO::EPT_int32:
			case UPO::EPT_uint32:
			case UPO::EPT_int64:
			case UPO::EPT_uint64:
			case UPO::EPT_float:
			case UPO::EPT_double:
			case UPO::EPT_enum:
				stream.Bytes(pElement, arrayElemTypeSize);
				break;

			case UPO::EPT_TObjectPtr:
				WritePrpObjectPointer(((TObjectPtr<Object>*)pElement)->Get(), stream);
				break;
			case UPO::EPT_ObjectPoniter:
				WritePrpObjectPointer(*((Object**)pElement), stream);
				break;
			case UPO::EPT_MetaClass:
				WriteClass((const ClassInfo*)prp->GetTypeInfo(), pElement, stream);
				break;
			}
		}
	}
	void ObjectArchive::WritePrpObjectPointer(Object* pObject, Stream& stream)
	{
		if (pObject)
		{
			if (Asset* pAsset = pObject->Cast<Asset>()) //is asset?
			{

			}
			else if (Entity* pEntity = pObject->Cast<Entity>()) // is entity?
			{
				if (this->HasObjectInList(pEntity))
				{

				}
				else
				{
					///////////Write Null ID
				}
			}
			else
			{
				/////////////error
			}
		}
		else
		{
			//Write Null ID
		}
	}

	

	unsigned ObjectArchive::ClassToIndex(const ClassInfo* ci) const
	{
		size_t index = mUsedClasses.Find(ci);
		if (index != ~0) return (unsigned)index;
		return 0;
	}

	bool ObjectArchive::HasObjectInList(Object* obj) const
	{
		return mObjectsList.HasElement(obj);
	}
#endif
	void ObjectArchive::Save(TArray<Object*>& inObjects, Stream* stream)
	{
		ObjectArchiveDefault saver;
		saver.Save(inObjects, stream);
	}

	void ObjectArchive::Load(TArray<Object*>& outObjects, Stream* stream, Object* additionalRefForAssets)
	{
		ObjectArchiveDefault loader;
		loader.Load(outObjects, stream, additionalRefForAssets);
	}

};