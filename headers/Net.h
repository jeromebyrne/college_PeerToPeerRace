#ifndef NET_H
#define NET_H

using namespace std;

#define MAX_MSG_SIZE 100
class Net
{
private:

	static Net * m_instance;

	WSADATA m_wsaData;
	struct sockaddr_in   m_my_addr;
	struct sockaddr_in   m_their_addr; //use to store remote address info
	int m_portNum;
	int m_sockfd;
	int m_new_fd; //used to create new socket for new connection
	

	// Create set
	fd_set m_master;   // master set
	fd_set m_readSet; // temp set    
	int m_socketArray[64]; //used to store sockets
	int m_maxSockets;
	
	int m_sin_size;

	//specify how long you want to wait for data (1 second in this case)
	struct timeval m_tv; 
	

public:
	Net(void);
	~Net(void);

	static Net * GetInstance();
	bool SetUpConnection(int port);
	void SendData(char * data, char * address, int port);
	void ReceiveData(char * data);
	inline int Port()
	{
		return m_portNum;
	}
};
#endif