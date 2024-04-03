#include <iostream>
#include <string>
#include <ctime>
//for protobuf
#include "Test.pb.h" 
//for socket
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
using namespace std;
using namespace Test::protobuf ;

const int BUFFSIZE = 128;
const int QLEN = 10 ;

int main()
{

    int listenfd ;
    int connfd ;
    struct sockaddr_in seraddr ;
    //建立socket
    //AF_INET:IPv4因特网域
    //SOCK_STREAM:TCP链接
    //0:给定前两个参数，选择默认的协议
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd < 0 )
    {
        cout<<"socket failed"<<endl;
    }
    //地址置空
    bzero(&seraddr,sizeof(seraddr));
    //
    seraddr.sin_family = AF_INET ;
    seraddr.sin_port = htons(9999);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //关联地址和套接字
    if(bind(listenfd,(struct sockaddr *)&seraddr, sizeof(seraddr)) < 0)
    {
        cout<<"bind address with socket failed..."<<endl;
        close(listenfd);
        return -1;
    }
    //调用listen，宣告server愿意接受链接请求
    if(listen(listenfd,QLEN) == -1)
    {
        cout<<"listen on socket failed..."<<endl;
        close(listenfd);
        return -1;
    }
    //获得连接请求，并建立连接
    if( (connfd = accept(listenfd,(struct sockaddr *)NULL,NULL)) < 0 )
    {
        cout<<"accept the request failed"<<endl;
        close(listenfd);
        return -1;
    }
        mypackage::PointsArray points_array;  
 for (int i = 0; i < 2048; ++i) {  
        mypackage::Point* point = points_array.add_points();  
        point->set_x(i);  
        point->set_y(i * 2);  // 或其他任何你需要的值  
    }  
  
    // 序列化消息到字符串  
    std::string serialized_data;  
    points_array.SerializeToString(&serialized_data);  

    while(1)
    {
        if(recv(connfd,serialized_data,sizeof(serialized_data),0) < 0)
        {
            cout<<"recv failed ..."<<endl;
            break;
        }
        //protobuf反序列化
        points_array.ParseFromArray(serialized_data,BUFFSIZE);
        cout<<"last heart time:"<<myprotobuf.curtime()<<"\t"
            <<"host ip:"<<myprotobuf.hostip()<<endl;
    }
    close(listenfd);
    close(connfd);
    return 0;
}

