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

	Button(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn) : 
		framesOfAnimation(4), 
		framesToBeShownPerSecond(4)
	{
		float rotation = 0.0f;
		float scale = 1.f;

		texture = buttonSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, scale, 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		position = positionIn;
		dimensions.x = textureRectangle.Width = animation->getFrameWidth();
		dimensions.y = textureRectangle.Height = animation->getFrameHeight();
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

	void Update(float elapsed)
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

	void resize(float x, float y)
	{
		position.x *= x;
		position.y *= y;
		updateBoundingRect();
	}

protected:

	void updateBoundingRect()
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
	
	DirectX::XMFLOAT2									position;
	DirectX::XMFLOAT2									dimensions;
	
	Windows::Foundation::Rect							boundingRectangle;
	Windows::Foundation::Rect							textureRectangle;
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	texture;
	std::unique_ptr<AnimatedTexture>					animation;
};