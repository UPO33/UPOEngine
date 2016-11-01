#pragma once

#include "UBasic.h"
#include "UName.h"
#include "UMemory.h"
#include "UBuffer.h"
#include "UString.h"
#include "UArray.h"

namespace UPO
{
	static const unsigned UMAX_PATH = FILENAME_MAX;

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
		Name mfileName;
	public:
		File() : mHandle(nullptr), mOpenMode(EFileOpenMode::Read) {}
		File(const char* filename, EFileOpenMode openMode)
		{
			mHandle = nullptr;
			mOpenMode = openMode;
			mfileName = filename;
			Open(filename, openMode);
		}
		~File()
		{
			Close();
		}
		bool Flush();

		//The total number of bytes successfully written is returned.
		size_t WriteBytes(const void* bytes, size_t size);
		//The total number of bytes successfully read is returned.
		size_t ReadBytes(void* outBytes, size_t size);
		//return size of the file in bytes, -1 if error
		int GetSize() const;

		bool IsOpen() const { return mHandle != nullptr; }
		EFileOpenMode OpenMode() const { return mOpenMode; }
		const Name& GetName() const { return mfileName; }

		bool Open(const char* filename, EFileOpenMode mode);
		bool Close();

		static bool Rename(const char* oldname, const char* newname);
		static bool Delete(const char* filename);
		static bool Exist(const char* filename);
		static bool OpenReadFull(const char* filename, Buffer& out);
	};


	UAPI void PathGetFiles(const String& path, TArray<String>& outFileNames, bool includingSubFiles = false);
	UAPI void PathGetFolders(const String& path, TArray<String>& outFolderNames, bool includingSubFolders = false);
}