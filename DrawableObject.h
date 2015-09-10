#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>

class DrawableObject
{
public:
	DrawableObject() {};

	DrawableObject(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn,
		float scaleX, float scaleY, float rotation = 0.0f) :
		framesOfAnimation(4), 
		framesToBeShownPerSecond(4)
	{
		this->scale.x = scaleX;
		this->scale.y = scaleY;
		texture = buttonSpriteSheet;
		laodAnimation(scaleX, scaleY);

		position = positionIn;
		dimensions.x = animation->getFrameWidth();
		dimensions.y = animation->getFrameHeight();
		updateBoundingRect();
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

	virtual void Update(float elapsed)
	{
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

	void resize(float scaleX, float scaleY)
	{
		float tmpScaleX = scaleX / this->scale.x;
		float tmpScaleY = scaleY / this->scale.y;
		this->dimensions.x *= tmpScaleX;
		this->dimensions.y *= tmpScaleY;
		this->position.x *= tmpScaleX;
		this->position.y *= tmpScaleY;
		this->scale.x = scaleX;
		this->scale.y = scaleY;
		laodAnimation(scaleX, scaleY);
		updateBoundingRect();
	}

	void laodAnimation(float scaleX, float scaleY, float rotation = 0)
	{
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, scaleX, scaleY, 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);
	}

	virtual void updateBoundingRect()
	{
		//TODO: updating when rotate
		boundingRectangle.X = position.x;
		boundingRectangle.Y = position.y;
		boundingRectangle.Width = dimensions.x;
		boundingRectangle.Height = dimensions.y;
	}

public:
	int													framesOfAnimation;
	int													framesToBeShownPerSecond;
	DirectX::XMFLOAT2									scale;
	DirectX::XMFLOAT2									position;
	DirectX::XMFLOAT2									dimensions;
	Windows::Foundation::Rect							boundingRectangle;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	texture;
	std::unique_ptr<AnimatedTexture>					animation;
};