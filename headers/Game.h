#ifndef GAME_H
#define GAME_H

#include "Car.h"
#include "BasicCamera.h"
#include "PacketStream.h"
#include "Track.h"
#include "Checkpoint.h"

#define HOSTPORT 25000 // the port the host is listening on
#define JOINPORT 28000 // the port the client which joined is listening on

#define INTERPOLATE_SENDTIME 200 // the time to send a packet when using interpolate
#define DEADRECKONING_THRESHOLD 1 // when using dead reckoning send packet when this distance is exceeded
#define CONNECTION_TIMEOUT 3000 // 3 second timeout
 
class Game
{
private:
	bool m_inMainMenu; // are we in the main menu or are we in a game
	bool m_connected; // initialise a connection
	bool m_host; // are we hosting or are we joining
	int m_sendToPort; // the port we are sending to
	bool m_connectedToPeer; // is someone else in the game

	// a very basic camera
	BasicCamera m_basicCam;

	// our 2 cars
	Car * m_pMyCar;
	Car * m_pOtherCar;

	// checkpoints
	Checkpoint * m_start;
	Checkpoint * m_midCheckpoint;
	Checkpoint * m_finish;

	bool m_startPassed;
	bool m_midCheckpointPassed;
	bool m_finishPassed;

	int m_lapCount;

	float m_hostCarStartPosX;
	float m_hostCarStartPosY;
	float m_joinCarStartPosX;
	float m_joinCarStartPosY;

	float m_otherCarLastSpeed;
	bool m_carHasJustStopped;

	bool m_raceStarted;
	bool m_raceFinished;
	bool m_hostWon;

	// this car represents the position the remote side is predicting - used for dead reckoning
	Car * m_pMyCarRemote;

	// The different types of packet transfer types
	enum ENUM_PacketTransfer
	{ 
		CONSTANT,
		INTERPOLATE,
		DEADRECKONING
	};

	ENUM_PacketTransfer m_packetTransferState;
	
	// stores the number of packets sent for the current lap
	int m_numPacketsSent;

	// stores the number of packets for the last lap
	int m_numPacketsSentPerLap;
	
	// the different types of events that can be sent
	enum ENUM_Event
	{
		FINISHEDRACE,
		STARTRACE,
		STATEDATA,
		ENDOFMESSAGE
	};

	// the time the last packet was sent
	int m_lastPacketSentTime;

	// the last time we recieved a packet
	int m_lastPacketRecieveTime;

	// the track
	Track * m_track;
	
	// car defaults
	float carAcceleration ;
	float carTopSpeed ;
	float defaultFriction ;
	
	void ProcessInput();
	void DoNetworking();
	void UpdateMainMenu();
	void SendMyCarData();
	void SendStartRace();
	void SendFinishRace();
	void RecieveData();
public:
	Game(void);
	~Game(void);
	void Initialise();
	void Update();
	void Draw();
	

	// draws a simple text hud
	void DrawHud();

	// see if the players are passing through checkpoints
	void UpdateCheckpoints();

};

#endif
