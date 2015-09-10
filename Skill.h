#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "DrawableObject.h"

class Skill
{
public:
	Skill(){}

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