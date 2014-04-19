#ifndef BASICCAMERA_H
#define BASICCAMERA_H

class BasicCamera
{
	
	public:
	float X;
	float Y;
	float Z;

	float ForwardX;
	float ForwardY;
	float ForwardZ;

	BasicCamera(void)
	{
		X = 0;
		Y = 0;
		Z = -130;
		ForwardX = 0;
		ForwardY = 1;
		ForwardZ = 0;
	}

	~BasicCamera(void)
	{
	}

	void Update()
	{
		glTranslatef(X,Y,Z);
		glRotatef(90,1,0,0);
	}

	void MoveForward(float speed)
	{
		X += ForwardX * speed;
		Y += ForwardY * speed;
		Z -= ForwardZ * speed;
	}
	void MoveBackward(float speed)
	{
		X -= ForwardX * speed;
		Y -= ForwardY * speed;
		Z += ForwardZ * speed;
	}
	void MoveLeft(float speed)
	{
		X -= speed;
	}
	void MoveRight(float speed)
	{
		X += speed;
	}
};
#endif
