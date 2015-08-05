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
	Game::Game()
	{
		map = std::unique_ptr<Map>(new Map());
		enemies = std::unique_ptr<std::vector<Person>>(new std::vector<Person>());
		bonus = std::unique_ptr<std::vector<Person>>(new std::vector<Person>());
		score = 0;
	};
	Game::~Game() {};

	void Game::addBrickTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> playerSpriteSheet)
	{
		map->addBrickTexture(playerSpriteSheet);
	}

	void Game::addBrickTexture2(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> playerSpriteSheet)
	{
		map->addBrickTexture2(playerSpriteSheet);
	}

	void Game::setMapLevel(DirectX::XMFLOAT2 sizeIn, int* numberTestureVectorIn, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVectorIn, int screenWidth, int screenHeight)
	{
		map->setMapLevel(sizeIn, numberTestureVectorIn, baehaviorTestureVectorIn, screenWidth, screenHeight);
	}
	
	void Game::addPlayer(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> playerSpriteSheet, DirectX::XMFLOAT2 positionIn)
	{
		player = std::unique_ptr<Person>(new Person(playerSpriteSheet,positionIn));
	}

	void Game::Update(float elapsed)
	{
		map->Update(elapsed);
		player->Update(elapsed);
		isColision();
	}

	void Game::isColision()
	{
		if (map->isColision(player->position.x, player->position.y) == BRICK_BEHAVIOR_BLOCK)
		{
			player->speed = 0;
		}
	}

	void resize(float size)
	{
		/*position.x *= x;
		position.y *= y;
		updateBoundingRect();*/
	}

	void Game::Draw(DirectX::SpriteBatch* batch)
	{
		map->Draw(batch);
		player->Draw(batch);
	}

	std::unique_ptr<Map>					map;
	std::unique_ptr<Person>					player;
	std::unique_ptr<std::vector<Person>>	enemies;
	std::unique_ptr<std::vector<Person>>	bonus;
	std::unique_ptr<int>					score;
};