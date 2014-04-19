#ifndef TRACKTILE_H
#define TRACKTILE_H

class TrackTile
{
private:
    int m_texture;
	enum ENUM_tileTypes
	{
		OFFTRACK,
		TRACKVERTICAL,
		TRACKHORIZONTAL,
		TRACKCORNER1,
		TRACKCORNER2,
		TRACKCORNER3,
		TRACKCORNER4
	};

	int m_type; // this actually only determines texture coordinates
public:
	float m_x, m_y;
	float m_friction;
	int m_width;
	TrackTile(float x = 0, float y = 0, int width = 1, int texture = 0, float friction = 0, int type = OFFTRACK);
	~TrackTile(void);
	void Draw();
};

#endif