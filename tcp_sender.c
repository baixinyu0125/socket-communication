#include "net_exp.h"

int main(int argc, char **argv)
{

    /* 建立套接字 */
    int sockfd;
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        return 1;
    }

    /* 建立tcp连接 */
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port =htons(TCP_SERVER_PORT);

    if (connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))) {
        perror("connect error");
        return 1;
    }

    /* 发送数据 */
    char *msg = "hello";
    send(sockfd, msg, strlen(msg),0);

    /* 接收数据 */
    char recv_buf[TCP_BUF_LENGTH];
    size_t pkt_len = 0;
    pkt_len = recv(sockfd, recv_buf,TCP_BUF_LENGTH,0);

    if (pkt_len == -1) {
        perror("recv error");
        return 1;
    }

    if (pkt_len == 0) {
        /* 连接被远端关闭 */
        printf("finish\n");
        return 0;
    }

    /* 输出接收到的信息 */
    recv_buf[pkt_len] = '\0';
    printf("[TCP SENDER] receive echo msg[%d bytes]\n", pkt_len);
    printf("\t%s\n", recv_buf);

    /* 关闭套接字 */
    close(sockfd);

    return 0;
}
