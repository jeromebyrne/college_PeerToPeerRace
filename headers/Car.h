#ifndef CAR_H
#define CAR_H
#include "movableobject2d.h"

class Car : public MovableObject2D
{
private:
	unsigned int m_texture;
	float m_width, m_length;
public:
	Car(float x = 0, float y = 0, float topSpeed = 2, float acceleration = 0.2f, int texture = 0);
	virtual ~Car(void);
	void Draw();
};
#endif 
