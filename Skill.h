#pragma once

#include <wrl.h>

class Skill
{
public:
	Skill(int life, int speedMove, int point, int shotDistance, int shotSpeed)
	{
		this->life = life;
		this->speedMove = speedMove;
		this->point = point;
		this->shotDistance = shotDistance;
		this->shotSpeed = shotSpeed;
	}

	int life;
	int speedMove;
	int point;
	int shotDistance;
	int shotSpeed;	
};