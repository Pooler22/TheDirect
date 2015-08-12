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
	Game::Game(int screenWidth, int screenHeight, float scaleX, float scaleY)
	{
		this->map.reset(new Map());
		this->enemies.reset(new std::vector<Enemy>());
		this->bonus.reset(new std::vector<Bonus>());
		this->screenHeight = screenHeight;
		this->screenWidth = screenWidth;
		this->scaleX = scaleX;
		this->scaleY = scaleY;
	};

	void Game::setMapLevel(int x, int y, int* numberTestureVectorIn, ID3D11ShaderResourceView* playerSpriteSheetIn, std::shared_ptr<SpriteFont> spriteFontIn)
	{
		this->map->setMapLevel(x, y, numberTestureVectorIn, this->screenWidth, this->screenHeight, scaleX, scaleY, playerSpriteSheetIn, spriteFontIn);
	}
	
	void Game::addPlayer(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn)
	{
		this->player = std::unique_ptr<Player>(new Player(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), scaleX, scaleY));
	}

	void Game::addEnemy(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, int moveDirection)
	{
		this->enemies->push_back(Enemy(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), scaleX, scaleY, moveDirection));
	}
	
	void Game::addBonus(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, std::shared_ptr<Skill> bonus)
	{
		this->bonus->push_back(Bonus(buttonSpriteSheet, DirectX::XMFLOAT2(positionIn.x * (screenWidth / map->getSzie().x), positionIn.y * (screenHeight / map->getSzie().y)), scaleX, scaleY, bonus));
	}

	void Game::Update(float elapsed)
	{
		for (auto brick : map->bricks)
		{
			if (brick->getBehavior() == BRICK_BEHAVIOR_BLOCK) 
			{
				player->colision(brick->getBoundingRectangle());
				for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
				{
					it->colision(brick->getBoundingRectangle());
				}
				for (std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end(); ++it)
				{
					it->colision(brick->getBoundingRectangle());
				}
			}
		}

		playerVsEnemyColision();
		playerVsBonusColision();
		player->correctPersonPosition(screenWidth, screenHeight);
		
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			it->correctPersonPosition(screenWidth, screenHeight);
			it->Update(elapsed);
		}
		for (std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end(); ++it)
		{
			it->correctPersonPosition(screenWidth, screenHeight);
			it->Update(elapsed);
		}
		updateScore();
		this->map->Update(elapsed);
		this->player->Update(elapsed);
	}

	void updateScore()
	{
		this->map->setStringText(L"Life: " + std::to_wstring(player->getLife()) + L" Score: " + std::to_wstring(player->getScore()));
	}

	void playerVsEnemyColision() 
	{
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			if (player->boundingRectangle.IntersectsWith(it->boundingRectangle))
			{
				this->player->die();
			}
		}
	}
	
	void playerVsBonusColision()
	{
		for ( std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end();)
		{
			if (this->player->boundingRectangle.IntersectsWith(it->boundingRectangle))
			{
				this->player->addBonus(it->getBonus());
				it = bonus->erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	
	void resize(float scaleX, float scaleY)
	{
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		map->resize(scaleX, scaleY);
		player->resize(scaleX, scaleY);
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
		{
			it->resize(scaleX, scaleY);
		}
		for (std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end(); ++it)
		{
			it->resize(scaleX, scaleY);
		}
		//TODO: ?
		screenHeight *= scaleX;
	}

	void Game::Draw(DirectX::SpriteBatch* batch)
	{
		map->Draw(batch);
		player->Draw(batch);
		for (std::vector<Bonus>::iterator it = bonus->begin(); it != bonus->end(); ++it)
		{
			it->Draw(batch);
		}
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); ++it)
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

	void Game::addBrickTexture(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		this->map->addBrickTexture(buttonSpriteSheet);
	}

	void Game::addBrickTexture2(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		this->map->addBrickTexture2(buttonSpriteSheet);
	}

	bool									moveDown;
	int										screenWidth;
	int										screenHeight;
	float scaleX;
	float scaleY;

	std::unique_ptr<Map>					map;
	std::unique_ptr<Player>					player;
	std::unique_ptr<std::vector<Enemy>>		enemies;
	std::unique_ptr<std::vector<Bonus>>		bonus;
};