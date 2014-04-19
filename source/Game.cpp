#include "precompiled.h"
#include "Game.h"

Game::Game(void) : m_inMainMenu(true), m_connected(false), m_host(false), m_sendToPort(0),m_lastPacketSentTime(0),
m_connectedToPeer(false), m_raceStarted(false),m_lastPacketRecieveTime(0)
{
	m_lastPacketSentTime = 0;

	// car defaults
	 carAcceleration = 0.2f;
	 carTopSpeed = 2.0f;
	 defaultFriction = 0.05f;

	 // initialise the car which represents my car on the remote side
	 // this gets initialised here because it wont be drawn and doesnt rely on being assigned a texture
	 m_pMyCarRemote = new Car(0,0,carTopSpeed,carAcceleration);
	
	 m_hostCarStartPosX = -10;
	 m_hostCarStartPosY = -10;
	 m_joinCarStartPosX = -10;
	 m_joinCarStartPosY = -3;

	 m_otherCarLastSpeed = 0;
	 m_carHasJustStopped = false;
}

Game::~Game(void)
{
	// delete our car
	if(m_pMyCar != 0)
	{
		delete m_pMyCar;
		m_pMyCar = 0;
	}

	if(m_pOtherCar != 0)
	{
		delete m_pOtherCar;
		m_pOtherCar = 0;
	}

	if(m_pMyCarRemote != 0)
	{
		delete m_pMyCarRemote;
		m_pMyCarRemote = 0;
	}

	if(m_track != 0)
	{
		delete m_track;
		m_track = 0;
	}
}

void Game::Initialise()
{
	// initialise our texture manager
	Texture::GetInstance()->Initialise(GLwindow::GetInstance()->GetHwndRef());
	
	// initialise the packet transfer rate to constant
	m_packetTransferState = CONSTANT;

	// create and build our track
	m_track = new Track(-38,-32,22);
	m_track->Build();
	
	// create the checkpoint texture
	int checkpointTexture = Texture::GetInstance()->generateTexture("textures/checkpoint.bmp");
	int checkpointTexture2 = Texture::GetInstance()->generateTexture("textures/checkpoint2.bmp");

	// create our checkpoints
	m_start = new Checkpoint(-10,-17,16,10,checkpointTexture);
	m_midCheckpoint = new Checkpoint(10,27,16,10,checkpointTexture2);
	m_finish = new Checkpoint(-51,0,10,16,checkpointTexture2);

	m_startPassed = false;
	m_midCheckpointPassed = false;
    m_finishPassed = true;

	m_lapCount = -1;

	m_numPacketsSent = 0;

	m_numPacketsSentPerLap = 0;

	m_raceFinished = false;
	m_hostWon = false;

}

void Game::Update()
{
	// escape exits the program
	if(GetAsyncKeyState(VK_ESCAPE) < 0)
	{
		delete this;
		exit(0);
	}
	// if we havent started a game
	if(m_inMainMenu)
	{
		UpdateMainMenu();
	}
	// if we are in a game
	else
	{
		// get the friction value of the track at the player cars position
		float friction = m_track->GetTileFriction(m_pMyCar->X(),m_pMyCar->Y());

		// if there is resistance then lower the top speed of the car by half
		if(friction > 0)
		{
			m_pMyCar->SetTopSpeed(carTopSpeed / 3);

			// now apply the friction of the tile
			m_pMyCar->SetFriction(friction);
		}
		// else there is no resistance so set to full top speed
		else
		{
			m_pMyCar->SetTopSpeed(carTopSpeed);

			// set to the default friction
			m_pMyCar->SetFriction(defaultFriction);

		}

		// update movement
		m_pMyCar->Update();
		
		// update the other car
		m_pOtherCar->Update();
		
		// if we are using dead reckoning then 
		// update what the remote side thinks
		if(m_packetTransferState == DEADRECKONING)
		{
			m_pMyCarRemote->Update();
			
			// accelerate our remote view car if our car is faster
			if(m_pMyCarRemote->Speed() > m_otherCarLastSpeed)
			{
				m_pMyCarRemote->Accelerate();
				m_carHasJustStopped = false;
			}
			else if(m_pMyCar->Speed() <= 0 && m_carHasJustStopped == false)
			{
				m_carHasJustStopped = true;
			}
		}

		// do our networking stuff
		DoNetworking();
		
		// if the speed is greater than it was then we should accelerate
		if(m_pOtherCar->Speed() > m_otherCarLastSpeed)
		{
			m_pOtherCar->Accelerate();
		}
		// process keyboard input
		ProcessInput();

		//check if we pass the right checkpoint
		if(m_start->PointCollides(m_pMyCar->X(), m_pMyCar->Y()) && m_finishPassed == true)
		{
			m_startPassed = true;
			m_midCheckpointPassed = false;
			m_finishPassed = false;
			m_lapCount++;

			m_numPacketsSentPerLap = m_numPacketsSent;
			m_numPacketsSent = 0;
		}
		else if(m_midCheckpoint->PointCollides(m_pMyCar->X(), m_pMyCar->Y()) && m_startPassed == true)
		{
			m_startPassed = false;
			m_midCheckpointPassed = true;
			m_finishPassed = false;
		}
		else if(m_finish->PointCollides(m_pMyCar->X(), m_pMyCar->Y()) && m_midCheckpointPassed == true)
		{
			m_startPassed = false;
			m_midCheckpointPassed = false;
			m_finishPassed = true;
		}
		
		// 3 laps and the race is finished
		if(m_lapCount >= 3 && !m_raceFinished)
		{
			SendFinishRace();
		}
	}

}
void Game::Draw()
{
	if(!m_inMainMenu)
	{
		// draw the hud first so that no transformations will affect it
		DrawHud();
		
		// transform everything by the camera
		m_basicCam.Update();
		 
		// draw the track
		m_track->Draw();

		// draw start and finish checkpoints
		glPushMatrix();
			m_start->Draw();
		glPopMatrix();
		glPushMatrix();
			m_midCheckpoint->Draw();
		glPopMatrix();
		glPushMatrix();
			m_finish->Draw();
		glPopMatrix();

		glPushMatrix();
			m_pOtherCar->Draw();
		glPopMatrix();

		glPushMatrix();
			// draw the car
			m_pMyCar->Draw();
		glPopMatrix();

		// temp
		if(m_packetTransferState == DEADRECKONING)
		{
			glPushMatrix();
			m_pMyCarRemote->Draw();
			glPopMatrix();
		}
	}
}
void Game::ProcessInput()
{
	if(GetAsyncKeyState(VK_ESCAPE) < 0)
	{
		delete this;
		exit(0);
	}


	if(m_host && m_raceStarted)
	{
		// accelerate car
		if(GetAsyncKeyState(VK_UP) < 0)
		{
			m_pMyCar->Accelerate();
		}
		
		// turn left
		if(GetAsyncKeyState(VK_LEFT) < 0)
		{
			m_pMyCar->TurnLeft(5.0f);
		}
		// turn right
		if(GetAsyncKeyState(VK_RIGHT) < 0)
		{
			m_pMyCar->TurnRight(5.0f);
		}
		
	}
	else if(!m_host && m_raceStarted)
	{
		// accelerate car
		if(GetAsyncKeyState('W') < 0)
		{
			m_pMyCar->Accelerate();
		}
		
		// turn left
		if(GetAsyncKeyState('A') < 0)
		{
			m_pMyCar->TurnLeft(5.0f);
		}
		// turn right
		if(GetAsyncKeyState('D') < 0)
		{
			m_pMyCar->TurnRight(5.0f);
		}
	}

		// this variable determines when we can change the packet behaviour (just stops from scrolling through too fast)
		static int packetChangeTime = 0;
		// change packet rate and behaviour
		if(GetAsyncKeyState('P')< 0 && packetChangeTime == 0)
		{
			if(m_packetTransferState == CONSTANT)
			{
				m_packetTransferState = INTERPOLATE;
			}
			else if(m_packetTransferState == INTERPOLATE)
			{
				m_packetTransferState = DEADRECKONING;
			}
			else if(m_packetTransferState == DEADRECKONING)
			{
				m_packetTransferState = CONSTANT;
			}

			packetChangeTime = 10;
		}
		if(packetChangeTime > 0)
		{
			packetChangeTime--;
		}
	
}

// draw all text
void Game::DrawHud()
{
	glColor3f(1.0f,0.0f,0.0f);
	if(m_connected)
	{
		if(m_host)
		{
			glPushMatrix();
			glTranslatef(5,0,0);
			GLwindow::GetInstance()->Print("Hosting");
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glTranslatef(5,0,0);
			GLwindow::GetInstance()->Print("Joined");
			glPopMatrix();
		}
		
		if(m_connectedToPeer)
		{
			glPushMatrix();
			glTranslatef(5,-1,0);
			GLwindow::GetInstance()->Print("Connected to peer");
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glTranslatef(5,-1,0);
			GLwindow::GetInstance()->Print("Not Connected to peer");
			glPopMatrix();
		}
		
		if(!m_raceStarted && m_raceFinished == false)
		{
			glPushMatrix();
			glTranslatef(5,-1.5f,0);
			GLwindow::GetInstance()->Print("Waiting to Race...");
			glPopMatrix();
		}
		else if(m_raceStarted && m_raceFinished == false)
		{
			glPushMatrix();
			glTranslatef(5,-1.5f,0);
			GLwindow::GetInstance()->Print("Racing!");
			glPopMatrix();
		}
		else if(m_raceFinished == true)
		{
			glPushMatrix();
			glTranslatef(5,-1.5f,0);
			GLwindow::GetInstance()->Print("Race Finished!");
			glPopMatrix();

			if(m_host && m_hostWon)
			{
				glPushMatrix();
				glTranslatef(5,-2.0f,0);
				GLwindow::GetInstance()->Print("You Won!");
				glPopMatrix();
			}
			else if(m_host && !m_hostWon)
			{
				glPushMatrix();
				glTranslatef(5,-2.0f,0);
				GLwindow::GetInstance()->Print("You Lost");
				glPopMatrix();
			}
			else if(!m_host && !m_hostWon)
			{
				glPushMatrix();
				glTranslatef(5,-2.0f,0);
				GLwindow::GetInstance()->Print("You Won!");
				glPopMatrix();
			}
			else if(!m_host && m_hostWon)
			{
				glPushMatrix();
				glTranslatef(5,-2.0f,0);
				GLwindow::GetInstance()->Print("You Lost");
				glPopMatrix();
			}
		}


		
		
		glPushMatrix();
		glTranslatef(0,0.2f,0);
		GLwindow::GetInstance()->Print("Laps Complete: %i",m_lapCount);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0,-0.3f,0);
		GLwindow::GetInstance()->Print("Number of packets for last lap: %i",m_numPacketsSentPerLap);
		glPopMatrix();
		
		
		if(m_packetTransferState == CONSTANT)
		{
			glPushMatrix();
			glTranslatef(1,-7.3f,0);
			GLwindow::GetInstance()->Print("Packet transfer type: CONSTANT (hit 'p' to switch)");
			glPopMatrix();
		}
		else if(m_packetTransferState == INTERPOLATE)
		{
			glPushMatrix();
			glTranslatef(1,-7.3f,0);
			GLwindow::GetInstance()->Print("Packet transfer type: INTERPOLATE (hit 'p' to switch)");
			glPopMatrix();
		}
		else if(m_packetTransferState == DEADRECKONING)
		{
			glPushMatrix();
			glTranslatef(1,-7.3f,0);
			GLwindow::GetInstance()->Print("Packet transfer type: DEAD RECKONING (hit 'p' to switch)");
			glPopMatrix();
		}
		glPopMatrix();
	}
	else
	{
		GLwindow::GetInstance()->Print("Not Connected");
	}
}

// does all the networking stuff
void Game::DoNetworking()
{
	// constant packet generation
	if(m_packetTransferState == CONSTANT)
	{
		// send the data every frame while constant
		SendMyCarData();
	}// end of constant packet generation

	else if(m_packetTransferState == INTERPOLATE)
	{
		// only send a packet if its time to
		if(GetTickCount() > m_lastPacketSentTime + INTERPOLATE_SENDTIME)
		{
			SendMyCarData();
		}
	}
	else if(m_packetTransferState == DEADRECKONING)
	{
		// if we have exceeded the threshold then send our info, or if our car has just stopped
		if(m_pMyCar->Position().Distance(m_pMyCarRemote->Position()) > DEADRECKONING_THRESHOLD ||
			m_carHasJustStopped)
		{
			// send a packet
			SendMyCarData();

			// now update our local representation of the remote side
			m_pMyCarRemote->SetX(m_pMyCar->X());
			m_pMyCarRemote->SetY(m_pMyCar->Y());
			m_pMyCarRemote->SetDirectionX(m_pMyCar->DirectionX());
			m_pMyCarRemote->SetDirectionY(m_pMyCar->DirectionY());
			m_pMyCarRemote->SetAngleY(m_pMyCar->GetAngleY());
			m_pMyCarRemote->SetTopSpeed(m_pMyCar->TopSpeed());
			m_pMyCarRemote->SetSpeed(m_pMyCar->Speed());
			m_pMyCarRemote->SetFriction(m_pMyCar->Friction());

		}
	}

	// now recieve date
	RecieveData();

}

void Game::UpdateMainMenu()
{
	GLwindow::GetInstance()->Print("Press 'h' to host or 'j' to join");
	
		// if we press H then we are the host
		if(GetAsyncKeyState('H') < 0)
		{
			m_host = true;
			
			// set up a connection and listen on the host port
			if(Net::GetInstance()->SetUpConnection(HOSTPORT))
			{
				m_connected = true;
				// we want to send to the joining port
				m_sendToPort = JOINPORT;
				m_inMainMenu = false;
			}

			//================ Create cars ===================================
			int hostCarTexture = Texture::GetInstance()->generateTexture("textures/car1.bmp");
			int joinCarTexture = Texture::GetInstance()->generateTexture("textures/car2.bmp");

			// create the players car (this instance)
			m_pMyCar = new Car(0,-2,carTopSpeed,carAcceleration,hostCarTexture);

			// create the other car
			m_pOtherCar = new Car(0,-2,carTopSpeed,carAcceleration,joinCarTexture);
			//================================================================
		}
		// if we press J then we want to join
		else if(GetAsyncKeyState('J') < 0)
		{
			m_host = false;

			// set up a connection and listen on the joining port
			if(Net::GetInstance()->SetUpConnection(JOINPORT))
			{
				m_connected = true;
				// we want to send data to the host port
				m_sendToPort = HOSTPORT;
				m_inMainMenu = false;
			}

			//================ Create cars ===================================
			int hostCarTexture = Texture::GetInstance()->generateTexture("textures/car1.bmp");
			int joinCarTexture = Texture::GetInstance()->generateTexture("textures/car2.bmp");

			// create the players car (this instance)
			m_pMyCar = new Car(0,-2,carTopSpeed,carAcceleration,joinCarTexture);

			// create the other car
			m_pOtherCar = new Car(0,-2,carTopSpeed,carAcceleration,hostCarTexture);
			//================================================================
		}
}

void Game::SendMyCarData()
{
			PacketStream PS;
			//===================== send data ==================================
			ENUM_Event e = STATEDATA;
			PS.writeInt(e);

			// write the xy values to the stream
			PS.writeFloat(m_pMyCar->X());
			PS.writeFloat(m_pMyCar->Y());

			// write the angle of rotation to the stream
			PS.writeFloat(m_pMyCar->GetAngleY());

			// write the direction and topspeed variables to the stream
			PS.writeFloat(m_pMyCar->DirectionX());
			PS.writeFloat(m_pMyCar->DirectionY());
			PS.writeFloat(m_pMyCar->TopSpeed());
			PS.writeFloat(m_pMyCar->Speed());
			PS.writeFloat(m_pMyCar->Friction());

			e = ENDOFMESSAGE;
			PS.writeInt(e);

			// get the data from the stream
			char data[100];
			PS.toCharArray(data);

			// send the data
			Net::GetInstance()->SendData(data,"127.0.0.1",m_sendToPort);

			// update the time of the packet sent - for interpolate
			m_lastPacketSentTime = GetTickCount();

			// increment the number of packets sent
			m_numPacketsSent++;

			//===================================================================
}
void Game::SendStartRace()
{
	PacketStream PS;

	ENUM_Event e = STARTRACE;
	PS.writeInt(e);

	e = ENDOFMESSAGE;
	PS.writeInt(e);

	// get the data from the stream
	char data[100];
	PS.toCharArray(data);

	// send the data
	Net::GetInstance()->SendData(data,"127.0.0.1",m_sendToPort);

	// increment num packets sent
	m_numPacketsSent++;

}
void Game::SendFinishRace()
{
	PacketStream PS;

	ENUM_Event e = FINISHEDRACE;
	PS.writeInt(e);

	e = ENDOFMESSAGE;
	PS.writeInt(e);

	// get the data from the stream
	char data[100];
	PS.toCharArray(data);

	// send the data
	Net::GetInstance()->SendData(data,"127.0.0.1",m_sendToPort);

	// increment num packets sent
	m_numPacketsSent++;

	m_raceFinished = true;
	if(m_host)
	{
		m_hostWon = true;
	}
	else
	{
		m_hostWon = false;
	}
}
void Game::RecieveData()
{
	//********************* recieve data ********************************
	// create a packet stream object for recieving and sending data
	PacketStream PS;

	// now we want to recieve any data
	char recieved[100] = "n"; // set to "n" as a flag that it is unpopulated
	Net::GetInstance()->ReceiveData(recieved);
		
	// if the array has been populated
	if(recieved[0] != 'n')
	{
		// if we recieve any valid data and connectedToPeer is false
		if(!m_connectedToPeer)
		{
			/// we are now connected to a peer
			m_connectedToPeer = true;

			SendStartRace();
		}
		PS.fromCharArray(recieved);
		int e;
		PS.readInt(e);

		while(e != ENDOFMESSAGE)
		{
			switch (e)
			{
				case STATEDATA:
					{
						float x = m_pOtherCar->X();
						float y = m_pOtherCar->Y(); 
						float angle = m_pOtherCar->GetAngleY();
						float directionX = m_pOtherCar->DirectionX();
						float directionY = m_pOtherCar->DirectionY();
						float topSpeed = m_pOtherCar->TopSpeed();
						float speed = m_pOtherCar->Speed();
						float friction = m_pOtherCar->Friction();

						PS.readFloat(x);
						PS.readFloat(y);
						PS.readFloat(angle);
						PS.readFloat(directionX);
						PS.readFloat(directionY);
						PS.readFloat(topSpeed);
						PS.readFloat(speed);
						PS.readFloat(friction);
						
						if(m_packetTransferState == CONSTANT)
						{
							m_pOtherCar->SetX(x);
							m_pOtherCar->SetY(y);
							m_pOtherCar->SetAngleY(angle);
						}
						else
						{			
							// set this first so we can determine if the car should accelerate
							m_otherCarLastSpeed = m_pOtherCar->Speed();
							
							// set positional data of other car

							// we want a smooth movement to the new point
							Vector2 newPos(x,y);

							// get the vector between the new pos and old pos
							Vector2 vector = newPos-m_pOtherCar->Position();

								// if we are a good bit away then snap
								if(vector.Length() > 20)
								{
									  m_pOtherCar->SetX(x);
									  m_pOtherCar->SetY(y);
 
								}
								else if(vector.Length() > 2)
								{
									// normalise to find the direction
									  vector.Normalise();

									  directionX = vector.X;
									  directionY = vector.Y;
								}


							m_pOtherCar->SetAngleY(angle);
							m_pOtherCar->SetDirectionX(directionX);
							m_pOtherCar->SetDirectionY(directionY);
							m_pOtherCar->SetTopSpeed(topSpeed);
							m_pOtherCar->SetSpeed(speed);
							m_pOtherCar->SetFriction(friction);

						}
					}
					break;

				case FINISHEDRACE:
					  {
						  // the race is over
						  m_raceFinished = true;

						  // if the host recieved this message then they are not the winner
						  if(m_host)
						  {
							  m_hostWon = false;
						  }
						  else
						  {
							  m_hostWon = true;
						  }
					  }
					  break;

				case STARTRACE:
					{
						if(m_host)
						{
							m_pMyCar->SetX(m_hostCarStartPosX);
							m_pMyCar->SetY(m_hostCarStartPosY);
							m_pOtherCar->SetX(m_joinCarStartPosX);
							m_pOtherCar->SetY(m_joinCarStartPosY);
						}
						else
						{
							m_pMyCar->SetX(m_joinCarStartPosX);
							m_pMyCar->SetY(m_joinCarStartPosY);
							m_pOtherCar->SetX(m_hostCarStartPosX);
							m_pOtherCar->SetY(m_hostCarStartPosY);
						}
						
						m_raceStarted = true;
					}
					break;
			}
			PS.readInt(e);
		}
		m_lastPacketRecieveTime = GetTickCount();
	}
	// if we recieved an empty packet the we have reached a timeout
	else if(GetTickCount() - m_lastPacketRecieveTime > CONNECTION_TIMEOUT) 
	{
		// then we assume that we are no longer connected to the peer
		m_connectedToPeer = false;
		m_raceStarted = false;
	}
	
    //*******************************************************************
}
