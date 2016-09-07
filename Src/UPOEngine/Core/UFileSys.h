#pragma once

#include "UBasic.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	enum class EFileOpenMode
	{
		//Open file for input operations. The file must exist.
		Read,	
		//Create an empty file for output operations. If a file with the same name already exists, 
		//its contents are discarded and the file is treated as a new empty file. 
		Write,
		//Open file for output at the end of a file. Output operations always write data at the end of the file, expanding it.
		//Repositioning operations are ignored. The file is created if it does not exist.
		Append
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI File
	{
		void* mHandle;
		EFileOpenMode mOpenMode;
		
	public:
		File(): mHandle(nullptr), mOpenMode(EFileOpenMode::Read) {}
		File(void* handle, EFileOpenMode om) : mHandle(handle), mOpenMode(om) {}

		bool Flush();

		//The total number of bytes successfully written is returned.
		size_t WriteBytes(const void* bytes, size_t size);
		//The total number of bytes successfully read is returned.
		size_t ReadBytes(void* outBytes, size_t size);
		int GetFileSize() const;
		
		bool IsOpen() const { return mHandle != nullptr; }
		EFileOpenMode OpenMode() const { return mOpenMode; }
		bool Open(const char* filename, EFileOpenMode mode);
		bool Close();

		static bool Rename(const char* oldname, const char* newname);
		static bool Delete(const char* filename);
		static bool Exist(const char* filename);
	};


};