

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include "network.h"


int Network::set_nonblock_flag(int desc)
{
	int oldflags = fcntl(desc, F_GETFL, 0);
	if (oldflags == -1) {
		perror("Network::set_nonblock_flag: calling fcntl");
		return -1;
	}
	
	oldflags |= O_NONBLOCK;
	return fcntl(desc, F_SETFL, oldflags);
}


void Network::init_sockaddr(struct sockaddr_in *name, 
					struct hostent *hostname, 
					short int port)
{
	struct hostent *hostinfo;
	
	name->sin_family = AF_INET;
	name->sin_port = htons(port);
	hostinfo = hostname;
	if (hostinfo == NULL) {
		perror("Network::init_sockaddr: calling gethostbyname");
		exit(1);
	}
	
	name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}



int Network::call_socket()
{
	s = socket(AF_INET, SOCK_STREAM,0);
	if (s < 0) {
		perror("Network::call_socket: calling socket");
		return(-1);
	}

	setsockopt(s, SOL_SOCKET, SO_LINGER, &l, sizeof(struct linger) );
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, 0, 0);

	return s;
}



Network::Network(char *h, char block = 0, unsigned short p = 1234)
{
	hostname = h;
	port = p;
	l.l_onoff = 1;
	l.l_linger = 0;
	setblock = block;
	
}



int Network::StartClient()
{
	init_sockaddr(&sa, gethostbyname(hostname), port);
	call_socket();

	if (connect (s, (struct sockaddr *)&sa,sizeof sa) < 0) {
		perror("Network::StartClient: calling connect");
		close(s);
		return -1;
	}
	
	if (!setblock) 
		set_nonblock_flag(t);
	
	t = s;
	return s;
}


int Network::StartServer()
{
//	init_sockaddr(&sa, gethostbyname(hostname), port);

	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = INADDR_ANY;

	//sa.sin_addr.s_addr = INADDR_ANY;
	
	call_socket();
	
	if (bind(s, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) < 0) {
		perror("Network::StartServer: calling bind");
		close(s);
		return -1;
	}

	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, 0, 0);

	listen(s, 1);
	
	t = accept(s,NULL,NULL);
	if (t < 0) {
		printf("accept error\n");
		perror("StartServer: accept");
		return -1;
	}

	close(s);

	setsockopt(t, SOL_SOCKET, SO_LINGER, &l, sizeof(struct linger) );

	if (!setblock)
		set_nonblock_flag(t);

	return t;
}



int Network::WriteData(char *data, long int size)
{
	return send(t, data, size, 0);
}


int Network::ReadData(char *data, long int size)
{
	return recv(t, data, size, 0);
	
	if (setblock) {
		perror("Network::ReadData():");
	}
}

int Network::SendMsg(unsigned char command, unsigned char numargs, unsigned char *args)
{
	unsigned char *data;
	int i;

	data = new unsigned char[numargs+2];

	data[0] = command;
	data[1] = numargs;

	for (i=0;i<numargs;i++)	// copy arguments into data buffer
		data[i+2]=args[i];

	i = WriteData((char*)data, numargs+2); // send the message & return if
						// successful or not
	delete data;		// get rid of buffer
	return i;		// return success (or not)

}

int Network::GetMsg(unsigned char *command, unsigned char *numargs, unsigned char *args)   
{
	int count; // no of chars recv'd.

	if(ReadData((char *)command, 1)>0) 						// grab a message
	{
		ReadData((char *)numargs,1);					// and num of args
		if (*numargs) 									// if recieving any args....
		{
			count = ReadData((char *)args,*numargs);
			if (count == *numargs)
				return 1;
			else
				return -1; 								// invalid message - missing args....
		}
		else
			return 1;
	}
	else
	{
		*command = 0;
		return 0; 										// no message to be recv'd
	}	

}

void Network::EndSession()
{
	close(t);
}
