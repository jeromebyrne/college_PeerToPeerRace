#include "precompiled.h"
#include "GameObject.h"


GameObject::GameObject(void)
{
}

GameObject::GameObject(float x = 0, float y = 0)
: m_position(x,y), m_direction(1, 0), m_yAngle(0)
{
}

GameObject::~GameObject(void)
{
}

void GameObject::Update()
{
}

void GameObject::Draw()
{
}

// rotate the direction vector around the y axis
void GameObject::RotateY(float angle)
{
	m_yAngle+=angle;
    angle=(angle*PI)/180;//converts to radians
	
	Vector2 temp;
	temp.X = m_direction.Y*sin(angle)+m_direction.X*cos(angle);
	temp.Y = m_direction.Y*cos(angle)-m_direction.X*sin(angle);

	m_direction = temp;

}
