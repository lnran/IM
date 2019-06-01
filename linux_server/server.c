#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define FD_SIZE 100     //max client and save selsect return value
#define MAX_BACK 100  //max listen count
#define MAX_BUF 1024   //max message 

struct sockaddr_in client;
int cli[FD_SIZE];      //client count


int create_socket(char *PORT)
{
    
    //create
    int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock<0){
        perror("create socket error!\n");
        return -1;
    }

    struct sockaddr_in local;
    
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(PORT));
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind
    if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0){
        perror("bind error!\n");
        close(sock);
        return -1;
    }
    
    //listen
    if(listen(sock,MAX_BACK)<0){
        perror("listen error!\n");
        close(sock);
        return -1;
    }

    return sock;
}

static int ser_accept_process(int serfd,fd_set *allfds,int *maxfd)
{
	int i;
	memset((struct sockaddr*)&client,0,sizeof(client));
	socklen_t addrlen = sizeof(client);
	int confd = accept(serfd,(struct sockaddr*)&client,&addrlen);
	if(confd<0){
		printf("accept error\n");
		close(confd);
		return -1;
	}
	

	for(i=0;i<FD_SIZE;i++)
	{
		if(cli[i]<0){
			cli[i] = confd;
			break;
		}
	}
	FD_SET(confd,allfds);
	if(i==FD_SIZE){
		printf("Max client.\n");
		close(confd);		
		return -1;
	}
	if(confd > *maxfd)
		*maxfd = confd;
	printf("accept new client:%s:%d\n",inet_ntoa(client.sin_addr),client.sin_port);

	return 1;
}

static void ser_recv_process(fd_set *readfds,fd_set *allfds)
{
	int i = 0, j = 0, clifd;
	size_t iRed;
	char buf[MAX_BUF]={0};
	char backBuf[MAX_BUF+33]={0};

	for(i = 0; i<FD_SIZE; i++)
	{
		if(cli[i]<0)
			continue;
		/*判断此链接是否有新的数据可读*/
		if(FD_ISSET(cli[i],readfds)){
			iRed = read(cli[i],buf,sizeof(buf)-1);
			buf[iRed] = '\0';
			
			if(iRed <= 0){
			    /*数据为空 代表关闭客户端*/
               printf("%s is left.\n",inet_ntoa(client.sin_addr));
                for(j = 0; j<FD_SIZE; j++)
                {
                    if(cli[j]==-1 || (cli[j] == cli[i]))
                        continue;
                    memset(buf,'\0',sizeof(buf));
                    sprintf(buf,"%s is left.\n",inet_ntoa(client.sin_addr));
                    write(cli[j],buf,strlen(buf));
                }
                FD_CLR(cli[i],allfds);
                close(cli[i]);
                cli[i] = -1;
                continue;
			} 
			else{
			/*数据不为空 向其余链接客户端发送数据*/
				printf("%s:%s",inet_ntoa(client.sin_addr),buf);
				sprintf(backBuf,"%s:%s",inet_ntoa(client.sin_addr),buf);
				backBuf[iRed+32]='\0';

				for(j=0; j<FD_SIZE; j++)
				{
					if(cli[j]==-1 || (cli[j] == cli[i]))
						continue;
					else
						write(cli[j],backBuf,strlen(backBuf));				
				}
			}
		}
	}
}

int main(int argc,char *argv[])
{
    if(argc<2){
        perror("Please enter: ./server [port]\n");
        exit(-1); 
    }
    int i;
    int serfd = create_socket(argv[1]);
    int maxfd = serfd;
    fd_set fds;
    fd_set allfds;
    struct timeval timeout = {3,0};
    for(i=0;i<FD_SIZE;i++)
        cli[i]=-1;    

    FD_ZERO(&fds);
    FD_ZERO(&allfds);
    FD_SET(serfd,&allfds);
    while(1){
        fds = allfds;
        switch(select(maxfd+1,&fds,NULL,NULL,&timeout)){
        case 0:
            //perror("select tinmeout!\n");
            break;//timeout and continue
        case -1:
            perror("select error!\n");
            break;
        default:
            {
                if(FD_ISSET(serfd,&fds))
		        	ser_accept_process(serfd,&allfds,&maxfd);
		        else
			        ser_recv_process(&fds,&allfds);
            }
            break; //default break

         }//end switch
    }//end while
    
    close(serfd);
    for(i = 0; i<FD_SIZE; i++)
    {
        close(cli[i]);
    }


    return 0;
}


