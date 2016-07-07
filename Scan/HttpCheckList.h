#pragma once

#include <string>

using namespace std;

#include <curl\curl.h>

class CHttpCheckList
{
public:
	CHttpCheckList(void);
	~CHttpCheckList(void);

	long Check(string url);


private:	
};

