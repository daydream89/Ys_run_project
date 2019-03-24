#include "FileManager.h"

#include <fstream>
#include <Windows.h>

#include "SystemData.h"

namespace FileManagerPrivate
{
	eFileType GetFileTypeFromFilePath(const std::wstring &fullPath)
	{
		size_t pos = fullPath.find_last_of(L"\\");
		std::wstring fileName = fullPath.substr(pos, fullPath.length());
		if (fileName.compare(L"\\Path.txt") == 0)
			return FILE_TYPE_PATH;
		else if (fileName.compare(L"\\System.txt") == 0)
			return FILE_TYPE_SYSTEM;
		else if (fileName.compare(L"\\Tag.txt") == 0)
			return FILE_TYPE_TAG;
		else
			return FILE_TYPE_NONE;
	}
}

FileManager *FileManager::m_pFileManager = nullptr;
FileManager *FileManager::GetInstance()
{
	if (m_pFileManager == nullptr)
		m_pFileManager = new FileManager;
	return m_pFileManager;
}

void FileManager::Init()
{

}

bool FileManager::LoadData(std::wstring resFolderPath)
{
	if (SystemData::GetInstance() == nullptr)
		return false;

	// in first, based on windows
	// todo. add android & ios later
	std::vector<std::wstring> filePathList;
	if (GetFilePathList(resFolderPath, filePathList) == false)
		return false;
	
	for (const std::wstring &filePath : filePathList)
	{
		ReadFileNSetData(filePath);
	}

	return true;
}

bool FileManager::GetFilePathList(const std::wstring &resFolderPath, std::vector<std::wstring> &filePathList)
{
	std::wstring designFolderPath = resFolderPath + L"design/*.txt";

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(designFolderPath.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;

	if(fd.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
		filePathList.push_back(resFolderPath + L"design\\" + fd.cFileName);

	while (FindNextFile(hFind, &fd))
	{
		if (fd.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
			filePathList.push_back(resFolderPath + L"design\\" + fd.cFileName);
	}

	FindClose(hFind);

	return true;
}

bool FileManager::ReadFileNSetData(const std::wstring &fullPath)
{
	eFileType fileType = FileManagerPrivate::GetFileTypeFromFilePath(fullPath);
	if (fileType == FILE_TYPE_NONE)
		return false;

	wchar_t pBuffer[MAX_STR_LEN] = {0, };
	std::wifstream file;
	file.open(fullPath.c_str());
	if (file.is_open() == false)
		return false;

	std::wstring wcsBuffer;
	while (file.getline(pBuffer, (std::streamsize)MAX_STR_LEN))
	{
		if (pBuffer[0] != L'\0')
		{
			wcsBuffer.assign(pBuffer, MAX_STR_LEN);

			size_t keyEndPos = wcsBuffer.find_first_of(L"\t");
			std::wstring wcsKey = wcsBuffer.substr(0, keyEndPos);

			size_t valueStartPos = wcsBuffer.find_last_of(L"\t");
			std::wstring wcsValue = wcsBuffer.substr(valueStartPos + 1, wcsBuffer.length() - 1);
			
			SystemData::GetInstance()->SetData(fileType, wcsKey, wcsValue);
		}
	}

	file.close();
	
	return true;
}