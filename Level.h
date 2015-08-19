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
	Level(std::wstring name, std::wstring next, DirectX::XMINT2 dimension, std::shared_ptr<int> tab)
	{
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

private:
	std::shared_ptr<int> tab;
	std::wstring	name;
	std::wstring	next;
	DirectX::XMINT2 dimension;
};