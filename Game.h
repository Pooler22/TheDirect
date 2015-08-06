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

	void Game::addBrickTexture(ID3D11ShaderResourceView* playerSpriteSheet)
	{
		map->addBrickTexture(playerSpriteSheet);
	}

	void Game::addBrickTexture2(ID3D11ShaderResourceView* playerSpriteSheet)
	{
		map->addBrickTexture2(playerSpriteSheet);
	}

	void Game::setMapLevel(int x, int y, int* numberTestureVectorIn, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVectorIn, int screenWidthIn, int screenHeightIn)
	{
		screenWidth = screenWidthIn,
		screenHeight = screenHeightIn;
		map->setMapLevel(x,y, numberTestureVectorIn, baehaviorTestureVectorIn, screenWidthIn, screenHeightIn);
	}
	
	void Game::addPlayer(ID3D11ShaderResourceView* playerSpriteSheet, DirectX::XMFLOAT2 positionIn)
	{
		player = std::unique_ptr<Person>(new Person(playerSpriteSheet,positionIn));
	}

	void Game::Update(float elapsed)
	{
		correctPlayerPosition();
		map->Update(elapsed);
		player->Update(elapsed);
		isColision();
	}

	void correctPlayerPosition()
	{
		if (player->getPosition().y > screenHeight)
		{
			player->setPosition(XMFLOAT2(player->getPosition().x, 0.0 - player->getDimension().y));
		}
		else if (player->getPosition().y < 0.0 - player->getDimension().y)
		{
			player->setPosition(XMFLOAT2(player->getPosition().x, screenHeight));
		}
		if (player->getPosition().x > screenWidth)
		{
			player->setPosition(XMFLOAT2(0.0 - player->getDimension().x, player->getPosition().y));
		}
		else if (player->getPosition().x < 0.0 - player->getDimension().x)
		{
			player->setPosition(XMFLOAT2(screenWidth, player->getPosition().y));
		}
	}

	void Game::isColision()
	{
		if ((map->isColision(player->getBoundingRectangle().X + player->getBoundingRectangle().Width, player->getBoundingRectangle().Y + player->getBoundingRectangle().Height) == BRICK_BEHAVIOR_BLOCK)
			|| (map->isColision(player->getBoundingRectangle().X, player->getBoundingRectangle().Y + player->getBoundingRectangle().Height) == BRICK_BEHAVIOR_BLOCK))
		{
		
			player->setPosition(XMFLOAT2(player->position.x, player->position.y-1));
			player->setStand(true);
		}
		else 
		{
			player->setStand(false);
		}
	}

	void resize(float x, float y)
	{
		map->resize(x, y);
		player->resize(x, y);
	}

	void Game::Draw(DirectX::SpriteBatch* batch)
	{
		map->Draw(batch);
		player->Draw(batch);
	}

	int										screenWidth;
	int										screenHeight;
	std::unique_ptr<Map>					map;
	std::unique_ptr<Person>					player;
	std::unique_ptr<std::vector<Person>>	enemies;
	std::unique_ptr<std::vector<Person>>	bonus;
	std::unique_ptr<int>					score;
};