#pragma once


#include "pch.h"
#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Button.h"

class Menu
{
public:
public:
	Menu() {}
	void Add(std::unique_ptr<Button> button)
	{
		buttons.push_back((button));
	}

	void Update(float elapsed)
	{
		for (auto wallsIterator = buttons.begin(); wallsIterator < buttons.end(); wallsIterator++)
		{
			(*wallsIterator)->Update((float)elapsed);
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		for (auto &wall : buttons)
		{
			wall->Draw(batch);
		}
	}

private:
	std::vector<std::unique_ptr<Button>>	buttons;
};