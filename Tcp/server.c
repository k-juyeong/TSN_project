#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h>
#include <arpa/inet.h> // sockaddr
#include <sys/socket.h>
#include <unistd.h> // write(), close()

// 에러 처리
void error_handling(char* message);

int main(int argc, char *argv[]) {
    // 1. socket 생성
    int server_sock, client_sock;
    int str_len;

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    
    char message[] = "Hello Client!";

    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 소켓 생성
    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    // 에러 처리
    if (server_sock == -1)
        error_handling("socket() error");
    
    
    // 2. bind() 로 주소 할당
    // 주소 할당
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // 주소 할당 실패 시 에러 처리
    if(bind(server_sock, (const struct sockadrr*) &server_addr, sizeof(server_addr)) == -1)
        error_handling("bind() error");


    // 3. listen() 로 클라이언트 접속 요청 기다림
    // 에러 처리
    if (listen(server_sock, 5) == -1)
        error_handling("listen() error");


    // 4. accept() 로 클라이언트 접속 요청 승인 후 전용 소켓 생성
    client_addr_size = sizeof(client_addr);

    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
    // 에러 처리
    if(client_sock == -1) 
        error_handling("accept() error");
    

    // 5. read(), write() 로 채팅
    while((str_len = read(client_sock, message, 99)) != 0)
        write(client_sock, message, str_len);

    // 6. close() 로 종료
    close(client_sock);


    close(server_sock);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}