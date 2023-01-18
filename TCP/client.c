#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h>
#include <arpa/inet.h> // sockaddr
#include <sys/socket.h>
#include <unistd.h> // write(), close()

// 에러 처리
void error_handling(char* message);

int main(int argc, char* argv[]) {
    // 1. socket 생성
    int sock;
    struct sockaddr_in server_addr;
    int str_len;
    char message[100];

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    // 에러 처리
    if (sock == -1) {
        error_handling("socket() error");
    }
    
    // 연결할 서버 주소
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // 2. connect() 로 서버와 연결
    // 에러 처리
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_handling("connect() error");
    }
    
    // 3. read(), write() 로 채팅
    while(1) {
        fputs("Leave a message(Q/q for quit) >> ", stdout);
        fgets(message, 100, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        
        write(sock, message, sizeof(message));
        str_len = read(sock, message, 99);

        printf("Server : %s\n", message);
    }
    
    // 4. close() 로 종료
    close(sock);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}