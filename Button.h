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

	Button(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> playerSpriteSheet, DirectX::XMFLOAT2 positionIn) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		rotation = 0.0f;
		scale = 1.f;

		texture = playerSpriteSheet;
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

	void setDiemnstion(DirectX::XMFLOAT2 dimensionIn)
	{
		dimensions = dimensionIn;
	}

	DirectX::XMFLOAT2 getDimension()
	{
		return dimensions;
	}

	Windows::Foundation::Rect getBoundingRectangle()
	{
		return boundingRectangle;
	}

	void Update(float elapsed)
	{
		animation->Update(elapsed);
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		animation->Draw(batch, position);
	}

	bool isColision(float x, float y)
	{
		if (x > (boundingRectangle.X) && y > (boundingRectangle.Y) && x < (boundingRectangle.X + boundingRectangle.Width) && y < (boundingRectangle.Y + boundingRectangle.Height))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void resize(float scale)
	{
		position.x *= scale;
		position.y *= scale;
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

	float												rotation;
	float												scale;

	DirectX::XMFLOAT2									position;
	DirectX::XMFLOAT2									dimensions;
	
	Windows::Foundation::Rect							boundingRectangle;
	Windows::Foundation::Rect							textureRectangle;
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	texture;
	std::unique_ptr<AnimatedTexture>					animation;
};