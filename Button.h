#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>

class Button
{
public:
	Button::Button() 
	{
	};

	Button(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scale) : 
		framesOfAnimation(4), 
		framesToBeShownPerSecond(4)
	{
		float rotation = 0.0f;

		texture = buttonSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, scale, 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		position = positionIn;
		dimensions.x = animation->getFrameWidth();
		dimensions.y = animation->getFrameHeight();
		updateBoundingRect();
	}

	void Button::setPosition(DirectX::XMFLOAT2 positionIn)
	{
		position = positionIn;
		updateBoundingRect();
	}

	DirectX::XMFLOAT2 Button::getPosition()
	{
		return position;
	}

	DirectX::XMFLOAT2 Button::getDimension()
	{
		return dimensions;
	}

	bool Button::isColision(Windows::Foundation::Rect rect)
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

	void Button::Update(float elapsed)
	{
		animation->Update(elapsed);
	}

	void Button::Draw(DirectX::SpriteBatch* batch)
	{
		animation->Draw(batch, position);
	}

	Windows::Foundation::Rect Button::getBoundingRectangle()
	{
		return boundingRectangle;
	}

	void Button::resize(float scaleIn)
	{
		dimensions.x *= scaleIn;
		dimensions.y *= scaleIn;
		position.x *= scaleIn;
		position.y *= scaleIn;
		scale = scaleIn;
		updateBoundingRect();
	}

public:

	void Button::updateBoundingRect()
	{
		//TODO: proper updating when rotating player object
		boundingRectangle.X = position.x;
		boundingRectangle.Y = position.y;
		boundingRectangle.Width = dimensions.x;
		boundingRectangle.Height = dimensions.y;
	}

public:

	int													framesOfAnimation;
	int													framesToBeShownPerSecond;
	
	float												scale;
	DirectX::XMFLOAT2									position;
	DirectX::XMFLOAT2									dimensions;
	
	Windows::Foundation::Rect							boundingRectangle;
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	texture;
	std::unique_ptr<AnimatedTexture>					animation;
};