#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

   
int main(int argc, char const *argv[]) 
{ 

	int sock, port_server;
	char * ex_file[512];
	struct hostent *server;
	struct sockaddr_in ip_server;

	if(argc!=3)
	{
		fprintf(stderr, "input format executable code IP Address Port No.\n" );
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

	login_handler(sock);
	printf("hey boi how to doshafijh\n");	


    return 0; 
}

int login_handler(int socket)
{
	char buffer[1024],temp1[512],temp2[512];
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
	if(strcmp(buffer,"Connections Exceeded")==0) 
		exit(0);

	while(1)
	{           
		memset(temp1,0,512);
		memset(temp2,0,512);
		memset(buffer,0,1024);

		printf("Enter username:"); 
		gets(temp1);				// storing the username provided by user into temp1
		temp1[strlen(temp1)-1] = '\0';
		printf("Enter password:");
		gets(temp2);             //storing password into temp2
		temp2[strlen(temp2)-1] = '\0';
		strcat(buffer,"L#");              	//in buffer initially we put 1# because this is login information server understands that this login information by seeing this 1
		strcat(buffer,temp1);				//next we put username:passwd into the buffer which the same format client login detils are stored in login.txt in the server side
		strcat(buffer,":");
		strcat(buffer,temp2);
		strcat(buffer,";\0");
		n=write(socket,buffer,strlen(buffer));		//we write this into newsock we got after connecting
		bzero(buffer,1025);
		n = read(socket,buffer,1025);
		if(n < 0)
			fprintf(stderr, "ERROR reading from socket\n"); 
		if(n==0)
		{
			printf("Server Closed\n");                //if n==0 bytes then it is an indication that server has closed so we close the client also
			exit(0);
		}
		printf("%s\n",buffer);
		
		int v=strcmp(buffer,"Already Logged In\0");  //if alreading logged in aslo we should close the client
		
		if(v==0)
		{
			exit(0);
		}
		int h=strcmp(buffer,"Invalid Username or Password\0"); //if invalid username or password we do not close the client we print try again and this goes in a loop
		if(h!=0)
		{
			break;
		}
		bzero(buffer,1025);
	}


	return 0;
}




