#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024

int main(){
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    ssize_t n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Listen for UDP packets on port %d \n", PORT);

    while(1){
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &len);

        if(n < 0){
            perror("recvfrom failed");
            continue;
        }

        buffer[n] = '\0';

        printf("\n Received %zd bytes from %s:%d\n", n, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        printf("Data: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}