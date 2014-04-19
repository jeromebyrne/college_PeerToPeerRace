#include "precompiled.h"
#include "TrackTile.h"

TrackTile::TrackTile(float x, float y, int width, int texture, float friction, int type) 
: m_x(x), m_y(y), m_width(width), m_texture(texture), m_friction(friction), m_type(type)
{
}

TrackTile::~TrackTile(void)
{
}

void TrackTile::Draw()
{
	glColor3f(0.8f,0.8f,0.8f);
	glBindTexture(GL_TEXTURE_2D,m_texture);
	
	glPushMatrix();
		glBegin(GL_QUADS);

		// decide the texture coordinates and draw the quad
		if(m_type == TRACKCORNER2)
		{
			glTexCoord2i(0,0); glVertex3i(m_x ,1, m_y);
			glTexCoord2i(0,1); glVertex3i(m_x,1, m_y + m_width);
			glTexCoord2i(1,1); glVertex3i(m_x + m_width,1, m_y + m_width);
			glTexCoord2i(1,0); glVertex3i(m_x + m_width,1, m_y);
		}
		else if(m_type == TRACKCORNER3)
		{
			glTexCoord2i(1,0); glVertex3i(m_x ,1, m_y);
			glTexCoord2i(1,1); glVertex3i(m_x,1, m_y + m_width);
			glTexCoord2i(0,1); glVertex3i(m_x + m_width,1, m_y + m_width);
			glTexCoord2i(0,0); glVertex3i(m_x + m_width,1, m_y);
		}
		else if(m_type == TRACKCORNER4)
		{
			glTexCoord2i(1,1); glVertex3i(m_x ,1, m_y);
			glTexCoord2i(1,0); glVertex3i(m_x,1, m_y + m_width);
			glTexCoord2i(0,0); glVertex3i(m_x + m_width,1, m_y + m_width);
			glTexCoord2i(0,1); glVertex3i(m_x + m_width,1, m_y);
		}
		else if(m_type == TRACKVERTICAL)
		{
			glTexCoord2i(0,0); glVertex3i(m_x ,1, m_y);
			glTexCoord2i(1,0); glVertex3i(m_x,1, m_y + m_width);
			glTexCoord2i(1,1); glVertex3i(m_x + m_width,1, m_y + m_width);
			glTexCoord2i(0,1); glVertex3i(m_x + m_width,1, m_y);
		}
		else
		{
			glTexCoord2i(0,1); glVertex3i(m_x ,1, m_y);
			glTexCoord2i(0,0); glVertex3i(m_x,1, m_y + m_width);
			glTexCoord2i(1,0); glVertex3i(m_x + m_width,1, m_y + m_width);
			glTexCoord2i(1,1); glVertex3i(m_x + m_width,1, m_y);
		}

		
		 glEnd();

	 glPopMatrix();
}
