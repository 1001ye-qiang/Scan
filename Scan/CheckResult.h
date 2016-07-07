#pragma once

#include <Windows.h>
#define MAX_SEM_COUNT 1024

#include <string>
#include <queue>
using namespace std;

class CCheckResult
{
public:
	CCheckResult(void);
	~CCheckResult(void);

	void PutResult(string str);
	string GetResult();

	int ResultCount();

private:
	HANDLE gHANDLEhSemaphore;
	HANDLE m_mutex;
	queue<string> queResult;
};

