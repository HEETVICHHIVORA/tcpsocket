#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in servaddr, cliaddr;
	socklen_t len;

	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    	perror("socket creation failed");
    	exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
	servaddr.sin_port = htons(PORT); // Port

	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    	perror("bind failed");
    	close(sockfd);
    	exit(EXIT_FAILURE);
	}

	len = sizeof(cliaddr); // len is value/result

	while (1) {
    	int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
    	buffer[n] = '\0';
    	printf("Client : %s\n", buffer);
    	sendto(sockfd, (const char *)buffer, n, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    	printf("Echoed message back to client\n");
	}

	close(sockfd);
	return 0;
}

