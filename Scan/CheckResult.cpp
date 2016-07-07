#include "CheckResult.h"


CCheckResult::CCheckResult(void)
{
	HANDLE gHANDLEhSemaphore = CreateSemaphore( 
								NULL,           // default security attributes
								0,  // initial count
								MAX_SEM_COUNT,  // maximum count
								NULL);

	
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);
}


CCheckResult::~CCheckResult(void)
{
}


void CCheckResult::PutResult(string str)
{	
	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
	queResult.push(str);
	::ReleaseMutex(m_mutex);

	ReleaseSemaphore(gHANDLEhSemaphore, 1, NULL);
}

string CCheckResult::GetResult()
{
	WaitForSingleObject(gHANDLEhSemaphore, INFINITE);

	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
	string ret = queResult.front();
	queResult.pop();
	::ReleaseMutex(m_mutex);


	return ret;
}

int CCheckResult::ResultCount()
{
	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
	int count = queResult.size();
	::ReleaseMutex(m_mutex);
	return count;
}
