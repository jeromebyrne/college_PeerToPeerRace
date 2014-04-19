#include "precompiled.h"
#include "MovableObject2d.h"

MovableObject2D::MovableObject2D(void) : GameObject()
{
}

MovableObject2D::MovableObject2D(float x, float y, float topSpeed, float acceleration) 
: GameObject(x,y),m_speed(0),m_topSpeed(topSpeed),m_acceleration(acceleration),m_friction(0.05f)
{
}

MovableObject2D::~MovableObject2D(void)
{
}

void MovableObject2D::Update()
{
	GameObject::Update(); // update base class first
	UpdateMovement();
}

void MovableObject2D::Draw()
{
	GameObject::Draw();
}

void MovableObject2D::UpdateMovement()
{
	// stop the entity going too fast
	if(m_speed > m_topSpeed) 
	{
		m_speed = m_topSpeed;
	}

	// apply friction
	m_speed -= m_friction;
	if(m_speed < 0)
	{
		m_speed = 0;
	}

	// get the velocity
	Vector2 velocity(m_direction.X * m_speed,m_direction.Y * m_speed);

	// update the position
	m_position.X = m_position.X + velocity.X;
	m_position.Y = m_position.Y + velocity.Y;
}

void MovableObject2D::Accelerate()
{
	m_speed += m_acceleration;
}

void MovableObject2D::DeAccelerate(float value)
{
	m_speed -= value;
}

void MovableObject2D::TurnLeft(float value)
{
	RotateY(value * m_speed);
}

void MovableObject2D::TurnRight(float value)
{
	RotateY(value * -m_speed);
}