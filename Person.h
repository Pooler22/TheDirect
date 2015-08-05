#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Button.h"

class Person : public Button
{
public:
	Person(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> playerSpriteSheet, DirectX::XMFLOAT2 positionIn) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		float rotation = 0.f;
		float scale = 1.f;

		texture = playerSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, 1, 0.5f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		position = positionIn;
		dimensions.x = textureRectangle.Width = animation->getFrameWidth() / 3;
		dimensions.y = textureRectangle.Height = animation->getFrameHeight() / 3;
		updateBoundingRect();
		speed = 10;
	}

	void move(float x, float y)
	{
		position.x = position.x + (x * speed);
		position.y = position.y - (y * speed);
		updateBoundingRect();
	}

public:

	int		speed;
	int		framesOfAnimation;
	int		framesToBeShownPerSecond;
};