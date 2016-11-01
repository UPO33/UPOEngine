uint64 GetNewAssetID()
{
	FILETIME filetime;
	GetSystemTimeAsFileTime(&filetime);
	LARGE_INTEGER ret;
	ret.LowPart = filetime.dwLowDateTime;
	ret.HighPart = filetime.dwHighDateTime;
	return (uint64)ret.QuadPart;
}
std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
	using namespace std;
	vector<string> names;
	string search_path = folder + "*";
	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			ULOG_WARN("file %s", fd.cFileName);
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
				
			}
		} while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}