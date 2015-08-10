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
#include "Button.h"

class Game
{
public:
	Game::Game(int screenWidth, int screenHeight)
	{
		map.reset(new Map());
		enemies.reset(new std::vector<Person>());
		bonus.reset(new std::vector<Person>());
		this->screenHeight = screenHeight;
		this->screenWidth = screenWidth;
		score = 0;
	};

	void Game::addBrickTexture(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		map->addBrickTexture(buttonSpriteSheet);
	}

	void Game::addBrickTexture2(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		map->addBrickTexture2(buttonSpriteSheet);
	}

	void Game::setMapLevel(int x, int y, int* numberTestureVectorIn, float scaleIn)
	{
		map->setMapLevel(x,y, numberTestureVectorIn, screenWidth, screenHeight, scaleIn);
	}
	
	void Game::addPlayer(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float sizeIn)
	{
		player = std::unique_ptr<Person>(new Person(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), sizeIn));
	}

	void Game::addEnemy(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float sizeIn)
	{
		enemies->push_back(Person(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), sizeIn));
	}
	void Game::addBonus(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float sizeIn)
	{
		bonus->push_back(Person(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), sizeIn));
	}


	void Game::Update(float elapsed)
	{
		isColision();
		playerVsEnemyColision();
		playerVsBonusColision();
		//playerVsBonusColision();
		correctPlayerPosition();
		
		for (std::vector<Person>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			isColision(it);
			correctPlayerPosition(it);
			it->Update(elapsed);
		}
		for (std::vector<Person>::iterator it = bonus->begin(); it != bonus->end(); ++it)
		{
			isColision(it);
			correctPlayerPosition(it);
			it->Update(elapsed);
		}

		map->Update(elapsed);
		player->Update(elapsed);
	}

	void playerVsEnemyColision() 
	{
		for (std::vector<Person>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			if (player->boundingRectangle.IntersectsWith(it->boundingRectangle))
			{
				player->die();
			}
		}
	}
	void playerVsBonusColision()
	{
		int i = 0;
		for ( std::vector<Person>::iterator it = bonus->begin(); it != bonus->end();)
		{
			if (player->boundingRectangle.IntersectsWith(it->boundingRectangle))
			{
				it = bonus->erase(it);
				player->speed += 1;
			}
			else
			{
				++it;
			}
			i++;
		}
	}
	

	void correctPlayerPosition(std::vector<Person>::iterator person)
	{
		if (person->getPosition().y > screenHeight)
		{
			person->setPosition(XMFLOAT2(person->getPosition().x, (0.0 - person->getDimension().y)));
		}
		else if (person->getPosition().y < 0.0 - person->getDimension().y)
		{
			person->setPosition(XMFLOAT2(person->getPosition().x, screenHeight));
		}
		if (person->getPosition().x > screenWidth)
		{
			person->setPosition(XMFLOAT2(0.0 - person->getDimension().x, person->getPosition().y));
		}
		else if (person->getPosition().x < 0.0 - person->getDimension().x)
		{
			person->setPosition(XMFLOAT2(screenWidth, person->getPosition().y));
		}
	}
	

	void correctPlayerPosition()
	{
		if (player->getPosition().y > screenHeight)
		{
			player->setPosition(XMFLOAT2(player->getPosition().x, (0.0 - player->getDimension().y)));
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
		if (map->isStanding(player->boundingRectangle))
		{
			player->setStand(true);
			player->setMoveDown(false);
		}
		else 
		{
			player->setStand(false);
			player->setMoveDown(true);
		}
	}

	void Game::isColision(std::vector<Person>::iterator person)
	{
		if (map->isStanding(person->boundingRectangle))
		{
			person->setStand(true);
			person->setMoveDown(false);
		}
		else
		{
			person->setStand(false);
			person->setMoveDown(true);
		}
	}

	void resize(float scale)
	{
		map->resize(scale);
		player->resize(scale);
		for (std::vector<Person>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			it->resize(scale);
		}
		for (std::vector<Person>::iterator it = bonus->begin(); it != bonus->end(); ++it)
		{
			it->resize(scale);
		}
		screenWidth *= scale;
		screenHeight *= scale;
	}

	void Game::Draw(DirectX::SpriteBatch* batch)
	{
		map->Draw(batch);
		player->Draw(batch);
		for (std::vector<Person>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			it->Draw(batch);
		}
		for (std::vector<Person>::iterator it = bonus->begin(); it != bonus->end(); ++it)
		{
			it->Draw(batch);
		}
	}

	bool gameOver()
	{
		return player->gameOver();
	}

	bool									moveDown;
	int										screenWidth;
	int										screenHeight;
	std::unique_ptr<Map>					map;
	std::unique_ptr<int>					score;

	std::unique_ptr<Person>					player;
	std::unique_ptr<std::vector<Person>>	enemies;
	std::unique_ptr<std::vector<Person>>	bonus;
};