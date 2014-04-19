#include "precompiled.h"
#include "Net.h"

Net * Net::m_instance = 0;

Net * Net::GetInstance()
{
	if(m_instance == 0)
	{
		m_instance = new Net();
	}
	return m_instance;
}

Net::Net(void)
{
}

Net::~Net(void)
{
}

bool Net::SetUpConnection(int port)
{
	if (WSAStartup(MAKEWORD(2,2),&m_wsaData) !=0)
	{
		GLwindow::GetInstance()->Print("An error has occured in Net::SetUpConnection");

		return false;
	}

	m_portNum = port;

	m_my_addr.sin_family = AF_INET;
	m_my_addr.sin_port = htons(m_portNum); 
	m_my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(m_my_addr.sin_zero),0,8);

	m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	bind(m_sockfd, (struct sockaddr *)&m_my_addr, sizeof(m_my_addr)); 
	m_sin_size = sizeof (m_their_addr);

	// Create set
	FD_ZERO(&m_master); //clear the master set
	FD_ZERO(&m_readSet); //clear the temp set
	FD_SET(m_sockfd, &m_master);//add socket to master set

	//specify how long you want to wait for data (1 second in this case)
	m_tv.tv_sec = 0.00;
	m_tv.tv_usec = 0;

	return true;
}

void Net::SendData(char* data, char * address, int port)
{
	m_their_addr.sin_family = AF_INET;
	m_their_addr.sin_port = htons(port); 
	m_their_addr.sin_addr.s_addr = inet_addr(address); 
	memset(m_their_addr.sin_zero, 0, sizeof m_their_addr.sin_zero);
	int sin_size = sizeof (m_their_addr); 

	sendto(m_sockfd, data, MAX_MSG_SIZE, 0, (struct sockaddr *)&m_their_addr, m_sin_size );

	//cout<<"\n\n Data sent: "<<*data;
}

void Net::ReceiveData(char * data)
{
		m_readSet = m_master; //copy the master
		int ready = select(0, &m_readSet, NULL, NULL, &m_tv); //check for reads

		if (ready)
		{
			if (FD_ISSET(m_sockfd,&m_readSet))
			{	
				// receive from
				recvfrom(m_sockfd, data, MAX_MSG_SIZE, 0,(struct sockaddr *)& m_their_addr, &m_sin_size);

				//cout<<"\n\n data received: "<<*data;
			}  
		}	
}
