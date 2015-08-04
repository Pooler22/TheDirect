#pragma once

#include <wrl.h>
#include <future>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Map.h"
//#include "..\Common\DirectXHelper.h"	// For ThrowIfaFailed and ReadDataAsync

class Game
{
public:
	Game()
	{
		map = std::unique_ptr<Map>(new Map());
	};
	~Game() {};

	void addBrickTexture(ID3D11ShaderResourceView* playerSpriteSheet)
	{
		map->addBrickTexture(playerSpriteSheet);
	}

	void setMapLevel(XMFLOAT2 sizeIn, std::shared_ptr<std::vector<int>> numberTestureVectorIn, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVectorIn)
	{
		map->setMapLevel(sizeIn, numberTestureVectorIn, baehaviorTestureVectorIn);
	}

	void Update(float elapsed)
	{
		map->Update(elapsed);
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		map->Draw(batch);
	}

private:
	std::unique_ptr<Map>	map;
	
};