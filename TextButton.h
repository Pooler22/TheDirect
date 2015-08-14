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
		this->id = inId;
		this->string = inString;
		this->colorNormal = this->color = Colors::Black;
		this->colorOver = Colors::Blue;
		this->m_font = spriteFont;
		centerHorizontally();
		updateBoundingRect();
	}

	void centerHorizontally()
	{
		position.x -= dimensions.x / 2.;
		position.y -= dimensions.y / 2.;
	}

	bool isOver(Windows::Foundation::Rect rect)
	{
		if (this->boundingRectangle.IntersectsWith(rect))
		{
			this->color = this->colorOver;
			return true;
		}
		else
		{
			this->color = this->colorNormal;
			return false;
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		animation->Draw(batch, position);
		m_font->DrawString(batch, string.c_str(), XMFLOAT2(position.x + 16 * scaleX, position.y + 4 * scaleY), color);
	}

	void setString(std::wstring string)
	{
		this->string = string;
	}

	std::wstring getString()
	{
		return this->string;
	}

	void setId(std::wstring string)
	{
		this->id = string;
	}

	std::wstring getId()
	{
		return this->id;
	}

private:
	std::wstring							id;
	std::wstring							string;
	DirectX::XMVECTOR						color;
	DirectX::XMVECTOR						colorNormal;
	DirectX::XMVECTOR						colorOver;
	std::shared_ptr<DirectX::SpriteFont>	m_font;
};