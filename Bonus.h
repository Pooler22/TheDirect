#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "Button.h"

class Bonus : public Person
{
public:
	Bonus::Bonus()
	{}

	Bonus::Bonus(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn) : framesOfAnimation(4), framesToBeShownPerSecond(4), Person(buttonSpriteSheet, positionIn, scaleIn)
	{
	}

public:
	int			framesOfAnimation;
	int			framesToBeShownPerSecond;
};