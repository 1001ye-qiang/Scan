#pragma once

#include <string>
#include <Windows.h>
#include <queue>

using namespace std;

class CDirContainer
{
public:
	CDirContainer(string path);
	~CDirContainer(void);


	void Add(string str);
	string Get();

private:
	void Read();

	queue<string> queDir;

	HANDLE m_mutex;
};

