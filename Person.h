#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"

class Person
{
public:
	Person(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		float rotation = 0.f;
		float scale = 1.f;

		texture = buttonSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, 1, 0.5f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);
		
		position = positionIn;
		dimensions.x = textureRectangle.Width = animation->getFrameWidth() / 3;
		dimensions.y = textureRectangle.Height = animation->getFrameHeight() / 3;
		updateBoundingRect();
		speed = 10;
		gravity = 1;
		stand = false;
	}

	void setPosition(DirectX::XMFLOAT2 positionIn)
	{
		position = positionIn;
		updateBoundingRect();
	}

	DirectX::XMFLOAT2 getPosition()
	{
		return position;
	}

	DirectX::XMFLOAT2 getDimension()
	{
		return dimensions;
	}


	void Update(float elapsed)
	{
		if (!stand)
		{
			move(0, -gravity);
		}
		
		animation->Update(elapsed);
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		animation->Draw(batch, position);
	}

	Windows::Foundation::Rect getBoundingRectangle()
	{
		return boundingRectangle;
	}

	void setStand(bool standIn)
	{
		stand = standIn;
	}

	void move(float x, float y)
	{
		position.x = position.x + (x * speed);
		position.y = position.y - (y * speed);
		updateBoundingRect();
	}

	void resize(float x, float y)
	{
		position.x *= x;
		position.y *= y;
		updateBoundingRect();
	}

private:

	void updateBoundingRect()
	{
		//TODO: proper updating when rotating player object
		boundingRectangle.X = position.x;
		boundingRectangle.Y = position.y;
		boundingRectangle.Width = dimensions.x;
		boundingRectangle.Height = dimensions.y;
	}

public:
	bool												stand;
	int													speed;
	int													gravity;
	int													framesOfAnimation;
	int													framesToBeShownPerSecond;
	DirectX::XMFLOAT2									position;
	DirectX::XMFLOAT2									dimensions;
	Windows::Foundation::Rect							boundingRectangle;
	Windows::Foundation::Rect							textureRectangle;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	texture;
	std::unique_ptr<AnimatedTexture>					animation;
};