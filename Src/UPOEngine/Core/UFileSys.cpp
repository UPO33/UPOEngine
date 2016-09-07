#include "UFileSys.h"

#include <stdio.h>

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	bool File::Exist(const char* filename)
	{
		if (filename == nullptr) return false;

		FILE* stream = nullptr;
		if (fopen_s(&stream, filename, "r") == 0)
		{
			fclose(stream);
			return true;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool File::Flush()
	{
		if (mHandle == nullptr) return false;
		return std::fflush((FILE*)mHandle) == 0;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t File::WriteBytes(const void* bytes, size_t size)
	{
		if (bytes == nullptr) return 0;
		if (mHandle == nullptr) return 0;
		return std::fwrite(bytes, 1, size, (FILE*)mHandle);
	}
	//////////////////////////////////////////////////////////////////////////
	size_t File::ReadBytes(void* outBytes, size_t size)
	{
		if (outBytes == nullptr) return 0;
		if (mHandle == nullptr) return 0;
		return std::fread(outBytes, 1, size, (FILE*)mHandle);
	}

	int File::GetFileSize() const
	{
		if (mHandle == nullptr) return -1;
		std::fflush((FILE*)mHandle);
		long cur = std::ftell((FILE*)mHandle);
		if (cur == -1L) return -1;
		std::fseek((FILE*)mHandle, 0, SEEK_END);
		long end = std::ftell((FILE*)mHandle);
		if (end != -1)
		{
			std::fseek((FILE*)mHandle, cur, SEEK_SET);
			return end;
		}
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	bool File::Open(const char* filename, EFileOpenMode mode)
	{
		if (filename == nullptr) return nullptr;

		Close();

		const char* m = nullptr;
		switch (mode)
		{
		case UPO::EFileOpenMode::Read: m = "r";
			break;
		case UPO::EFileOpenMode::Write: m = "w";
			break;
		case UPO::EFileOpenMode::Append: m = "a";
			break;
		default:
			break;
		}
		
		FILE* stream = nullptr;
		mHandle = nullptr;
		if (fopen_s(&stream, filename, m) == 0)
		{
			mOpenMode = mode;
			mHandle = stream;
			return true;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool File::Close()
	{
		if (mHandle == nullptr) return false;
		if (std::fclose((FILE*)mHandle) == 0)
			return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool File::Rename(const char* oldname, const char* newname)
	{
		if (oldname == nullptr || newname == nullptr) return false;
		return std::rename(oldname, newname) == 0;
	}

	//////////////////////////////////////////////////////////////////////////
	bool File::Delete(const char* filename)
	{
		if (filename == nullptr) return false;
		return std::remove(filename) == 0;
	}

};