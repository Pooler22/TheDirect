#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Button.h"

enum BRICK_BEHAVIOR
{
	BRICK_BEHAVIOR_NONE,
	BRICK_BEHAVIOR_BLOCK
};

enum COLISION_TYPE
{
	COLISION_TYPE_TRUE,
	COLISION_TYPE_FALSE,
	COLISION_TYPE_STAND
};

class Brick : public Button
{
public:

	Brick::Brick(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, int screenWidth, int screenHeight, XMFLOAT2 sizeIn, BRICK_BEHAVIOR behaviorIn) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		float rotation = 0.0f;
		float scale = 1.f;

		texture = buttonSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, scale, 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);
		//TO DO: resize 
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		texture->GetResource(resource.GetAddressOf());
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
		resource.As(&tex2D);
		D3D11_TEXTURE2D_DESC desc;
		tex2D->GetDesc(&desc);

		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, (((float)screenHeight / ((float)sizeIn.y)) / (float)desc.Height), 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		position = positionIn;
		dimensions.x = textureRectangle.Width = animation->getFrameWidth()/3;
		dimensions.y = textureRectangle.Height = animation->getFrameHeight()/3;
		updateBoundingRect();
		
		behavior = behaviorIn;
	}

	void setBehavior(BRICK_BEHAVIOR behaviorIn) 
	{
		behavior = behaviorIn;
	}

	BRICK_BEHAVIOR getBehavior()
	{
		return behavior;
	}

	bool isColision(Windows::Foundation::Rect rect)
	{
		if (boundingRectangle.IntersectsWith(rect))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

public:
	int													framesOfAnimation;
	int													framesToBeShownPerSecond;

	BRICK_BEHAVIOR										behavior;
};