
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

	std::string url = "https://www.google.co.kr/?gws_rd=ssl";
	std::string content;

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		if (url.substr(0, 5) == "https")
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_func);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);

		CURLcode res = curl_easy_perform(curl);
		if (res == CURLE_OK)
		{
			std::cout << content << std::endl;
		}
	}
	curl_easy_cleanup(curl);

	std::cin.get();
	return 0;
}

