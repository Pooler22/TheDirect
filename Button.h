#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"

class Button
{
public:
	Button(ID3D11ShaderResourceView* playerSpriteSheet, SpriteFont *spriteFont, std::wstring inString, std::wstring inId, XMFLOAT2 inPosition) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		float rotation = 0.f;
		float scale = 3.f;

		id = inId;
		string = inString;
		position = inPosition;

		texture = playerSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, 3, 0.5f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		color = Colors::Black;
		m_font.reset(spriteFont);

		width = textureWidth = animation->getFrameWidth();
		height = textureHeight = animation->getFrameHeight();
		position.x -= width/2;
		position.y -= height/2;
		updateBoundingRect();

	}

	void setPosition(DirectX::XMFLOAT2 newPosition)
	{
		position = newPosition;
		updateBoundingRect();
	}

	void setPosition(float posX, float posY)
	{
		position.x = posX;
		position.y = posY;
		updateBoundingRect();
	}

	DirectX::XMFLOAT2 getPosition()
	{
		return position;
	}

	DirectX::XMFLOAT2 getDimension()
	{
		return DirectX::XMFLOAT2(width, height);
	}

	bool isClicked(float x, float y) 
	{
		if (x > (boundingRectangle.X) && y > (boundingRectangle.Y) && x < (boundingRectangle.X + boundingRectangle.Width) && y < (boundingRectangle.Y + boundingRectangle.Height))
		{
			color = Colors::Blue;
			return true;
		}
		else
		{
			color = Colors::Black;
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
		m_font->DrawString(batch, string.c_str(), position, color);
	}

	void setString(std::wstring in)
	{
		string = in;
	}

	std::wstring getString()
	{
		return string;
	}

	void setId(std::wstring in)
	{
		id = in;
	}

	std::wstring getId()
	{
		return id;
	}

	Windows::Foundation::Rect getBoundingRect()
	{
		return boundingRectangle;
	}

	void updatePosition(float x, float y)
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
		boundingRectangle.Height = height;
		boundingRectangle.Width = width;
	}

public:
	
	int													width;
	int													height;
	int													textureWidth;
	int													textureHeight;
	int													framesOfAnimation;
	int													framesToBeShownPerSecond;
	Windows::Foundation::Rect							boundingRectangle;
	DirectX::XMFLOAT2									position;
	
	std::wstring										string;
	std::wstring										id;
	
	std::unique_ptr<DirectX::SpriteFont>				m_font;
	DirectX::XMVECTOR									color;
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	texture;
	std::unique_ptr<AnimatedTexture>					animation;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pressTexture;
	std::unique_ptr<AnimatedTexture>					presAnimation;
};