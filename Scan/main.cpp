

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <process.h>
#include  <io.h>

#include "DirContainer.h"
#include "HttpCheckList.h"
#include "CheckResult.h"
#include <curl\curl.h>

using namespace std;


struct ThreadData
{
	HANDLE handle;
	CDirContainer *dir;
	CCheckResult *cr;
	string baseUrl;
};

//线程函数
unsigned int __stdcall StartThread(void *pParam)
{
	if(pParam == NULL) return 1;

	ThreadData *td = (ThreadData*) pParam;

	CHttpCheckList * hcl = new CHttpCheckList();

	string str = td->dir->Get();
	while(str != "")
	{
		long ret = hcl->Check(td->baseUrl + str);

		// 这个地方可能需要再来个多线程处理，不然会存在控制台输出错位
		//cout << td->baseUrl + str << "\t" << ret << endl;
		string result;  
		ostringstream os;  
		os<<ret;
		istringstream is(os.str());
		is>>result; 
		td->cr->PutResult(td->baseUrl + str + "\t" + result);

		str = td->dir->Get();
	}
	
	ReleaseSemaphore(td->handle, 1, NULL);
	delete hcl;
	return (unsigned int)0;
}

static int MAX_THREAD = 4;

void ParseArgs(int argc, char * argv[])
{
	for(int i = 2; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-thread") == 0)
			{
				MAX_THREAD = atoi(argv[i+1]);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc < 2) 
	{
		cout << "Please, specified file!!!" << endl;
		return -1;
	}
	ParseArgs(argc, argv);

	curl_global_init(CURL_GLOBAL_DEFAULT); 


	// prepare share data
	if(_access(argv[1], 4) == -1)
	{
		cout << "File not exist or no access right!!!" << endl;
		return -1;
	}

	CDirContainer * dir = new CDirContainer(argv[1]);
	cout << "File name: " << argv[1] << endl;

	HANDLE gHANDLEhSemaphore = CreateSemaphore( 
								NULL,           // default security attributes
								0,  // initial count
								MAX_THREAD,  // maximum count
								NULL);

	CCheckResult *cr = new CCheckResult();

	ThreadData * td = new ThreadData();
	td->dir = dir;
	td->handle = gHANDLEhSemaphore;
	td->cr = cr;
	td->baseUrl = "http://123.57.152.1:38870/gm/";

	// create threads
	unsigned int uiThreadId1;	
	int count = MAX_THREAD;
	while(count-- > 0)
	{
		HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0, &StartThread, (void *)td, 0, &uiThreadId1);
	}
	cout << "Thread count: " << MAX_THREAD << endl;
	
	//cout << (new CHttpCheckList())->Check("http://192.168.1.139:38870/gm/") << endl; return 0;

	count = MAX_THREAD;
	while(count > 0 || cr->ResultCount() > 0)
	{
		DWORD dw = WaitForSingleObject(gHANDLEhSemaphore, 1);
		switch(dw)
		{
		case WAIT_TIMEOUT:
			if(cr->ResultCount() > 0)
				cout << cr->GetResult() << endl;
			break;
		case WAIT_OBJECT_0:
			--count;
			break;
		default:
			
			cout << "ERROR" << endl;
			return 2;
		}
	}
	CloseHandle(gHANDLEhSemaphore);


    return 0;
}


