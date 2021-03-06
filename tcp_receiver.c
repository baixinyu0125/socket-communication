#include "net_exp.h"

int main(int argc, char **argv)
{

    /* 建立服务端套接字 */
    int server_sockfd;
    if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error");
        return 1;
    }

    /* 监听端口 */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(TCP_SERVER_PORT);

    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind error");
        return 1;
    }

    if (listen(server_sockfd,CONN_NUM) == -1)
    {
        perror("listen error");
        return 1;
    };

    /* 建立tcp连接 */
    int client_sockfd;
    struct sockaddr_in client_addr;
    unsigned int client_addr_len = sizeof(struct sockaddr_in);
    if ((client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
    {
        perror("accept error");
        return 1;
    }
    printf("accept client %s:%d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);

    /* 接收数据 */
    size_t pkt_len;
    char recv_buf[TCP_BUF_LENGTH];
    pkt_len = recv(client_sockfd, recv_buf, TCP_BUF_LENGTH, 0);

    if (pkt_len == -1)
    {
        perror("recv error");
        return 1;
    }

    if (pkt_len == 0)
    {
        /* 连接被远端关闭 */
        printf("finish\n");
        return 0;
    }

    /* 输出接收到的信息 */
    recv_buf[pkt_len] = '\0';
    printf("[TCP RECEIVER] receive msg[%d bytes]\n", pkt_len);
    printf("\t%s\n", recv_buf);

    /* 发送信息 */
    char *message = "hello";
    if (send(client_sockfd, message, strlen(message), 0) == -1)
    {
        perror("send error");
        return 1;
    }

    /* 关闭套接字 */
    close(client_sockfd);
    close(server_sockfd);

    return 0;
}
