#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "cinder/Vector.h"
#include "cinder/Color.h"

using namespace ci;
struct Particle
{
	vec3	Position;
	int		Age,
		Life,
		Index;

	float	Size;

	Color	Emissive;

	bool	IsShining;

	Particle(vec3 pPos, int pId) : Position(pPos), Index(pId),
		Life(60), Age(60),
		Size(1.0f),
		Emissive(Color::black()),
		IsShining(false)
	{}

	void Update(float frames, float step, bool zaxis)
	{
		float x = math<float>::cos(Index*step);
		float y = math<float>::sin(Index*step);

		float t = (float)frames*0.1f;
		float offset = math<float>::sin((Index*step*8.f) + t)*0.1f;
		float size = math<float>::sin((Index*step*2.f) + t)*0.1f;

		if (zaxis)
			Position = vec3(x + x*offset, y + y*offset, 0.0);
		else
			Position = vec3(x + x*offset, 0.0, y + y*offset);
		Size = math<float>::max(size*10.0f, 0.5f);

		if ((offset >= 0.099f) && (IsShining == false))
			IsShining = true;

		if (IsShining)
		{
			Age--;
			if (Age > 0)
			{
				float norm = Age / (float)Life;
				Emissive = Color::white()*norm;
			}
			else if (Age <= 0)
			{
				Age = Life;
				IsShining = false;
				Emissive = Color::black();
			}
		}
	}
};
#endif