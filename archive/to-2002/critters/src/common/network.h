

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "messages.h"

#define PORTNUMBER 	1234 
#define BLOCK 1

struct NetMsg 
{
	char command; // command (doh!)
	char numargs; // number of arguments
	char *args; // arguments
};


class Network {
private:
	int s, t;
	struct sockaddr_in sa;
	struct hostent *hp;
	
	struct linger l;
	
	char *hostname;
	unsigned short port;
	
	char setblock;
	
	int set_nonblock_flag(int desc);
	
protected:
	void init_sockaddr(	struct sockaddr_in *name, 
				struct hostent *hostname, 
				short int port);
				
	int call_socket();
	
public:
	Network(char *h, char block = 0, unsigned short p = PORTNUMBER);
	
	int StartServer();
	int StartClient();
	
	int WriteData(char *data, long int size);
	int ReadData(char *data, long int size);

	int SendMsg(unsigned char command, unsigned char numargs, unsigned char *args);
	int GetMsg(unsigned char *command, unsigned char *numargs, unsigned char *args);	

	void EndSession();
};

	
