
#include <iostream>

#include "uri_conn.hpp"

int main()
{
	waspp::uri_conn uri("http://www.boost.org/LICENSE_1_0.txt");
	uri.query();

	std::cout << uri.res_content() << std::endl;

	std::cin.get();
	return 0;
}
