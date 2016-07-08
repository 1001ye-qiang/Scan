#pragma once

#include <string>
#include <Windows.h>
#include <queue>
#include <array>
#include <vector>

using namespace std;

class CFileContainer
{
public:
	CFileContainer(string path, string dir_path);
	~CFileContainer(void);


	void Add(string str);
	string Get();

	vector<string> getDir() const;

private:
	void Read();

	queue<string> queDir;
	HANDLE m_mutex;

	vector<string> vecDir;
};

