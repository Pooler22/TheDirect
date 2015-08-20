#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"

class Level
{
public:
	Level(std::wstring name, std::wstring next, DirectX::XMINT2 dimension, std::shared_ptr<int> tab, DirectX::XMINT2 playerStartPosition, std::shared_ptr<std::vector<DirectX::XMINT4>> vectorEnemyStartPosition)
	{
		this->vectorEnemyStartPosition.reset((new std::vector<DirectX::XMINT4>()));
		this->vectorEnemyStartPosition = std::move(vectorEnemyStartPosition);
		this->playerStartPosition = playerStartPosition;
		this->next = next;
		this->name = name;
		this->dimension = dimension;
		this->tab = tab;
		
	}

	void setName(std::wstring stringIn)
	{
		this->name = stringIn;
	}

	std::wstring getName()
	{
		return this->name;
	}

	void setNext(std::wstring next)
	{
		this->next = next;
	}

	std::wstring getNext()
	{
		return this->next;
	}

	void setTab(std::shared_ptr<int>  tab)
	{
		this->tab = tab;
	}

	std::shared_ptr<int>  getTab()
	{
		return this->tab;
	}

	void setDimension(DirectX::XMINT2 dimension)
	{
		this->dimension = dimension;
	}

	DirectX::XMINT2 getDimension()
	{
		return this->dimension;
	}

	void setVectorEnemyStartPosition(std::shared_ptr<std::vector<DirectX::XMINT4>> vectorEnemyStartPosition)
	{
		this->vectorEnemyStartPosition = vectorEnemyStartPosition;
	}

	std::shared_ptr<std::vector<DirectX::XMINT4>> getVectorEnemyStartPosition()
	{
		return this->vectorEnemyStartPosition;
	}

	void setPlayerStartPosition(DirectX::XMINT2 playerStartPosition)
	{
		this->playerStartPosition = playerStartPosition;
	}

	DirectX::XMINT2 getPlayerStartPosition()
	{
		return this->playerStartPosition;
	}

private:
	std::shared_ptr<int> tab;
	std::wstring	name;
	std::wstring	next;
	DirectX::XMINT2 dimension;
	DirectX::XMINT2 playerStartPosition;
	std::shared_ptr<std::vector<DirectX::XMINT4>> vectorEnemyStartPosition;

};