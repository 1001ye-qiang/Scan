#include "HttpCheckList.h"


#include <iostream>


CHttpCheckList::CHttpCheckList(void)
{
}


CHttpCheckList::~CHttpCheckList(void)
{
}

static int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
    unsigned long sizes = size * nmemb;
    if (writerData == NULL) return 0;
    writerData->append(data, sizes);
    return sizes;
}

 long CHttpCheckList::Check(string url) // thread safe
{
	long ret;
	CURLcode res;
	CURL *curl;

	curl = curl_easy_init();    // ≥ı ºªØ
	res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); // not thread safe
	res = curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	res = curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1); 

	if (curl)
    {
		res = curl_easy_setopt(curl, CURLOPT_URL,url.c_str());

		string buffer;

		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);


        res = curl_easy_perform(curl);   // ÷¥––
        if (res == CURLE_OK) {
			long retcode = 0;
			res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode); 
			if(res == CURLE_OK && retcode == 200)
			{
				curl_easy_cleanup(curl);
				return retcode;
			}
				curl_easy_cleanup(curl);
			return retcode;
        }
				curl_easy_cleanup(curl);
		return res;
    }
				curl_easy_cleanup(curl);
	return 0;
}
