#pragma once

#include <wrl.h>
#include <future>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "Brick.h"

class Map
{
public:
	Map()
	{
		this->textureVector.reset(new std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>());
	}

	void addBrickTexture(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		this->textureVector->push_back(buttonSpriteSheet);
	}

	void Update(float elapsed)
	{
		for (auto &brick : bricks)
		{
			brick->Update(elapsed);
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		for (auto &brick : bricks)
		{
			brick->Draw(batch);
		}
		this->m_font->DrawString(batch, scoreText.c_str(), textPosition, Colors::Blue);
	}
	
	void setSize(XMFLOAT2 sizeIn)
	{
		this->size = sizeIn;
	}

	XMFLOAT2 getSzie()
	{
		return this->size;
	}

	void setStringText(std::wstring string)
	{
		this->scoreText = string;
	}

	void setMapLevel(int x, int y, int* numberTestureVectorIn, int screenWidth, int screenHeight, float scaleX, float scaleY, ID3D11ShaderResourceView* playerSpriteSheetIn, std::shared_ptr<SpriteFont> spriteFontIn)
	{
		size.x = x;
		size.y = y + 1;
		m_font = spriteFontIn;
		numberTestureVector = numberTestureVectorIn;
		generateMap(screenWidth, screenHeight, scaleX, scaleY);
	}

	void generateMap(int screenWidth, int screenHeight, float scaleX, float scaleY)
	{
		textPosition.x = 0;
		textPosition.y = 0;
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y -1; y++)
			{
				if(numberTestureVector[(int)((y * size.x) + x)] == 1)
					bricks.push_back(std::shared_ptr<Brick>(new Brick(textureVector->begin()->Get(), XMFLOAT2(x * (screenWidth / size.x),(y+1) * (screenHeight/ size.y)), scaleX, scaleY ,size, BRICK_BEHAVIOR_BLOCK)));
				if (numberTestureVector[(int)((y * size.x) + x)] == 2)
					bricks.push_back(std::shared_ptr<Brick>(new Brick(textureVector->at(1).Get(), XMFLOAT2(x * (screenWidth / size.x), (y + 1) * (screenHeight / size.y)), scaleX, scaleY, size, BRICK_BEHAVIOR_BLOCK)));
			}
		}
	}

	XMFLOAT2 getTextureDiension(ID3D11ShaderResourceView* texture)
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		texture->GetResource(resource.GetAddressOf());
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
		resource.As(&tex2D);
		D3D11_TEXTURE2D_DESC desc;
		tex2D->GetDesc(&desc);
		return XMFLOAT2(desc.Width, desc.Height);
	}

	void resize(float scaleX, float scaleY)
	{
		for (auto &brick : bricks) 
		{
			brick->resize(scaleX, scaleY);
		}
	}

	void reset()
	{
		bricks.clear();
	}

	void hardReset()
	{
		bricks.clear();

	}

	int*																				numberTestureVector;
	float																				standingPlatformHeight;
	XMFLOAT2																			size;
	XMFLOAT2																			textPosition;
	std::wstring																		scoreText;
	std::shared_ptr<DirectX::SpriteFont>												m_font;
	std::vector<std::shared_ptr<Brick>>													bricks;
	std::shared_ptr<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>>		textureVector;
};