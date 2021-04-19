#include "net_exp.h"

int main(int argc, char **argv)
{

    /* 建立套接字 */
    int socket_fd;
    if ((socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket error");
        return 1;
    }

    /* 发送数据 */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(UDP_SERVER_PORT);

    int counter = 0;
    char send_buf[UDP_BUF_LENGTH];

    while (1)
    {
        memset(send_buf, 0, sizeof(send_buf));
        printf("sending data packet with #: %d\n", counter);
        sprintf(send_buf, "data packet with #: %d.", counter);
        sendto(socket_fd, send_buf, UDP_BUF_LENGTH,0,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in));

        counter++;
        if (counter > 10)
            break;

        sleep(1);
    }

    /* 关闭套接字 */
    close(socket);

    return 0;
}
