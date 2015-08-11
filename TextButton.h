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
	TextButton(ID3D11ShaderResourceView* buttonSpriteSheet, std::shared_ptr<DirectX::SpriteFont> spriteFont, std::wstring inString, std::wstring inId, XMFLOAT2 inPosition, float scaleX, float scaleY) :
		Button(buttonSpriteSheet,  inPosition, scaleX, scaleY)
	{
		id = inId;
		string = inString;
		colorNormal = color = Colors::Black;
		colorOver = Colors::Blue;
		m_font = spriteFont;
	}

	bool isOver(float x, float y) 
	{
		if(boundingRectangle.IntersectsWith(Windows::Foundation::Rect(x,y,1,1)))
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
	std::wstring							string;
	std::wstring							id;
	DirectX::XMVECTOR						color;
	DirectX::XMVECTOR						colorNormal;
	DirectX::XMVECTOR						colorOver;
	std::shared_ptr<DirectX::SpriteFont>	m_font;
};