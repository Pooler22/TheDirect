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
		numberTestureVector = std::shared_ptr<std::vector<int>>();
		baehaviorTestureVector = std::shared_ptr<std::vector<BRICK_BEHAVIOR>>();
		bricks = std::vector<std::shared_ptr<Brick>>();
		textureVector = std::shared_ptr<std::vector<ID3D11ShaderResourceView>>();
	};
	~Map() {};

	void addBrickTexture(ID3D11ShaderResourceView* playerSpriteSheet)
	{
		//textureVector->push_back(*playerSpriteSheet);
		texture = playerSpriteSheet;
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
		for (auto &button : bricks)
		{
			if (button->isColision(x, y))
			{
				return button->getBehavior();
			}
		}
		return BRICK_BEHAVIOR_NONE;
	}

	void setMapLevel(XMFLOAT2 sizeIn, std::shared_ptr<std::vector<int>> numberTestureVectorIn, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVectorIn)
	{
		size = sizeIn;
		numberTestureVector = numberTestureVectorIn;
		baehaviorTestureVector = baehaviorTestureVectorIn;
		generateMap();
	}

	void generateMap()
	{
		for (int x = 0; x < size.x; x++) 
		{
			for (int y = 0; y < size.y; y++)
			{
				bricks.push_back(std::shared_ptr<Brick>(new Brick(texture,XMFLOAT2(x*100.0,y*100.0))));
			}
		}
	}


private:
	XMFLOAT2													size;
	std::shared_ptr<std::vector<int>>							numberTestureVector;
	std::shared_ptr<std::vector<BRICK_BEHAVIOR>>				baehaviorTestureVector;

	std::wstring												name;
	std::vector<std::shared_ptr<Brick>>							bricks;
	std::shared_ptr<std::vector<ID3D11ShaderResourceView>>		textureVector;
	ID3D11ShaderResourceView*			texture;

};