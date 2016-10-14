
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <netdb.h> // addrinfo
#include <unistd.h> // close
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

int main()
{
	char* host = "127.0.0.1";
	int port = 6379;
	char port_[6]; // strlen("65535")

	int sockfd;
	struct addrinfo hints, *addr_info, *p;
	int rv;

#ifndef _WIN32
	snprintf(port_, 6, "%d", port);
#else
	sprintf_s(port_, 6, "%d", port);

	WSADATA wsaData;
	if ((rv = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		return 1;
	}

#endif

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(host, port_, &hints, &addr_info)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for (p = addr_info; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
#ifndef _WIN32
			close(sockfd);
#else
			closesocket(sockfd);
#endif
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "failed to connect\n");
		return 1;
	}

	freeaddrinfo(addr_info);

	printf("OK\n");
	int c = getchar();
	return 0;
}
