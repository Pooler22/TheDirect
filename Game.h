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
#include "Level.h"

class Game
{
public:
	Game::Game(int screenWidth, int screenHeight, float scaleX, float scaleY)
	{
		this->levels.reset(new std::vector<Level>());
		this->map.reset(new Map());
		this->enemies.reset(new std::vector<Enemy>());
		this->bonus.reset(new std::vector<Bonus>());
		this->screenHeight = screenHeight;
		this->screenWidth = screenWidth;
		this->scaleX = scaleX;
		this->scaleY = scaleY;
	};

	void Game::addLevel(std::wstring name, std::wstring next, DirectX::XMINT2 dimension, std::shared_ptr<int>  tab, DirectX::XMINT2 playerStartPosition, std::shared_ptr<std::vector<DirectX::XMINT4>> vectorEnemyStartPosition)
	{
		this->levels->push_back(Level(name,next, dimension, tab, playerStartPosition, vectorEnemyStartPosition));
	}
	
	void prepareMap(ID3D11ShaderResourceView* brickSpriteSheetIn, std::shared_ptr<SpriteFont> spriteFontIn)
	{
		this->brickSpriteSheet = brickSpriteSheetIn;
		this->spriteFontIn = spriteFontIn;
	}

	void loadLevel(std::wstring name)
	{
		for (std::vector<Level>::iterator it = levels->begin(); it != levels->end(); ++it)
		{
			if (name.compare(it->getName()) == 0)
			{
				nextLevelName = it->getNext();
				this->map->setMapLevel(it->getDimension().x, it->getDimension().y, it->getTab().get(), this->screenWidth, this->screenHeight, scaleX, scaleY, brickSpriteSheet, spriteFontIn);
				this->player->setStartPositionExt(DirectX::XMINT2(it->getPlayerStartPosition().x * (screenWidth / map->getSzie().x), it->getPlayerStartPosition().y * (screenHeight / map->getSzie().y)));

				for (std::vector<DirectX::XMINT4>::iterator it1 = it->getVectorEnemyStartPosition().get()->begin(); it1 != it->getVectorEnemyStartPosition().get()->end(); ++it1)
				{
					this->enemies->push_back(Enemy(enemySpriteSheet,DirectX::XMFLOAT2(it1->x * (screenWidth / map->getSzie().x), it1->y * (screenHeight / map->getSzie().y)), scaleX, scaleY, it1->z, it1->w));
				}
				break;
			}
		}	
	}

	void Game::addPlayerTexture(ID3D11ShaderResourceView* buttonSpriteSheet, ID3D11ShaderResourceView* shotSpriteSheet)
	{
		this->player = std::unique_ptr<Player>(new Player(buttonSpriteSheet, DirectX::XMFLOAT2(0,0), scaleX, scaleY, shotSpriteSheet));
	}

	void Game::addEnemyTexture(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		this->enemySpriteSheet = buttonSpriteSheet;
	}

	void Game::addBonusTexture(ID3D11ShaderResourceView* buttonSpriteSheet, std::shared_ptr<Skill> bonus)
	{
		this->bonus->push_back(Bonus(buttonSpriteSheet, DirectX::XMFLOAT2(0,0), scaleX, scaleY, bonus));
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
		
		for (std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end();)
		{
			if (player->shotColision(it->getBoundingRectangle(), it->getPoint()))
			{
				it = enemies->erase(it);
			}
			else
			{
				it->correctPersonPosition(screenWidth, screenHeight);
				it->Update(elapsed);
				++it;
			}
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
		screenWidth*= scaleX;
		screenHeight *= scaleY;
	}

	void Draw(DirectX::SpriteBatch* batch)
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

	bool win()
	{
		return (enemies->size() == 0);
	}

	void loadNextLevel()
	{
		for (std::vector<Level>::iterator it = levels->begin(); it != levels->end(); ++it)
		{
			
			if (nextLevelName.compare(it->getName()) == 0)
			{
				nextLevelName = it->getNext();
				this->map.reset(new Map());
				break;
			}
		}
		loadLevel(nextLevelName);
	}

	int getScore()
	{
		return player->getScore();
	}

	void Game::addBrickTexture(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		this->map->addBrickTexture(buttonSpriteSheet);
	}

	void Game::addBrickTexture2(ID3D11ShaderResourceView* buttonSpriteSheet)
	{
		this->map->addBrickTexture2(buttonSpriteSheet);
	}

	int										screenWidth;
	int										screenHeight;
	float									scaleX;
	float									scaleY;
	std::wstring							nextLevelName;
	
	std::unique_ptr<Map>					map;
	std::unique_ptr<Player>					player;
	std::unique_ptr<std::vector<Enemy>>		enemies;
	std::unique_ptr<std::vector<Bonus>>		bonus;
	std::unique_ptr<std::vector<Level>>		levels;
	
	std::shared_ptr<SpriteFont>				spriteFontIn;
	ID3D11ShaderResourceView*				playerSpriteSheetIn;
	ID3D11ShaderResourceView*				brickSpriteSheet;
	ID3D11ShaderResourceView*				enemySpriteSheet;
};