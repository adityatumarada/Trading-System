#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int login_handler(int socket)
{
	char buffer[1024],user_name[512],user_pass[512];
	int n;

	memset(buffer,0,1024);
	n = read(socket,buffer,1025);

	if(n < 0)
		fprintf(stderr, "ERROR reading from socket\n");
	else
	if(n==0)
	{
		printf("Server Closed\n");             
		exit(0);
	}	
	printf("%s\n",buffer);
	
	if(strcmp(buffer,"Number of connections exceeded")==0) 
		exit(0);

	while(1)
	{           
		memset(user_name,0,512);
		memset(user_pass,0,512);
		memset(buffer,0,1024);
		
		// storing the username provided by user into user_name
		printf("Enter username:"); 
		gets(user_name);				
		user_name[strlen(user_name)] = '\0';

		//storing password into user_pass		
		printf("Enter password:");
		gets(user_pass);             
		user_pass[strlen(user_pass)] = '\0';
		
		//we send data to server using buffer.
		//input format is L#username-passwd
		strcat(buffer,"L#");              	
		strcat(buffer,user_name);	
		strcat(buffer,"-");
		strcat(buffer,user_pass);
		strcat(buffer,"\0");
		
		//we write this into newsock we got after connecting
		n=write(socket,buffer,strlen(buffer));		
		bzero(buffer,1025);
		n = read(socket,buffer,1025);
		
		//if n<0 then error in reading from socket
		if(n < 0)
			fprintf(stderr, "ERROR reading from socket\n"); 
		
		//if n==0 server has closed so we close the client
		if(n==0)
		{
			printf("Server Closed\n");               
			exit(0);
		}
		printf("%s\n",buffer);
		
		//if alreading logged we close the client
		int v=strcmp(buffer,"Already Logged In\0"); 
		if(v==0)
		{
			exit(0);
		}
		int h=strcmp(buffer,"Invalid username or password\0"); 
		if(h!=0)
		{
			break;
		}
		bzero(buffer,1025);
	}


	return 0;
}



void buy(int newsock)
{
	char buffer[1025];
		int n,code,qty,price;
		//taking item code , quantity , price 
		printf("Enter item code (choose items between 1-10) :");
		while(1)
		{
			scanf("%d",&code);
			if(code>0 && code<=10)  
			{
				break;
			}
			printf("Enter valid item code :");
		}
		printf("Enter item quantity:");
		while(1)
		{
			scanf("%d",&qty);
			if(qty>0)
			{
				break;
			}
			printf("Enter valid item quantity :");
		}
		printf("Enter item price:");
		while(1)
		{
			scanf("%d",&price);
			if(price>=0)
			{
				break;
			}
			printf("Enter valid price :");
		}

		// format is B#itemcode-quantity-price
		sprintf(buffer,"B#%d-%d-%d;",code,qty,price);
		n=write(newsock,buffer,strlen(buffer));
		if(n < 0){
			fprintf(stderr, "Error in writing to socket\n");
			exit(0);
		}
		bzero(buffer,1024);
		n=read(newsock,buffer,1024);
		if(n<0)
		{
			fprintf(stderr, "Error in reading from socket\n");
			exit(0);
		}
		if(n==0)
		{
			printf("Server Closed\n");	// if n is 0, server is closed so the client program is closed
			exit(0);
		}
		printf("%s",buffer);
}

void sell(int newsock)
{
		char buffer[1025];
		//taking item code , quantity , price 
		int n,code,qty,price;
		printf("Enter item code (choose items between 1-10) :");
		while(1)
		{
			scanf("%d",&code);
			if(code>0 && code<=10)   
			{
				break;
			}
			printf("Enter valid item code :");
		}
		printf("Enter item quantity:");
		while(1)
		{
			scanf("%d",&qty);
			if(qty>0)
			{
				break;
			}
			printf("Enter valid item quantity :");
		}
		printf("Enter item price:");
		while(1)
		{
			scanf("%d",&price);
			if(price>=0)
			{
				break;
			}
			printf("Enter valid price :");
		}
		// format is S#itemcode-quantity-price
		sprintf(buffer,"S#%d-%d-%d;",code,qty,price);
		n=write(newsock,buffer,strlen(buffer));
		if(n < 0){
			fprintf(stderr, "Error in writing to socket\n");
			exit(0);
		}
		bzero(buffer,1024);
		n=read(newsock,buffer,1024);
		if(n<0)
		{
			fprintf(stderr, "Error in reading from socket\n");
			exit(0);
		}
		if(n==0)
		{
			printf("Server Closed\n");	// if n is 0, server is closed so the client program is closed
			exit(0);
		}
		printf("%s",buffer);
}

void order_status(int newsock)
{

		int n=0;   
		char buffer[1024];
		// format is O#
		n=write(newsock,"O#",2);
		if(n<0)
		{
			fprintf(stderr, "Error in writing to socket\n");
			exit(0);
		}
		bzero(buffer,1024);
		n=read(newsock,buffer,1024);
		if(n<0)
		{
			fprintf(stderr, "Error in reading from socket\n");
			exit(0);
		}
		if(n==0)
		{
			printf("Server Closed\n");// if n is 0, server is closed so the client program is closed
			exit(0);
		}
		printf("%s",buffer);
}


void trade_status(int newsock)
{
	
		int n=0;           
		char buffer[1024];
		// format is T#
		n=write(newsock,"T#",2);
		if(n<0)
		{
			fprintf(stderr, "Error in writing to socket\n");
			exit(0);
		}
		bzero(buffer,1024);
		n=read(newsock,buffer,1024);
		if(n<0)
		{
			fprintf(stderr, "Error in reading from socket\n");
			exit(0);
		}
		if(n==0)
		{
			printf("Server Closed\n");// if n is 0, server is closed so the client program is closed
			exit(0);
		}
		printf("%s",buffer);
}

void request_handler(int sock)
{
	printf("To buy press 1\n");
	printf("To sell press 2\n");
	printf("To view order status press 3\n");
	printf("To view your trade status press 4\n");
	printf("To exit press 5\n");
	int n = 0;
	//calls the respective function based on user request.
	while(1)
	{
		printf("Enter your option:");
		scanf("%d",&n);
		switch(n)
		{
			case 1: buy(sock);
					break;
			case 2: sell(sock);
					break;
			case 3: order_status(sock);
					break;
			case 4: trade_status(sock);
					break;
			case 5: return;
			default:printf("enter valid option \n"); 
		}
	}	

}
   
int main(int argc, char const *argv[]) 
{ 

	int sock, port_server;
	char * ex_file[512];
	struct hostent *server;
	struct sockaddr_in ip_server;

	if(argc!=3)
	{
		fprintf(stderr, "input format executable_code IP_Address Port_No.\n" );
		exit(0);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	port_server = atoi(argv[2]);

	if(sock<0)
	{
		fprintf(stderr, "Error while opening the socket\n" );
		exit(0);
	}

	server = gethostbyname(argv[1]);

	if(server==NULL)
	{
		fprintf(stderr, "Host Does not exist\n" );
		exit(0);
	}
	bzero((char *)&ip_server,  sizeof(ip_server));
	ip_server.sin_family = AF_INET;	
	bcopy((char *) server->h_addr, (char *) &ip_server.sin_addr.s_addr, server->h_length);
	ip_server.sin_port= htons(port_server);

	if(connect(sock,(struct sockaddr *)&ip_server,sizeof(ip_server))<0) 
	{
		fprintf(stderr, "ERROR connecting\n");			
		exit(0);
	}

	login_handler(sock); //login handler takes care of login part
	request_handler(sock); //request_handler takes care of user requests
    return 0; 
}

