#include "precompiled.h"
#include "Checkpoint.h"

//Checkpoint::Checkpoint(void)
//{
//}
Checkpoint::Checkpoint(float x, float y, float length, float width, int texture)
: m_length(length), m_width(width), m_texture(texture)
{
	m_position.X = x;
	m_position.Y = y;
}
Checkpoint::~Checkpoint(void)
{
}
bool Checkpoint:: PointCollides(float x, float y)
{
	bool collision = false;

	if(x > m_position.X && x < m_position.X + m_width &&
		y > m_position.Y && y < m_position.Y + m_length)
	{
		collision = true;
	}

	return collision;
}
void Checkpoint:: Draw()
{
	glColor3f(0.8f,0.8f,0.8f);
	glBindTexture(GL_TEXTURE_2D,m_texture);
	
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2i(0,0); glVertex3i(m_position.X ,1, m_position.Y);
			glTexCoord2i(0,1); glVertex3i(m_position.X,1, m_position.Y + m_length);
			glTexCoord2i(1,1); glVertex3i(m_position.X + m_width,1, m_position.Y + m_length);
			glTexCoord2i(1,0); glVertex3i(m_position.X + m_width,1, m_position.Y);
		glEnd();
	glPopMatrix();
}
