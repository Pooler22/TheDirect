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
	TextButton(ID3D11ShaderResourceView* buttonSpriteSheet, SpriteFont *spriteFont, std::wstring inString, std::wstring inId, XMFLOAT2 inPosition, float scaleIn) : 
		Button(buttonSpriteSheet,  inPosition, scaleIn),
		framesToBeShownPerSecond(4),
		framesOfAnimation(4)
	{
		id = inId;
		string = inString;
		m_font.reset(spriteFont);
		colorNormal = color = Colors::Black;
		colorOver = Colors::Blue;
	}

	bool isOver(float x, float y) 
	{
		if (x > (boundingRectangle.X) && y > (boundingRectangle.Y) && x < (boundingRectangle.X + boundingRectangle.Width) && y < (boundingRectangle.Y + boundingRectangle.Height))
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

public:

	int										framesOfAnimation;
	int										framesToBeShownPerSecond;

	std::wstring							string;
	std::wstring							id;
	DirectX::XMVECTOR						color;
	DirectX::XMVECTOR						colorNormal;
	DirectX::XMVECTOR						colorOver;
	std::unique_ptr<DirectX::SpriteFont>	m_font;
};