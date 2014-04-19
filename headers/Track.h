#ifndef TRACK_H
#define TRACK_H

#include "TrackTile.h"

#define ROWS    6
#define COLUMNS 7

class Track
{
private:
	// position of the track (top left corner)
	float m_x, m_y;
	
	// the width of the tiles that make up the track
	int m_tileWidth;

	// an array of all the track tiles
	TrackTile m_tiles[ROWS][COLUMNS];

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


public:
	Track(float x = 0, float y = 0, int tileWidth = 1);
	~Track(void);
	void Build(); // construct the track
	void Draw();

	// get the friction value of the tile at the xy position 
	float GetTileFriction(float x, float y);
	

	inline float X()
	{
		return m_x;
	}
	inline float Y()
	{
		return m_y;
	}

};

#endif
