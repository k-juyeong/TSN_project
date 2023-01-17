#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h>
#include <arpa/inet.h> // sockaddr
#include <sys/socket.h>
#include <unistd.h> // write(), close()

void error_handling(char* message);

int main(int argc, char* argv[]) {
    // 1. socket() 생성
    int sock;
    int recv_len, send_len;
    char message[100];

    struct sockaddr_in server_addr, client_addr;
    socklen_t server_addr_size, client_addr_size;

    if(argc != 3) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    // 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    // 에러 처리
    if(sock == -1)
        error_handling("socket() error");

    while(1) {
        fputs("Leave a message(Q/q for quit) >> ", stdout);
        fgets(message, 100, stdin);
        
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 2. sendto()
        server_addr_size = sizeof(server_addr);
        send_len = sendto(sock, message, 100, 0, (struct sockaddr*) &server_addr, &server_addr_size);
        // 에러 처리
        if(send_len == -1)
            error_handling("sendto() error");

        // 3. recvfrom()
        client_addr_size = sizeof(client_addr);
        recv_len = recvfrom(sock, message, 100, 0, (struct sockaddr*) &client_addr, &client_addr_size);
        // 에러 처리
        if(recv_len == -1)
            error_handling("recvfrom() error");

        printf("Server : %s\n", message);
    }

    // 4. close()
    close(sock);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}