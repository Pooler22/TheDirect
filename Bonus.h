#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "Button.h"
#include "Skill.h"

class Bonus : public Person
{
public:
	Bonus::Bonus(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, std::shared_ptr<Skill> bonus) :
		Person(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->bonus = bonus;
	}

	std::shared_ptr<Skill> getBonus()
	{
		return bonus;
	}

	void setBonus(std::shared_ptr<Skill> bonus)
	{
		this->bonus = bonus;
	}

private:
	std::shared_ptr<Skill> bonus;
};