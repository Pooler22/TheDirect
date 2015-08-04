#pragma once

#include <wrl.h>
#include <future>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Map.h"
#include "Person.h"

class Game
{
public:
	Game()
	{
		map = std::unique_ptr<Map>(new Map());
		enemies = std::unique_ptr<std::vector<Person>>(new std::vector<Person>());
		bonus = std::unique_ptr<std::vector<Person>>(new std::vector<Person>());
		score = 0;
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
	
	void addPlayer(ID3D11ShaderResourceView* playerSpriteSheet, DirectX::XMFLOAT2 positionIn)
	{
		player = std::unique_ptr<Person>(new Person(playerSpriteSheet,positionIn));
	}

	void Update(float elapsed)
	{
		map->Update(elapsed);
		player->Update(elapsed);
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		map->Draw(batch);
		player->Draw(batch);
	}

public:
	std::unique_ptr<Map>					map;
	std::unique_ptr<Person>					player;
	std::unique_ptr<std::vector<Person>>	enemies;
	std::unique_ptr<std::vector<Person>>	bonus;
	std::unique_ptr<int>					score;
	
};