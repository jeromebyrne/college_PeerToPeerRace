#include "precompiled.h"
#include "Car.h"

Car::Car(float x, float y, float topSpeed, float acceleration , int texture) 
: MovableObject2D(x,y,topSpeed,acceleration), m_texture(texture)
{
	 m_width = 3;
	 m_length = 5;
}

Car::~Car(void)
{
}

void Car::Draw()
{

	// translating the z position by the y value because this entity has no notion of 3 dimensions but
	// it is being transformed in 3D space
	glTranslatef(m_position.X,0,m_position.Y);
    glRotatef(90 + m_yAngle,0,1,0);

    glColor3f(0.8f,0.8f,0.8f);
	glBindTexture(GL_TEXTURE_2D,m_texture);
	glBegin(GL_QUADS);
             glTexCoord2i(0,0);	glVertex3i(-m_width/2 ,1, m_length/2);
             glTexCoord2i(0,1);	glVertex3i(-m_width/2,1, -m_length/2);
             glTexCoord2i(1,1);	glVertex3i(m_width/2,1, -m_length/2);
             glTexCoord2i(1,0);	glVertex3i(m_width/2,1, m_length/2);
     glEnd();
}
