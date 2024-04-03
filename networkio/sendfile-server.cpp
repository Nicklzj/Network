#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <time.h>
#include <iostream>
int main(int argc, char *argv[])
{
    if (argc <= 3)
    {
        printf("usage: %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }
    int sum_bytes = 0;
    static char buf[1024];
    memset(buf, '\0', sizeof(buf));
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    const char *file_name = argv[3];

    int filefd = open(file_name, O_RDONLY);
    assert(filefd > 0);
    struct stat stat_buf;
    fstat(filefd, &stat_buf);

    FILE *fp = fdopen(filefd, "r");

    struct sockaddr_in sock_addr;
    bzero(&sock_addr, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &sock_addr.sin_addr);
    sock_addr.sin_port = htons(port);

    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(lfd >= 0);
    // 设置端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(bind(lfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr))==-1)
    {
        std::cout<<"绑定失败"<<std::endl;
    }
   

    if(-1==listen(lfd, 5))
    {
        std::cout<<"监听失败"<<std::endl;
    }
 

    struct sockaddr_in client_addr;
    socklen_t client_addrlength = sizeof(client_addr);
    printf("在此处阻塞\n");
    int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_addrlength);

    if (cfd < 0)
    {
        printf("errno is: %d\n", errno);
    }
    else
    {
        // time_t begintime = time(NULL);
        //time_t endtime = time(NULL);
        //printf("need time:%d\n", endtime - begintime);

        // while ((fgets(buf, 1024, fp)) != NULL)
        // {
        //     int num = send(cfd, buf, sizeof(buf), 0);
        //     sum_bytes += num;
        //     memset(buf, '\0', sizeof(buf));
        // }

        sum_bytes = sendfile(cfd, filefd, NULL, stat_buf.st_size);
        printf("sum_bytes:%ld\n", sum_bytes);
        close(cfd);
    }

    close(lfd);
    return 0;
}
