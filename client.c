// client code for UDP socket programming 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1" // localhost 
#define PORT_NO 15050 
#define NET_BUF_SIZE 32 
#define cipherKey 'S' 
#define sendrecvflag 0 

// function to clear buffer 
void clearBuf(char* b) 
{ 
	int i; 
	for (i = 0; i < NET_BUF_SIZE; i++) 
		b[i] = '\0'; 
} 

// function for decryption 
char Cipher(char ch) 
{ 
	return ch ^ cipherKey; 
} 

// function to receive file 
int recvFile(char* buf, int s,char* ss) 
{ 
    printf("%s",ss);
	int i; 
	char ch; 
    char data[NET_BUF_SIZE];
    FILE *fileAddress;
    fileAddress = fopen("clientfiles/ss.txt", "a+");
	for (i = 0; i < s; i++) { 
		ch = buf[i]; 
		ch = Cipher(ch); 
         

		if (ch == EOF) 
			{
                fprintf(fileAddress,"%s",data);
                fclose(fileAddress);
                return 1;} 
		else
            data[i] = ch;
			//fputs(data, fileAddress);
            printf("%c", data[i]); 
	} 
    //fputs(data, fileAddress);
    
	return 0; 
} 

// driver code 
int main() 
{ 
	int sockfd, nBytes,select; 
	struct sockaddr_in addr_con; 
	int addrlen = sizeof(addr_con); 
	addr_con.sin_family = AF_INET; 
	addr_con.sin_port = htons(PORT_NO); 
	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
	char net_buf[NET_BUF_SIZE]; 
	FILE* fp; 

	// socket() 
	sockfd = socket(AF_INET, SOCK_DGRAM, 
					IP_PROTOCOL); 

	if (sockfd < 0) 
		printf("\nfile descriptor not received!!\n"); 
	else
		printf("\nfile descriptor %d received\n", sockfd); 
    
    printf("\n if you want to view the files press 1");
    printf("\n if you want to send the files press 2");
    printf("\n if you want to receive the files press 3");
    scanf("%d",&select);

    //switch(select)
    

    
    
	while (1) { 

		switch(select){
			case 1:
		{
		char* type="receive";
		printf("\nPlease enter file name to receive:\n"); 
		scanf("%s", net_buf); 
        char filename[20];
        strcpy(filename,net_buf);
		sendto(sockfd, net_buf, NET_BUF_SIZE, 
			sendrecvflag, (struct sockaddr*)&addr_con, 
			addrlen); 
		sendto(sockfd, type, sizeof(type), 
			sendrecvflag, (struct sockaddr*)&addr_con, 
			addrlen);
		printf("\n---------Data Received---------\n"); 

		while (1) { 
			// receive 
			clearBuf(net_buf); 
			nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, 
							sendrecvflag, (struct sockaddr*)&addr_con, 
							&addrlen); 

			// process 
			if (recvFile(net_buf, NET_BUF_SIZE,filename)) { 
				break; 
			} 
		} 
		printf("\n-------------------------------\n"); 

		char* name;
		int nameBytes = recvfrom(sockfd, name, 
						sizeof(name), sendrecvflag, 
						(struct sockaddr*)&addr_con, &addrlen);
						break;
	} 
	case 2:{
		char filename="";
		sendto(sockfd, filename, sizeof(filename), 
			sendrecvflag, (struct sockaddr*)&addr_con, 
			addrlen);
		clearBuf(net_buf);
			printf("\nPlease enter file name to send:\n"); 
		scanf("%s", net_buf); 
        char filen[20];
        strcpy(filen,net_buf);
		char ch;
		char* data;
		int i=0;
        FILE *file;
        file = fopen(filen,"r");
        while ((ch=fgetc(file)) != EOF)
        {printf("%c", ch);
		data[i]=ch;
		i++;
		}
        fclose(file);
		sendto(sockfd, data, sizeof(data), 
			sendrecvflag, (struct sockaddr*)&addr_con, 
			addrlen); 
	}
		}
	return 0; 
	}
} 
