#include "UFileSys.h"

#if UPLATFORM_WIN

#include <windows.h>

namespace UPO
{

	void WinPathGetFilesFolders(const char* path, TArray<String>& outFilesFoldersName, bool sub, bool getFiles, bool getFolders, const char* prefix = nullptr)
	{
		WIN32_FIND_DATAA ffd;

		char buffer[MAX_PATH];

		if (path == nullptr || (path && path[0] == 0))
		{
			sprintf_s(buffer, "*.*", path);
			path = nullptr;
		}
		else
		{
			sprintf_s(buffer, "%s/*.*", path);
		}

		HANDLE dir = FindFirstFileA(buffer, &ffd);

		if (dir == INVALID_HANDLE_VALUE) return; /* No files found */

		do
		{
			const bool is_directory = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

			if (ffd.cFileName[0] == '.')
				continue;

			char resultStr[MAX_PATH];
			if (prefix)
				sprintf_s(resultStr, "%s/%s", prefix, ffd.cFileName);
			else
				sprintf_s(resultStr, "%s", ffd.cFileName);


			if (is_directory)
			{
				char fullFileName[MAX_PATH];
				if (path)
					sprintf_s(fullFileName, "%s/%s", path, ffd.cFileName);
				else
					sprintf_s(fullFileName, "%s", ffd.cFileName);


				if (prefix)
					sprintf_s(buffer, "%s/%s", prefix, ffd.cFileName);
				else
					sprintf_s(buffer, "%s", ffd.cFileName);

				if (getFolders)
					outFilesFoldersName.Add(String(resultStr));

				if (sub)
					WinPathGetFilesFolders(fullFileName, outFilesFoldersName, sub, getFiles, getFolders, buffer);
			}
			else if (getFiles)
			{
				outFilesFoldersName.Add(String(resultStr));
			}


		} while (FindNextFileA(dir, &ffd));

		FindClose(dir);
	}



	//////////////////////////////////////////////////////////////////////////
	void PathGetFiles(const String& path, TArray<String>& outFileNames, bool sub /*= false*/)
	{
		outFileNames.RemoveAll();
		WinPathGetFilesFolders(path.CStr(), outFileNames, sub, true, false, nullptr);
	}
	//////////////////////////////////////////////////////////////////////////
	void PathGetFolders(const String& path, TArray<String>& outFoldersNames, bool sub /*= false*/)
	{
		outFoldersNames.RemoveAll();
		WinPathGetFilesFolders(path.CStr(), outFoldersNames, sub, false, true, nullptr);
	}
};

#endif