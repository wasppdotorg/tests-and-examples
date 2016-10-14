
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <netdb.h> // addrinfo
#include <unistd.h> // close
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

#define READ_BUF_LEN 1024 * 16

void port2char(char* s, size_t n, const char* f, int p)
{
#ifndef _WIN32
	snprintf(s, n, f, p);
#else
	sprintf_s(s, n, f, p);
#endif
}

int wsa_start_up()
{
#ifdef _WIN32
	int rv = 0;
	WSADATA wsaData;
	if ((rv = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		return -1;
	}
#endif

	return 0;
}

void wsa_clean_up()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

void close_(int sockfd, int do_close, int do_wsa_clean_up)
{
#ifndef _WIN32
	if (do_close)
	{
		close(sockfd);
	}
#else
	if (do_close)
	{
		closesocket(sockfd);
	}

	if (do_wsa_clean_up)
	{
		wsa_clean_up();
	}
#endif
}

int main()
{
	const char* host = "www.geeksen.com";
	int port = 80;
	char port_[6];

	int sockfd = 0;
	struct addrinfo hints, *addr_info, *p;
	int rv;

	const char* sendbuf = "GET / HTTP/1.1\nHost:localhost\nContent-Length: 0\nConnection: close\n\n";

	char recvbuf[READ_BUF_LEN];
	int recvbuflen = READ_BUF_LEN;

	port2char(port_, 6, "%d", port);

	if (wsa_start_up() == -1)
	{
		return 1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(host, port_, &hints, &addr_info)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		close_(sockfd, 0, 1);
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
			close_(sockfd, 1, 0);
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "failed to connect\n");
		close_(sockfd, 0, 1);
		return 1;
	}

	if ((rv = send(sockfd, (const char*)sendbuf, strlen(sendbuf), 0)) == -1)
	{
		fprintf(stderr, "send: %s\n", gai_strerror(rv));
		close_(sockfd, 1, 1);
		return 1;
	}

	memset(recvbuf, 0, recvbuflen);
	rv = recv(sockfd, recvbuf, recvbuflen, 0);
	
	printf("Bytes received: %d\n", rv);
	printf("%s\n", recvbuf);

	close_(sockfd, 1, 1);

	printf("OK\n");
	int c = getchar();
	return 0;
}

