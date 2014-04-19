#ifndef MOVABLEOBJECT2D_H
#define MOVABLEOBJECT2D_H

#include "gameobject.h"

class MovableObject2D : public GameObject
{
private:
	void UpdateMovement();

protected:
	float m_speed;
	float m_topSpeed;
	float m_acceleration;
	float m_friction;

public:
	MovableObject2D(void);
	MovableObject2D(float x, float y, float topSpeed,float m_acceleration);
	virtual ~MovableObject2D(void);

	inline float Speed()
	{
		return m_speed;
	}
	inline void SetSpeed(float value)
	{
		m_speed = value;
	}
	inline float Friction()
	{
		return m_friction;
	}
	inline void SetFriction(float value)
	{
		m_friction = value;
	}
	inline float TopSpeed()
	{
		return m_topSpeed;
	}
	inline void SetTopSpeed(float value)
	{
		m_topSpeed = value;
	}

	virtual void Update();
	virtual void Draw();

	void Accelerate();
	void DeAccelerate(float value);

	void TurnLeft(float value);
	void TurnRight(float value);
};

#endif
