
#include <iostream>

#include <curl/curl.h>

int main()
{
	CURL* curl = curl_easy_init();
	if (curl)
	{
		CURLcode res;
		
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.boost.org/LICENSE_1_0.txt");
		res = curl_easy_perform(curl);
		
		
		std::cout << (char*)res << std::endl;
		curl_easy_cleanup(curl);
	}
	
	std::cin.get();
	return 0;
}
