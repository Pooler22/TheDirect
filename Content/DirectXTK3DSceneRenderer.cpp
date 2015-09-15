﻿#include "pch.h"
#include "DirectXTK3DSceneRenderer.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "..\Common\DirectXHelper.h"	// For ThrowIfFailed and ReadDataAsync

using namespace MindBuster;
using namespace DirectX;
using namespace Windows::Foundation;

DirectXTK3DSceneRenderer::DirectXTK3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
    m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
    CreateAudioResources();
}

//this is the LOAD function
void DirectXTK3DSceneRenderer::CreateDeviceDependentResources()
{
	// Create DirectXTK objects
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();
	//auto windowSize = m_deviceResources->GetOutputSize(); // physical screen resolution
	logicalSize = m_deviceResources->GetLogicalSize(); //DPI dependent resolution

	m_sprites.reset(new SpriteBatch(context));
	m_font.reset(new SpriteFont(device, L"assets\\italic.spritefont"));

	flagFromPressToRelasedClick = true;
	m_playMusic = false;

	scaleX = (float)(logicalSize.Width / (32.0 * 25.0));
	scaleY = (float)(logicalSize.Height / (19.0 * 25.0));
	centerPosition.x = (float)(logicalSize.Width / 2.0);
	centerPosition.y = (float)(logicalSize.Height / 2.0);
	float oneUnitHeight = (float)(logicalSize.Height / 6.0);

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\ui\\button.png", nullptr, m_texture.ReleaseAndGetAddressOf())
		);
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\background\\background.png", nullptr, m_texture2.ReleaseAndGetAddressOf())
		);

	screenManager.reset(new ScreenManager(L"Main", logicalSize.Width, logicalSize.Height, scaleX, scaleY, m_texture.Get(), m_font, m_texture2.Get()));

	std::wstring name[] = { L"Start",L"Options",L"Exit" };
	std::wstring id[] = { L"StartMain",L"OptionsMain",L"ExitMain" };
	XMFLOAT2 position[] = { XMFLOAT2(centerPosition.x, centerPosition.y - oneUnitHeight), XMFLOAT2(centerPosition.x, centerPosition.y), XMFLOAT2(centerPosition.x, centerPosition.y + oneUnitHeight) };
	screenManager->addScreen(L"Main", 3, name, id, position);

	std::wstring name1[] = { L"Music: on",L"Author",L"Back" };
	std::wstring id1[] = { L"MusicOptions",L"AuthorOptions",L"BackOptions" };
	XMFLOAT2 position1[] = { XMFLOAT2(centerPosition.x, centerPosition.y - oneUnitHeight), XMFLOAT2(centerPosition.x, centerPosition.y), XMFLOAT2(centerPosition.x, centerPosition.y + oneUnitHeight) };
	screenManager->addScreen(L"Options", 3, name1, id1, position1);

	std::wstring name2[] = { L"Single",L"Back" };
	std::wstring id2[] = { L"SingleplayerLevel",L"BackLevel" };
	XMFLOAT2 position2[] = { XMFLOAT2(centerPosition.x, centerPosition.y - oneUnitHeight), XMFLOAT2(centerPosition.x, centerPosition.y)};
	screenManager->addScreen(L"Level", 2, name2, id2, position2);

	std::wstring name4[] = { L"Return", L"Main menu" };
	std::wstring id4[] = { L"ReturnPause",L"ExitPause" };
	XMFLOAT2 position4[] = { XMFLOAT2(centerPosition.x, centerPosition.y - oneUnitHeight), XMFLOAT2(centerPosition.x, centerPosition.y)};
	screenManager->addScreen(L"Pause", 2, name4, id4, position4);

	std::wstring name5[] = { L"Pawel Rutkowski",L"Back" };
	std::wstring id5[] = { L"DescriptionAuthor",L"BackAuthor" };
	XMFLOAT2 position5[] = { XMFLOAT2(centerPosition.x, centerPosition.y - oneUnitHeight), XMFLOAT2(centerPosition.x, centerPosition.y) };
	screenManager->addScreen(L"Author", 2, name5, id5, position5);

	std::wstring name3[] = { L"Pause" };
	std::wstring id3[] = { L"PausePlay" };
	XMFLOAT2 position3[] = { XMFLOAT2(centerPosition.x, 40) };
	screenManager->addScreen(L"Play", 1, name3, id3, position3);

	std::wstring name6[] = { L"Score",  L"Continue" };
	std::wstring id6[] = { L"ScoreGameOver" , L"ContinueGameOver" };
	XMFLOAT2 position6[] = { XMFLOAT2(centerPosition.x, centerPosition.y - oneUnitHeight),XMFLOAT2(centerPosition.x, centerPosition.y) };
	screenManager->addScreen(L"GameOver", 2, name6, id6, position6);

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\brick\\brick.png", nullptr, m_texture2.ReleaseAndGetAddressOf())
		);
	screenManager->addBrickTexture(m_texture2.Get());

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\brick\\brick2.png", nullptr, m_texture.ReleaseAndGetAddressOf())
		);
	screenManager->addBrickTexture(m_texture.Get());

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\bubble\\bubble.png", nullptr, m_texture2.ReleaseAndGetAddressOf())
		);
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\person\\player\\player.png", nullptr, m_texture.ReleaseAndGetAddressOf())
		);
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\person\\player\\player_jump.png", nullptr, m_texture3.ReleaseAndGetAddressOf())
		);
	screenManager->addPlayerTexture(m_texture.Get(), m_texture2.Get(), m_texture3.Get());

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\person\\enemy\\enemy.png", nullptr, m_texture.ReleaseAndGetAddressOf())
		);
	screenManager->addEnemyTexture(m_texture.Get());

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"assets\\person\\bonus\\bonus.png", nullptr, m_texture.ReleaseAndGetAddressOf())
		);
	screenManager->addBonusTexture(m_texture.Get());

	


	int x1 = 32;
	int y1 = 18;

	std::vector<int> tab1 = {
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
		1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
		1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL1;
	enemyStartPositionL1.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL1->push_back(DirectX::XMINT4(1, 7, 1, 10));
	enemyStartPositionL1->push_back(DirectX::XMINT4(29, 7, 1, 10));
	screenManager->addLevel(L"1", L"2", DirectX::XMINT2(x1, y1), tab1, XMINT2(2, 17), enemyStartPositionL1);

	std::vector<int> tab2 = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,2,
		2,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		2,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,2
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL2;
	enemyStartPositionL2.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL2->push_back(DirectX::XMINT4(15, 3, 1, 10));
	enemyStartPositionL2->push_back(DirectX::XMINT4(15, 7, -1, 10));
	enemyStartPositionL2->push_back(DirectX::XMINT4(15, 11, 1, 10));
	screenManager->addLevel(L"2", L"3", DirectX::XMINT2(x1, y1), tab2, XMINT2(3, 15), enemyStartPositionL2);

	std::vector<int> tab3 = {
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
		1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL3;
	enemyStartPositionL3.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL3->push_back(DirectX::XMINT4(10, 6, 1, 10));
	enemyStartPositionL3->push_back(DirectX::XMINT4(21, 6, -1, 10));
	enemyStartPositionL3->push_back(DirectX::XMINT4(5, 6, 1, 10));
	enemyStartPositionL3->push_back(DirectX::XMINT4(26, 6, -1, 10));
	enemyStartPositionL3->push_back(DirectX::XMINT4(7, 6, 1, 10));
	enemyStartPositionL3->push_back(DirectX::XMINT4(24, 6, -1, 10));
	screenManager->addLevel(L"3", L"4", DirectX::XMINT2(x1, y1), tab3, XMINT2(2, 17), enemyStartPositionL3);

	std::vector<int> tab4 = {
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
		1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL4;
	enemyStartPositionL4.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL4->push_back(DirectX::XMINT4(1, 0, 1, 10));
	enemyStartPositionL4->push_back(DirectX::XMINT4(2, 2, 1, 10));
	enemyStartPositionL4->push_back(DirectX::XMINT4(29, 0, -1, 10));
	enemyStartPositionL4->push_back(DirectX::XMINT4(27, 2, -1, 10));
	enemyStartPositionL4->push_back(DirectX::XMINT4(10, 8, 1, 10));
	enemyStartPositionL4->push_back(DirectX::XMINT4(21, 8, -1, 10));
	screenManager->addLevel(L"4", L"5", DirectX::XMINT2(x1, y1), tab4, XMINT2(2, 17), enemyStartPositionL4);

	std::vector<int> tab5 = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL5;
	enemyStartPositionL5.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL5->push_back(DirectX::XMINT4(15, 5, 1, 10));  
	enemyStartPositionL5->push_back(DirectX::XMINT4(8, 13, 1, 10));
	screenManager->addLevel(L"5", L"6", DirectX::XMINT2(x1, y1), tab5, XMINT2(1, 13), enemyStartPositionL5); // column // row

	std::vector<int> tab6 = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL6;
	enemyStartPositionL6.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL6->push_back(DirectX::XMINT4(1, 16, 1, 10));  
	enemyStartPositionL6->push_back(DirectX::XMINT4(30, 16, -1, 10));
	enemyStartPositionL6->push_back(DirectX::XMINT4(30, 11, -1, 10));
	enemyStartPositionL6->push_back(DirectX::XMINT4(1, 11, 1, 10));
	screenManager->addLevel(L"6", L"7", DirectX::XMINT2(x1, y1), tab6, XMINT2(16, 1), enemyStartPositionL6); // column // row

	std::vector<int> tab7 = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
		0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
		0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,
		0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
		0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL7;
	enemyStartPositionL7.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL7->push_back(DirectX::XMINT4(12, 1, 1, 10));  
	enemyStartPositionL7->push_back(DirectX::XMINT4(12, 31, -1, 10));
	enemyStartPositionL7->push_back(DirectX::XMINT4(3, 31, 1, 10));
	enemyStartPositionL7->push_back(DirectX::XMINT4(3, 1, -1, 10));
	screenManager->addLevel(L"7", L"8", DirectX::XMINT2(x1, y1), tab7, XMINT2(1, 16), enemyStartPositionL7); // column // row

	std::vector<int> tab8 = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL8;
	enemyStartPositionL8.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL8->push_back(DirectX::XMINT4(9, 14, 1, 10));  
	enemyStartPositionL8->push_back(DirectX::XMINT4(10, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(11, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(12, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(13, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(14, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(15, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(16, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(17, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(18, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(19, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(20, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(21, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(22, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(23, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(24, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(25, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(26, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(27, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(28, 16, 1, 10));
	enemyStartPositionL8->push_back(DirectX::XMINT4(29, 16, -1, 10));

	screenManager->addLevel(L"8", L"9", DirectX::XMINT2(x1, y1), tab8, XMINT2(8, 16), enemyStartPositionL8); // column // row

	std::vector<int> tab9 = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,
		0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,
		0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL9;
	enemyStartPositionL9.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL9->push_back(DirectX::XMINT4(3, 13, 1, 10));  
	enemyStartPositionL9->push_back(DirectX::XMINT4(11, 13, 1, 10));
	enemyStartPositionL9->push_back(DirectX::XMINT4(20, 13, -1, 10));
	enemyStartPositionL9->push_back(DirectX::XMINT4(28, 13, -1, 10));

	screenManager->addLevel(L"9", L"10", DirectX::XMINT2(x1, y1), tab9, XMINT2(16, 2), enemyStartPositionL9); // column // row

	std::vector<int> tab10 = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,
		0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
		0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionL10;
	enemyStartPositionL10.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionL10->push_back(DirectX::XMINT4(2, 1, 1, 10));  
	enemyStartPositionL10->push_back(DirectX::XMINT4(3, 1, -1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(4, 1, 1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(5, 1, -1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(6, 1, 1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(7, 1, -1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(8, 1, 1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(9, 1, -1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(10, 1, 1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(11, 1, -1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(12, 1, 1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(13, 1, -1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(14, 1, 1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(15, 1, -1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(16, 1, 1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(17, 1, -1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(18, 1, 1, 10));
	enemyStartPositionL10->push_back(DirectX::XMINT4(19, 1, -1, 10));

	screenManager->addLevel(L"10", L"1", DirectX::XMINT2(x1, y1), tab10, XMINT2(3, 15), enemyStartPositionL10); // column // row

	std::vector<int> tabSecret = {
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,1,
		1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,0,1,1,0,0,1,0,1,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,1,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,0,0,0,0,0,1,
		1,0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,1,0,0,0,0,1,
		1,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,0,1,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0,1,
		1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0,1,
		1,0,0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,1,0,0,1,0,0,1,0,1,0,0,0,0,1
	};
	std::shared_ptr<std::vector<DirectX::XMINT4>> enemyStartPositionLSecret;
	enemyStartPositionLSecret.reset(new std::vector<DirectX::XMINT4>());
	enemyStartPositionLSecret->push_back(DirectX::XMINT4(2, 3, -1, 10));

	screenManager->addLevel(L"Secret", L"1", DirectX::XMINT2(x1, y1), tabSecret, XMINT2(10, 2), enemyStartPositionLSecret);

	

	//Gamepad
	//GamePad.reset(new GamePad);
}

// Initializes view parameters when the window size changes.
void DirectXTK3DSceneRenderer::CreateWindowSizeDependentResources()
{
    Size outputSize = m_deviceResources->GetOutputSize(); 
    float aspectRatio = outputSize.Width / outputSize.Height; // If aspectRatio < 1.0f app is usually in snapped view (mode). Act accordingly

	// MUST BE DONE FOR EVERY SPRITEBATCH
	m_sprites->SetRotation( m_deviceResources->ComputeDisplayRotation() ); // necessary for the sprites to be in correct rotation when the
																			//screen rotation changes (portrait/landscape)
}

void DirectXTK3DSceneRenderer::CreateAudioResources()
{
    // Create DirectXTK for Audio objects
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif

    m_audEngine.reset(new AudioEngine(eflags));

    m_audioEvent = 0;
    m_audioTimerAcc = 10.f;
    m_retryDefault = false;

    m_waveBank.reset(new WaveBank(m_audEngine.get(), L"assets\\music\\ADPCMdroid.xwb"));
    m_soundEffect.reset(new SoundEffect(m_audEngine.get(), L"assets\\music\\musicmono_adpcm.wav"));
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);

	m_effect1->Play(true);
	m_effect2->Play();
}

// Updates the scene to be displayed.
void DirectXTK3DSceneRenderer::Update(DX::StepTimer const& timer, std::vector<PlayerInputData>* playerInputs, unsigned int playersAttached)
{
	m_audioTimerAcc -= (float)timer.GetElapsedSeconds();
	if (m_audioTimerAcc < 0)
	{
		if (m_retryDefault)
		{
			m_retryDefault = false;
			if (m_audEngine->Reset())
			{
				// Restart looping audio
				m_effect1->Play(true);
			}
		}
		else
		{
			m_audioTimerAcc = 4.f;
			m_waveBank->Play(m_audioEvent++);
			if (m_audioEvent >= 11)
				m_audioEvent = 0;
		}
	}

	if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
	{
		// Setup a retry in 1 second
		m_audioTimerAcc = 1.f;
		m_retryDefault = true;
	}

	if (screenManager->win())
	{
		screenManager->loadNextLevel();
		//screenManager->setString(L"GameOver", L"ScoreGameOver", std::to_wstring(screenManager->game->getScore()));
		//screenManager->setName(L"GameOver");
	}
	if (screenManager->gameOver())
	{
		screenManager->setString(L"GameOver", L"ScoreGameOver", L"Score: "  + std::to_wstring(screenManager->game->getScore()));
		screenManager->setName(L"GameOver");
	}
	
	//#pragma region Gamepad
	//	//GamePad
	//	auto statePlayerOne = GameePad->GetState(0);
	//	if (statePlayerOne.IsConnected())
	//	{
	//		XMFLOAT2 tempPos = player->getPosition();
	//		if (statePlayerOne.IsDPadUpPressed()){
	//			tempPos.y -= 10; //CHANGE TO PROPER OFFSET CALCULATION - USING TIME 
	//		}
	//
	//		if (statePlayerOne.IsDPadDownPressed()){
	//			tempPos.y += 10; ////CHANGE TO PROPER OFFSET CALCULATION - USING TIME 
	//		}
	//		
	//		if (statePlayerOne.IsDPadLeftPressed()){
	//			tempPos.x -= 10; //CHANGE TO PROPER OFFSET CALCULATION - USING TIME 
	//		}
	//		if (statePlayerOne.IsDPadRightPressed()){
	//			tempPos.x += 10; //CHANGE TO PROPER OFFSET CALCULATION - USING TIME 	
	//		}
	//		player->setPosition(tempPos);	
	//	}
	//#pragma endregion Handling the Enginepad Input

	//auto test = timer.GetElapsedSeconds();

	//update the animation
	//animation->Update((float)timer.GetElapsedSeconds());
	//player->Update((float)timer.GetElapsedSeconds());

//#pragma region Enemy AI
	// TODO: handle enemy AI using promises and Lambdas
	//std::vector<std::future<DirectX::XMFLOAT2>> futures;
	//for (auto enemy : enemiesVector)
	//{
	//	futures.push_back( std::async(std::launch::async,
	//		[]()
	//	{
	//		DirectX::XMFLOAT2 tempPos;
	//		//TODO: Write code for very complicated AI here
	//		return tempPos;
	//	}) );
	//}

	//for (auto &future : futures)
	//{
		//TODO:get results
		//auto enemiesIterator = enemiesVector.begin();

	//	DirectX::XMFLOAT2 tempPos;
	//	tempPos = future.get();
		//(*enemiesIterator).setPosition(tempPos);
		//enemiesIterator++;
	//}
//#pragma endregion Handling Enemy AI using std::async and std::Future. Also using C++11 Lambdas

//#pragma region Collisions
	//collisionString = L"There is no collision";
	//EnginePad->SetVibration(0, 0.f, 0.f);
	/*for (auto wallsIterator = wallsVector.begin(); wallsIterator < wallsVector.end(); wallsIterator++)
	{

	(*wallsIterator).Update((float)timer.GetElapsedSeconds());
	if ((*wallsIterator).isCollidingWith(player->rectangle)){
	collisionString = L"There is a collision with the wall";

	EnginePad->SetVibration(0, 0.75f, 0.75f);
	}
	}*/
	screenManager->Update((float)timer.GetElapsedSeconds());

//#pragma endregion Handling collision detection + simple EnginePad rumble on crash

	m_playersAttached = playersAttached;

	for (unsigned int i = 0; i < XINPUT_MAX_CONTROLLERS; i++)
	{
		unsigned int playerAttached = (playersAttached & (1 << i));

		if (!playerAttached)
			continue;

		for (unsigned int j = 0; j < playerInputs->size(); j++)
		{
			PlayerInputData playerAction = (*playerInputs)[j];

			if (playerAction.ID != i) continue;
			switch (playerAction.PlayerAction)
			{
			case PLAYER_ACTION_TYPES::INPUT_FIRE_PRESSED:
				break;
			case PLAYER_ACTION_TYPES::INPUT_FIRE_DOWN:
				if (flagFromPressToRelasedClick)
				{
					if (screenManager->getName() == L"Play")
					{
						screenManager->game->fire();
					}
					if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"StartMain"))
					{
						screenManager->setName(L"Level");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"OptionsMain"))
					{
						screenManager->setName(L"Options");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"ExitMain"))
					{
						//TO DO: exit
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"MusicOptions"))
					{
						m_playMusic = !m_playMusic;
						if (m_playMusic)
						{
							m_effect1->Play(true);
							m_effect2->Play();
							screenManager->setString(L"Options", L"MusicOptions", L"Music: on");
						}
						else
						{
							m_effect1->Pause();
							m_effect2->Pause();
							screenManager->setString(L"Options", L"MusicOptions", L"Music: off");
						}
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"AuthorOptions"))
					{
						screenManager->setName(L"Author");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"BackOptions"))
					{
						screenManager->setName(L"Main");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"SingleplayerLevel"))
					{
						screenManager->resetLevel();
						screenManager->loadLevel(L"1");
						screenManager->setName(L"Play");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"BackLevel"))
					{
						screenManager->setName(L"Main");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"PausePlay"))
					{
						screenManager->setName(L"Pause");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"ReturnPause"))
					{
						screenManager->setName(L"Play");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"ExitPause"))
					{
						screenManager->setName(L"Main");
						screenManager->resetLevel();
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"DescriptionAuthor"))
					{
						screenManager->resetLevel();
						screenManager->loadLevel(L"Secret");
						screenManager->setName(L"Play");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"BackAuthor"))
					{
						screenManager->setName(L"Options");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"ContinueGameOver"))
					{
						screenManager->setName(L"Main");
						screenManager->resetLevel();
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"RestartPause"))
					{
						screenManager->setName(L"Play");
						screenManager->game->resetLevel();
					}
					flagFromPressToRelasedClick = false;
				}
				break;
			case PLAYER_ACTION_TYPES::INPUT_CANCEL:
				break;
			case PLAYER_ACTION_TYPES::INPUT_FIRE_RELEASED:
				flagFromPressToRelasedClick = true;
				break;

			case PLAYER_ACTION_TYPES::INPUT_JUMP_PRESSED:
				break;
			case PLAYER_ACTION_TYPES::INPUT_JUMP_DOWN:
				break;
			case PLAYER_ACTION_TYPES::INPUT_JUMP_RELEASED:
				break;

			case PLAYER_ACTION_TYPES::INPUT_MOVE:
				screenManager->game->player->move(playerAction.X, playerAction.Y);
				if (playerAction.Y == 1)
					screenManager->game->player->jump();
				break;
			case PLAYER_ACTION_TYPES::INPUT_AIM:
				break;
			case PLAYER_ACTION_TYPES::INPUT_BRAKE:
				break;

			default:
				screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY);
				break;
			}
		}

		wchar_t intStringBuffer[8];
		size_t sizeInWords = sizeof(intStringBuffer) / 2;
		_itow_s(i + 1, intStringBuffer, sizeInWords, 10);
		std::wstring playerIdString(intStringBuffer);

		//m_text[i] = L"Input Player" + playerIdString += L": " + inputText;

		/*DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextLayout(
		m_text[i].c_str(),
		(uint32)m_text[i].length(),
		m_textFormat.Get(),
		DEBUG_INPUT_TEXT_MAX_WIDTH,
		DEBUG_INPUT_TEXT_MAX_HEIGHT,
		&m_textLayout[i]
		)
		);
		DX::ThrowIfFailed(
		m_textLayout[i]->GetMetrics(&m_textMetrics[i])
		);*/
	}
}

void DirectXTK3DSceneRenderer::NewAudioDevice()
{
    if (m_audEngine && !m_audEngine->IsAudioDevicePresent())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
}

// Drawing Grid on screen using primitives (PrimitiveBatch) - not usefull right now
//void XM_CALLCONV DirectXTK3DSceneRenderer::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
//{
//    auto context = m_deviceResources->GetD3DDeviceContext();
//    m_batchEffect->Apply(context);
//
//    context->IASetInputLayout(m_batchInputLayout.Get());
//
//    m_batch->Begin();
//
//    xdivs = std::max<size_t>(1, xdivs);
//    ydivs = std::max<size_t>(1, ydivs);
//
//    for (size_t i = 0; i <= xdivs; ++i)
//    {
//        float fPercent = float(i) / float(xdivs);
//        fPercent = (fPercent * 2.0f) - 1.0f;
//        XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
//        vScale = XMVectorAdd(vScale, origin);
//
//        VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
//        VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
//        m_batch->DrawLine(v1, v2);
//    }
//
//    for (size_t i = 0; i <= ydivs; i++)
//    {
//        FLOAT fPercent = float(i) / float(ydivs);
//        fPercent = (fPercent * 2.0f) - 1.0f;
//        XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
//        vScale = XMVectorAdd(vScale, origin);
//
//        VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
//        VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
//        m_batch->DrawLine(v1, v2);
//    }
//
//   m_batch->End();
//}
//this is the DRAW function

void DirectXTK3DSceneRenderer::Render()
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	// Set render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	//D3D11_TEXTURE2D_DESC pDesc;
	//Microsoft::WRL::ComPtr<ID3D11Resource> res;
	
	// get texture size 
	//1.) -----------------
	//m_texture->GetResource(&res);
	//((ID3D11Texture2D*)res.Get())->GetDesc(&pDesc); // Usually dangerous!

	//2.) -----------------
	//m_texture->GetResource(res.GetAddressOf());
	//m_texture2->GetResource(res.GetAddressOf());
	//Microsoft::WRL::ComPtr<ID3D11Texture2D> text2D;
	//res.As(&text2D);
	//text2D->GetDesc(&pDesc);

	//auto height = pDesc.Height; //texture height
	//auto width = pDesc.Width; //texture width

	//auto windowSize = m_deviceResources->GetOutputSize(); // physical screen resolution
	auto logicalSize = m_deviceResources->GetLogicalSize(); //DPI dependent resolution

	if ((this->logicalSize.Width != logicalSize.Width) || (this->logicalSize.Height != logicalSize.Height))
	{
		screenManager->resize(logicalSize.Width, logicalSize.Height, logicalSize.Width / (32.0 * 25.0), logicalSize.Height / (19.0 * 25.0));
	}
	
	// Draw sprites
	m_sprites->Begin();
	screenManager->Draw(m_sprites.get());
	m_sprites->End();
}

void DirectXTK3DSceneRenderer::ReleaseDeviceDependentResources()
{
    m_sprites.reset();
    m_font.reset();
    m_texture.Reset();
	screenManager.reset();
}