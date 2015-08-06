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
		//numberTestureVector = std::vector<int>();
		baehaviorTestureVector = std::shared_ptr<std::vector<BRICK_BEHAVIOR>>();
		bricks = std::vector<std::shared_ptr<Brick>>();
		textureVector = std::shared_ptr<std::vector<std::shared_ptr<ID3D11ShaderResourceView>>>();
	};
	~Map() {};

	void addBrickTexture(ID3D11ShaderResourceView* playerSpriteSheet)
	{
		//textureVector->push_back((std::shared_ptr<ID3D11ShaderResourceView>(playerSpriteSheet)));
		texture = playerSpriteSheet;
	}

	void addBrickTexture2(ID3D11ShaderResourceView* playerSpriteSheet)
	{
		//textureVector->push_back((std::shared_ptr<ID3D11ShaderResourceView>(playerSpriteSheet)));
		texture2 = playerSpriteSheet;
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

	BRICK_BEHAVIOR isColision(float x, float y)
	{
		for (auto &brick : bricks)
		{
			if (brick->isColision(x, y))
			{
				return brick->getBehavior();
			}
		}
		return BRICK_BEHAVIOR_NONE;
	}

	void setMapLevel(int x, int y, int* numberTestureVectorIn, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVectorIn, int screenWidth, int screenHeight)
	{
		size.x = x;
		size.y = y;
		numberTestureVector = numberTestureVectorIn;
		baehaviorTestureVector = baehaviorTestureVectorIn;
		generateMap(screenWidth, screenHeight);
	}

	void generateMap(int screenWidth, int screenHeight)
	{
		getTextureDiension(texture).x;

		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y; y++)
			{
				if(numberTestureVector[(int)((y * size.x) + x)]== 0)
					bricks.push_back(std::shared_ptr<Brick>(new Brick(texture, XMFLOAT2(x * (screenWidth/size.x),y * (screenHeight/ size.y)), screenWidth, screenHeight,size, BRICK_BEHAVIOR_NONE)));
				else
					bricks.push_back(std::shared_ptr<Brick>(new Brick(texture2, XMFLOAT2(x * (screenWidth / size.x), y * (screenHeight / size.y)), screenWidth, screenHeight, size, BRICK_BEHAVIOR_BLOCK)));
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

	void resize(float x, float y)
	{
		for (auto &brick : bricks) 
		{
			brick->resize(x, y);
		}
	}

private:

	XMFLOAT2													size;
	int*														numberTestureVector;
	std::shared_ptr<std::vector<BRICK_BEHAVIOR>>				baehaviorTestureVector;

	std::wstring												name;
	std::vector<std::shared_ptr<Brick>>							bricks;
	std::shared_ptr<std::vector<std::shared_ptr<ID3D11ShaderResourceView>>>	textureVector;
	ID3D11ShaderResourceView*									texture;
	ID3D11ShaderResourceView*									texture2;
};