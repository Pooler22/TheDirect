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
	{}

	Person::Person(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		float rotation = 0.f;
		scale = scaleIn;

		texture = buttonSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, scaleIn, 0.5f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		bubbles = std::vector<Button>();
		
		dimensions.x = animation->getFrameWidth();
		dimensions.y = animation->getFrameHeight();
		startPosition.x = position.x = positionIn.x;
		startPosition.y = position.y = positionIn.y - dimensions.y;
		updateBoundingRect();
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
	
	/*void  Person::addBonus(std::shared_ptr<Bonus_struct> bs)
	{
		score += bs->score;
		speed += bs->speed;
	}*/

	void resetLevel()
	{
		position = startPosition;
		updateBoundingRect();
	}

public:
	DirectX::XMFLOAT2	startPosition;
	bool		jumpFlag;
	int			jumpTime;
	bool		moveDown;
	bool		stand;
	int			speed;
	float		gravity;
	int			framesOfAnimation;
	int			framesToBeShownPerSecond;
	std::vector<Button> bubbles;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	bubbleTexture;
};