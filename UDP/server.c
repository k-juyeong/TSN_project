#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h>
#include <arpa/inet.h> // sockaddr
#include <sys/socket.h>
#include <unistd.h> // write(), close()

// #define MSG_SIZE 256;

void error_handling(char* message);

int main(int argc, char* argv[]) {
    // 1. socket() 생성
    int server_sock;
    int str_len, recv_len, send_len;
    char buffer[100];

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;

    if (argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    // 소켓 생성
    server_sock = socket(PF_INET, SOCK_DGRAM, 0);
    // 에러 처리
    if (server_sock == -1) {
        error_handling("socket() error");
    }
    
    // 2. bind() 로 주소 할당
    // 주소 할당
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // 에러 처리
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }
    
    while(1) {
        // 3. recvfrom()
        client_addr_size = sizeof(client_addr);
        recv_len = recvfrom(server_sock, buffer, sizeof(buffer), 0, (struct sockaddr*) &client_addr, &client_addr_size);
        // 에러 처리
        if (recv_len == -1) {
            error_handling("recvfrom() error");
        }

        // 4. sendto()
        send_len = sendto(server_sock, buffer, recv_len, 0, (struct sockaddr*) &client_addr, &client_addr_size);
        // 에러 처리
        if (send_len == -1) {
            error_handling("sendto() error");
        }
    }
    
    // 5. close() 로 종료
    close(server_sock);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}