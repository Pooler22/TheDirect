#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "Button.h"

class Skill
{
public:
	Skill()
	{
		this->life = this->speedMove = this->point = this->shotDistance = this->shotSpeed = 0;
	}

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