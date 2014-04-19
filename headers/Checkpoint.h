#ifndef CHECKPOINT_H
#define CHECKPOINT_H

class Checkpoint
{
	Vector2 m_position;
	float m_length;
	float m_width;
	int m_texture;
public:
	//Checkpoint(void);
	Checkpoint(float x = 0, float y = 0, float length = 1, float width = 1, int texture = 1);
	~Checkpoint(void);


	bool PointCollides(float x, float y);
	void Draw();
};

#endif