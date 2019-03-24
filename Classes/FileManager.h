#pragma once

#include "CommonData.h"

class FileManager
{
private:
	static FileManager *m_pFileManager;
public:
	static FileManager *GetInstance();

	void Init();

	bool LoadData(std::wstring resFolderPath);

private:
	bool GetFilePathList(const std::wstring &resFolderPath, std::vector<std::wstring> &filePathList);
	bool ReadFileNSetData(const std::wstring &fullPath);
	bool SetData(const std::wstring &key, const std::wstring &value);
};