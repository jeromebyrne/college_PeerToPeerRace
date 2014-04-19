#include "precompiled.h"
#include "Track.h"

Track::Track(float x, float y, int tileWidth)
: m_x(x), m_y(y), m_tileWidth(tileWidth)
{
}

Track::~Track(void)
{
}

void Track::Build()
{

	// initialise all tiles types to default (offtrack type)
	int tileTypes[ROWS][COLUMNS] = {OFFTRACK};

	// now manually add the specific track tiles - I am assuming a 7*6 track
	tileTypes[1][1] = TRACKCORNER1;
	tileTypes[2][1] = TRACKVERTICAL;
	tileTypes[3][1] = TRACKVERTICAL;
	tileTypes[4][1] = TRACKCORNER2;
	tileTypes[4][2] = TRACKHORIZONTAL;
	tileTypes[4][3] = TRACKHORIZONTAL;
	tileTypes[4][4] = TRACKHORIZONTAL;
	tileTypes[4][5] = TRACKCORNER3;
	tileTypes[3][5] = TRACKCORNER4;
	tileTypes[3][4] = TRACKCORNER2;
	tileTypes[2][4] = TRACKCORNER4;
	tileTypes[2][3] = TRACKHORIZONTAL;
	tileTypes[2][2] = TRACKCORNER2;
	tileTypes[1][2] = TRACKCORNER4;

	int offtrack_texture = Texture::GetInstance()->generateTexture("textures/sand/sand.bmp");
	int trackStraight_texture = Texture::GetInstance()->generateTexture("textures/road/straight.bmp");
	int trackCorner_texture = Texture::GetInstance()->generateTexture("textures/road/corner.bmp");
	
	// the friction value for tiles which dont have any road
	float offTrackFriction = 0.19f;
	
	float onTrackFriction = 0;
	
	// the position of each tile 
	float tile_x = m_x, tile_y = m_y;

	// create all of our track tiles using the types stored in the array
	for(int i = 0; i < ROWS; i++)
	{
		// set the tile x position back to the top left corner (track position)
		tile_x = m_x;
		for(int count = 0; count < COLUMNS; count++)
		{
			if(tileTypes[i][count] == TRACKVERTICAL)
			{
				TrackTile tile(m_x + tile_x,m_y + tile_y,m_tileWidth,trackStraight_texture,onTrackFriction,TRACKVERTICAL);
				m_tiles[i][count] = tile;
			}
			else if(tileTypes[i][count] == TRACKHORIZONTAL)
			{
				TrackTile tile(m_x + tile_x,m_y + tile_y,m_tileWidth,trackStraight_texture,onTrackFriction,TRACKHORIZONTAL);
				m_tiles[i][count] = tile;
			}
			else if(tileTypes[i][count] == TRACKCORNER1)
			{
				TrackTile tile(m_x + tile_x,m_y + tile_y,m_tileWidth,trackCorner_texture,onTrackFriction,TRACKCORNER1);
				m_tiles[i][count] = tile;
			}
			else if(tileTypes[i][count] == TRACKCORNER2)
			{
				TrackTile tile(m_x + tile_x,m_y + tile_y,m_tileWidth,trackCorner_texture,onTrackFriction,TRACKCORNER2);
				m_tiles[i][count] = tile;
			}
			else if(tileTypes[i][count] == TRACKCORNER3)
			{
				TrackTile tile(m_x + tile_x,m_y + tile_y,m_tileWidth,trackCorner_texture,onTrackFriction,TRACKCORNER3);
				m_tiles[i][count] = tile;
			}
			else if(tileTypes[i][count] == TRACKCORNER4)
			{
				TrackTile tile(m_x + tile_x,m_y + tile_y,m_tileWidth,trackCorner_texture,onTrackFriction,TRACKCORNER4);
				m_tiles[i][count] = tile;
			}
			// off track
			else
			{
				TrackTile tile(m_x + tile_x,m_y + tile_y,m_tileWidth,offtrack_texture,offTrackFriction,OFFTRACK);
				m_tiles[i][count] = tile;
			}
			
			// increment the tile x position to the next tile
			tile_x += m_tileWidth;
		}
		
		// increment the tile position y position to the next tile
		tile_y += m_tileWidth;
	}
}

void Track::Draw()
{
	for(int i =0; i<ROWS; i++)
	{
		for(int count = 0; count < COLUMNS; count++)
		{
			m_tiles[i][count].Draw();
		}
	}
}

// return the friction of the tile at the xy position
float Track::GetTileFriction(float x, float y)
{
	float friction = 0;
	bool tileFound = false;

	for(int i = 0; i < ROWS; i++)
	{
		for(int count = 0; count < COLUMNS; count++)
		{
			// if the x and y are within the bounds of this tile
			if(x > m_tiles[i][count].m_x				&&
			   x < m_tiles[i][count].m_x + m_tileWidth  &&
			   y > m_tiles[i][count].m_y				&&
			   y < m_tiles[i][count].m_y + m_tileWidth    )
			{
				friction = m_tiles[i][count].m_friction;
				tileFound = true;
				break;
			}
		}
	}

	return friction;
}