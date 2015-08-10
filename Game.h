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
#include "Enemy.h"
#include "Player.h"
#include "Bonus.h"

class Game
{
public:
	Game::Game(int screenWidth, int screenHeight)
	{
		map.reset(new Map());
		enemies.reset(new std::vector<Enemy>());
		bonus.reset(new std::vector<Bonus>());
		this->screenHeight = screenHeight;
		this->screenWidth = screenWidth;
	};

	void Game::addBrickTexture(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		map->addBrickTexture(buttonSpriteSheet);
	}

	void Game::addBrickTexture2(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		map->addBrickTexture2(buttonSpriteSheet);
	}

	void Game::setMapLevel(int x, int y, int* numberTestureVectorIn, float scaleIn, ID3D11ShaderResourceView* playerSpriteSheetIn, SpriteFont* spriteFontIn)
	{
		map->setMapLevel(x,y, numberTestureVectorIn, screenWidth, screenHeight, scaleIn, playerSpriteSheetIn, spriteFontIn);
	}
	
	void Game::addPlayer(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float sizeIn)
	{
		player = std::unique_ptr<Player>(new Player(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), sizeIn));
	}

	void Game::addEnemy(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float sizeIn, int moveDirection)
	{
		enemies->push_back(Enemy(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), sizeIn, moveDirection));
	}
	void Game::addBonus(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float sizeIn, std::shared_ptr<SKILL_T> bonus)
	{
		this->bonus->push_back(Bonus(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), sizeIn, bonus));
		//bonus_copy->push_back(Bonus(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), sizeIn));
	}


	void Game::Update(float elapsed)
	{
		map->setStringText(L"Life: " + std::to_wstring(player->getLife()) + L" Score: " + std::to_wstring(player->getScore()));
		isColision();
		playerVsEnemyColision();
		playerVsBonusColision();
		//playerVsBonusColision();
		correctPlayerPosition();
		
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			isColision(it);
			correctPlayerPosition(it);
			it->Update(elapsed);
		}
		for (std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end(); ++it)
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
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
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
		for ( std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end();)
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
	

	void correctPlayerPosition(std::vector<Bonus>::iterator person)
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
	
	void correctPlayerPosition(std::vector<Enemy>::iterator person)
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

	void Game::getColision()
	{
		if (map->getColision(player->boundingRectangle) == COLISION_TYPE::COLISION_TYPE_LEFT)
		{
			player->setBlockDirection(1);
		}
		else
		{
			player->setBlockDirection(0);
		}

		if (map->getColision(player->boundingRectangle) == COLISION_TYPE::COLISION_TYPE_RIGHT)
		{
			player->setBlockDirection(2);
		}
		else
		{
			player->setBlockDirection(0);
		}

		if (map->getColision(player->boundingRectangle) == COLISION_TYPE::COLISION_TYPE_TRUE)
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

	virtual void Game::isColision(std::vector<Bonus>::iterator person)
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
	virtual void Game::isColision(std::vector<Enemy>::iterator person)
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
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			it->resize(scale);
		}
		for (std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end(); ++it)
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
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			it->Draw(batch);
		}
		for (std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end(); ++it)
		{
			it->Draw(batch);
		}
	}

	bool gameOver()
	{
		return player->gameOver();
	}

	int getScore()
	{
		return player->getScore();
	}

	void resetLevel()
	{
		player->resetLevel();
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			it->resetLevel();
		}

		for (std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end(); ++it)
		{
			it->resetLevel();
		}
	}

	void copyVecFast(std::vector<Bonus> original) // no reference
	{

		std::vector<Bonus> new_;
		new_.swap(original);
	}

	bool									moveDown;
	int										screenWidth;
	int										screenHeight;
	std::unique_ptr<Map>					map;

	std::unique_ptr<Player>					player;
	std::unique_ptr<std::vector<Enemy>>		enemies;
	std::unique_ptr<std::vector<Bonus>>		bonus;
};