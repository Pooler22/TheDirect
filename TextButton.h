#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Button.h"

class TextButton : public Button
{
public:
	TextButton(ID3D11ShaderResourceView* playerSpriteSheet, SpriteFont *spriteFont, std::wstring inString, std::wstring inId, XMFLOAT2 inPosition) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		float rotation = 0.f;
		float scale = 3.f;

		id = inId;
		string = inString;
		position = inPosition;

		texture = playerSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, 3, 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		color = Colors::Black;
		m_font.reset(spriteFont);

		dimensions.x = animation->getFrameWidth();
		dimensions.y = animation->getFrameHeight();
		position.x -= (dimensions.x / 2);
		position.y -= (dimensions.y / 2);
		updateBoundingRect();

		colorNormal = Colors::Black;
		colorOver = Colors::Blue;
	}

	bool isOver(float x, float y) 
	{
		if (isColision(x,y))
		{
			color = colorOver;
			return true;
		}
		else
		{
			color = colorNormal;
			return false;
		}
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

	void Draw(DirectX::SpriteBatch* batch)
	{
		animation->Draw(batch, position);
		m_font->DrawString(batch, string.c_str(), position, color);
	}

public:

	int													framesOfAnimation;
	int													framesToBeShownPerSecond;
	
	std::unique_ptr<DirectX::SpriteFont>				m_font;
	DirectX::XMVECTOR									color;
	DirectX::XMVECTOR									colorNormal;
	DirectX::XMVECTOR									colorOver;
	std::wstring										string;
	std::wstring										id;
	
};