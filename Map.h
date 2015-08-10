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
		bricks = std::vector<std::shared_ptr<Brick>>();
		textureVector = std::shared_ptr<std::vector<std::shared_ptr<ID3D11ShaderResourceView>>>();
	};

	void addBrickTexture(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		texture = buttonSpriteSheet;
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
		m_font->DrawString(batch, textString.c_str(), textPosition, Colors::Blue);
	}
	void setName(std::wstring stringIn)
	{
		name = stringIn;
	}

	std::wstring getName()
	{
		return name;
	}

	void setSize(XMFLOAT2 sizeIn)
	{
		size = sizeIn;
	}

	XMFLOAT2 getSzie()
	{
		return size;
	}

	bool isColision(Windows::Foundation::Rect rect)
	{
		for (auto &brick : bricks)
		{
			if (brick->getColision(rect) == COLISION_TYPE::COLISION_TYPE_TRUE && brick->behavior == BRICK_BEHAVIOR_BLOCK)
			{
				return true;
			}
		}
		return false;
	}

	unsigned int getColision(Windows::Foundation::Rect rect)
	{
		unsigned int opt = 0x0;
		for (auto &brick : bricks)
		{
			if (brick->behavior == BRICK_BEHAVIOR_BLOCK)
			{
				opt |= brick->getColision(rect);
			}
		}
		return opt;
	}

	void setStringText(std::wstring string)
	{
		textString = string;
	}

	bool isStanding(Windows::Foundation::Rect rect)
	{
		for (auto &brick : bricks)
		{
			if (brick->isStanding(rect))
				return true;
		}
		return false;
	}

	void setMapLevel(int x, int y, int* numberTestureVectorIn, int screenWidth, int screenHeight, float scale, ID3D11ShaderResourceView* playerSpriteSheetIn, SpriteFont* spriteFontIn)
	{
		size.x = x;
		size.y = y;
		m_font.reset(spriteFontIn);
		numberTestureVector = numberTestureVectorIn;
		generateMap(screenWidth, screenHeight, scale);

	}

	void generateMap(int screenWidth, int screenHeight, float scale)
	{
		getTextureDiension(texture).x;
		textPosition.x = 0;
		textPosition.y = 0;
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y -1; y++)
			{
				if(numberTestureVector[(int)((y * size.x) + x)]== 0)
					bricks.push_back(std::shared_ptr<Brick>(new Brick(texture, XMFLOAT2(x * (screenWidth/size.x),(y+1) * (screenHeight/ size.y)), scale ,size, BRICK_BEHAVIOR_NONE)));
				else
					bricks.push_back(std::shared_ptr<Brick>(new Brick(texture2, XMFLOAT2(x * (screenWidth / size.x), (y+1) * (screenHeight / size.y)), scale, size, BRICK_BEHAVIOR_BLOCK)));
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

	void resize(float scale)
	{
		for (auto &brick : bricks) 
		{
			brick->resize(scale);
		}
	}

	float																	standingPlatformHeight;
private:
	
	XMFLOAT2																size;
	int*																	numberTestureVector;

	std::wstring															name;
	std::vector<std::shared_ptr<Brick>>										bricks;
	std::shared_ptr<std::vector<std::shared_ptr<ID3D11ShaderResourceView>>>	textureVector;
	ID3D11ShaderResourceView*												texture;
	ID3D11ShaderResourceView*												texture2;
	std::unique_ptr<DirectX::SpriteFont>									m_font;
	std::wstring															textString;
	XMFLOAT2																textPosition;
};