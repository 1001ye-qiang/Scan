#include "DirContainer.h"

#include <fstream>


CDirContainer::CDirContainer(string path)
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

}


CDirContainer::~CDirContainer(void)
{
	::CloseHandle(m_mutex);
}



void CDirContainer::Add(string str)
{
	queDir.push(str);
}
string CDirContainer::Get()
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