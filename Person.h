#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"

class Person : public Button
{
public:
	Person::Person()
	{
	}

	Person::Person(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn) : 
		Button(buttonSpriteSheet, positionIn, scaleIn)
	{
		bubbles = std::vector<Button>();
		speed = 10;
		gravity = 1;
		stand = false;
		jumpTime = 10;
	}

	virtual void  Person::Draw(DirectX::SpriteBatch* batch)
	{
		for (auto &bubble : bubbles)
		{
			bubble.Draw(batch);
		}
		animation->Draw(batch, position);
	}

	virtual void  Person::Update(float elapsed)
	{
		if(stand && jumpTime > 0)
			jumpTime--;

		if (!stand)
		{
			move(0, -gravity);
		}
		for (auto &bubble : bubbles)
		{
			bubble.Update(elapsed);
		}
		animation->Update(elapsed);
	}

	virtual void  Person::setStand(bool standIn)
	{
		stand = standIn;
	}

	virtual void  Person::move(float x, float y)
	{
		if (!moveDown)
		{
			if (y * speed > 0)
			{
				position.x = position.x + (x * speed);
				position.y = position.y - (y * speed);
			}
			else
			{
				position.x = position.x + (x * speed);
			}
		}
		else
		{
			position.x = position.x + (x * speed);
			position.y = position.y - (y * speed);
		}
		updateBoundingRect();
	}

	void  Person::jump()
	{
		if(stand && jumpTime > 0)
			move(0, 2);
	}

	void  Person::setMoveDown(bool flag)
	{
		moveDown = flag;
	}

	void  Person::setStartPosition()
	{
		position = startPosition;
		updateBoundingRect();
	}
	
	void resetLevel()
	{
		position = startPosition;
		updateBoundingRect();
	}

public:
	DirectX::XMFLOAT2									startPosition;
	bool												jumpFlag;
	int													jumpTime;
	bool												moveDown;
	bool												stand;
	int													speed;
	float												gravity;
	std::vector<Button>									bubbles;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	bubbleTexture;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	textureJump;
	std::unique_ptr<AnimatedTexture>					animationJump;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	textureLeft;
	std::unique_ptr<AnimatedTexture>					animationLeft;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	textureRight;
	std::unique_ptr<AnimatedTexture>					animationRight;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	textureFire;
	std::unique_ptr<AnimatedTexture>					animationFire;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	textureDie;
	std::unique_ptr<AnimatedTexture>					animationDie;
};