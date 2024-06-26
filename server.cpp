#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;
typedef struct{

    int x;
    int y;

}point;
point Point[1024];
int main() {
    // 创建socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket\n";
        return 1;
    }
    // 绑定服务器地址和端口
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(10000);
    if (bind(server_socket, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        std::cerr << "Error binding socket\n";
        return 1;
    }
    // 监听连接
    if (listen(server_socket, 1) < 0) {
        std::cerr << "Error listening on socket\n";
        return 1;
    }
    std::cout << "等待连接...\n";
    // 接受连接
    int client_socket = accept(server_socket, nullptr, nullptr);
    if (client_socket < 0) {
        std::cerr << "Error accepting connection\n";
        return 1;
    }
    std::cout << "连接成功，接收数据...\n";
    int index=0;
    while(1){
        // 接收数据
        point point_array[1024];
       int len =  recv(client_socket, point_array, sizeof(point_array), 0);

        // 输出接收到的整数数组
        std::cout << "接收到的整数数组长度: "<<len<<std::endl;
       // std::cout<<Point[index+=len/sizeof(point)].x<<" "<<Point[index+=len/sizeof(point)].y<<std::endl;
        if(len > 0)
        {
            for(int i=0;i<len/sizeof(point);i++){
              // Point[i+index].x=point_array[i].x;
              // Point[i+index].y=point_array[i].y;
               std::cout<<point_array[i].x<<"|"<<point_array[i].y<<"\t"; 
            }cout<<endl;
            //index+=len/sizeof(point);
        }
        else if(len  == 0)
        {
            printf("服务器断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }
    // 关闭连接
    //
    /**
    for(int i=0;i<1024;i++){
        std::cout<<Point[i].x<<"|"<<Point[i].y<<"\t";
    }cout<<endl;
    **/
    close(client_socket);
    close(server_socket);

    return 0;
}

