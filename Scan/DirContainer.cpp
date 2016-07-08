#include "DirContainer.h"

#include <fstream>


CFileContainer::CFileContainer(string path, string dir_path)
{
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);

	fstream file;
	file.open(path, ios::in);
	string line;
	while(getline(file, line))
	{
		if(line == "") continue;
		Add(line);
	}

	file.clear();
	file.close();

	if(dir_path == "")
		return;

	file.open(dir_path, ios::in);
	while(getline(file, line))
	{
		if(line == "") continue;
		vecDir.push_back(line);
	}
}


CFileContainer::~CFileContainer(void)
{
	::CloseHandle(m_mutex);
}



void CFileContainer::Add(string str)
{
	queDir.push(str);
}
string CFileContainer::Get()
{
	string ret = "";
	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
	if(!queDir.empty())
	{
		ret = queDir.front();
		queDir.pop();
	}
	::ReleaseMutex(m_mutex);
	return ret;

}

vector<string> CFileContainer::getDir() const
{
	return vecDir;
}