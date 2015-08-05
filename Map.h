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

	void addBrickTexture(ID3D11ShaderResourceView* playerSpriteSheet)
	{
		//textureVector->push_back((std::shared_ptr<ID3D11ShaderResourceView>(playerSpriteSheet)));
		texture = playerSpriteSheet;
	}

	void addBrickTexture2(ID3D11ShaderResourceView*playerSpriteSheet)
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

	void setMapLevel(XMFLOAT2 sizeIn, int* numberTestureVectorIn, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVectorIn, int screenWidth, int screenHeight)
	{
		size = sizeIn;
		numberTestureVector = numberTestureVectorIn;
		baehaviorTestureVector = baehaviorTestureVectorIn;
		generateMap(screenWidth, screenHeight);
	}

	void generateMap(int screenWidth, int screenHeight)
	{
		for (int x = 0; x < size.x; x++) 
		{
			for (int y = 0; y < size.y; y++)
			{
				if(numberTestureVector[x*y]== 0)
					bricks.push_back(std::shared_ptr<Brick>(new Brick(texture.Get(), XMFLOAT2(x * (screenWidth/size.x),y * (screenHeight/ size.y)), screenWidth, screenHeight,size)));
				else
					bricks.push_back(std::shared_ptr<Brick>(new Brick(texture2.Get(), XMFLOAT2(x * (screenWidth / size.x), y * (screenHeight / size.y)), screenWidth, screenHeight, size)));
			}
		}
	}

	void resize(float scale)
	{
		for (auto &brick : bricks)
		{
			brick->resize(scale);
		}
	}

private:

	XMFLOAT2																size;
	int*																	numberTestureVector;
	std::wstring															name;
	std::shared_ptr<std::vector<BRICK_BEHAVIOR>>							baehaviorTestureVector;
	std::vector<std::shared_ptr<Brick>>										bricks;
	std::shared_ptr<std::vector<std::shared_ptr<ID3D11ShaderResourceView>>>	textureVector;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>						texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>						texture2;
};