
#include <iostream>
#include <string>

#include <curl/curl.h>

std::size_t curl_write_func(char* ptr, std::size_t size, std::size_t nmemb, void* userdata)
{
	((std::string*)userdata)->append(ptr, size * nmemb);
	return size * nmemb;
}

int main()
{
	CURL* curl = curl_easy_init();
	std::string html;

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.boost.org/LICENSE_1_0.txt");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_func);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

		CURLcode res = curl_easy_perform(curl);
		if (res == CURLE_OK)
		{
			std::cout << html << std::endl;
			//std::cout << res << std::endl;
		}
	}
	curl_easy_cleanup(curl);

	std::cin.get();
	return 0;
}

