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
		this->bricks = std::vector<std::shared_ptr<Brick>>();
		//this->textureVector = std::shared_ptr<std::vector<ID3D11ShaderResourceView*>>();
	};

	void addBrickTexture(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		texture = buttonSpriteSheet;
		//this->textureVector->push_back(buttonSpriteSheet);
	}

	void addBrickTexture2(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		texture2 = buttonSpriteSheet;
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

	bool isColision(Windows::Foundation::Rect rect)
	{
		for (auto &brick : bricks)
		{
			//if (brick->getColision(rect) != COLISION_TYPE::COLISION_TYPE_FALSE)
			//{
			//	return true;
			//}
		}
		return false;
	}

	unsigned int getColision(Windows::Foundation::Rect rect)
	{
		unsigned int opt = 0x0;
		for (auto &brick : bricks)
		{
		//	opt |= brick->getColision(rect);
		}
		return opt;
	}

	void setStringText(std::wstring string)
	{
		this->scoreText = string;
	}

	bool isStanding(Windows::Foundation::Rect rect)
	{
		for (auto &brick : bricks)
		{
		//	if (brick->isStanding(rect))
			//	return true;
		}
		return false;
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
		//getTextureDiension(texture).x;
		textPosition.x = 0;
		textPosition.y = 0;
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y -1; y++)
			{
				if(numberTestureVector[(int)((y * size.x) + x)]== 0)
					bricks.push_back(std::shared_ptr<Brick>(new Brick(texture, XMFLOAT2(x * (screenWidth / size.x),(y+1) * (screenHeight/ size.y)), scaleX, scaleY ,size, BRICK_BEHAVIOR_NONE)));
				else
					bricks.push_back(std::shared_ptr<Brick>(new Brick(texture2, XMFLOAT2(x * (screenWidth / size.x), (y+1) * (screenHeight / size.y)), scaleX, scaleY, size, BRICK_BEHAVIOR_BLOCK)));
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

	int*																	numberTestureVector;
	float																	standingPlatformHeight;
	XMFLOAT2																size;
	XMFLOAT2																textPosition;
	ID3D11ShaderResourceView*												texture;
	ID3D11ShaderResourceView*												texture2;
	std::wstring															scoreText;
	std::shared_ptr<DirectX::SpriteFont>									m_font;
	std::vector<std::shared_ptr<Brick>>										bricks;
	std::shared_ptr<std::vector<ID3D11ShaderResourceView*>>					textureVector;
};