#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject
{
private:

protected:
	Vector2 m_position;
	Vector2 m_direction;

	// how much the entity is rotated around the y axis
	float m_yAngle;
public:
	GameObject(void);
	GameObject(float x, float y);
	virtual ~GameObject(void);

	virtual void Update();
	virtual void Draw();

	inline Vector2 Position()
	{
		return m_position;
	}
	inline Vector2 Direction()
	{
		return m_direction;
	}
	inline float X()
	{
		return m_position.X;
	}
	inline float Y()
	{
		return m_position.Y;
	}
	inline void SetX(float value)
	{
		m_position.X = value;
	}
	inline void SetY(float value)
	{
		m_position.Y = value;
	}
	inline float DirectionX()
	{
		return m_direction.X;
	}
	inline float DirectionY()
	{
		return m_direction.Y;
	}
	inline void SetDirectionX(float value)
	{
		m_direction.X = value;
	}
	inline void SetDirectionY(float value)
	{
		m_direction.Y = value;
	}
	inline float GetAngleY()
	{
		return m_yAngle;
	}
	inline void SetAngleY(float value)
	{
		m_yAngle = value;
	}
	// rotate around the y axis
	void RotateY(float angle); 
	
};

#endif