//--------------------------------------------------------------------------------------
// File: DirectXTK3DSceneRenderer.cpp
//
// This is a simple Windows Store app for Windows 8.1 showing use of DirectXTK
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "DirectXTK3DSceneRenderer.h"
#include "DDSTextureLoader.h"
#include "..\Common\DirectXHelper.h"	// For ThrowIfFailed and ReadDataAsync

using namespace SimpleSample;
using namespace DirectX;
using namespace Windows::Foundation;

DirectXTK3DSceneRenderer::DirectXTK3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
    m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
    CreateAudioResources();
}

// Initializes view parameters when the window size changes.
void DirectXTK3DSceneRenderer::CreateWindowSizeDependentResources()
{
    Size outputSize = m_deviceResources->GetOutputSize(); 
    float aspectRatio = outputSize.Width / outputSize.Height; // If aspectRatio < 1.0f app is usually in snapped view (mode). Act accordingly

	// MUST BE DONE FOR EVERY SPRITEBATCH
	m_sprites->SetRotation( m_deviceResources->ComputeDisplayRotation() ); // necessary for the sprites to be in correct rotation when the
																			//screen rotation changes (portrait/landscape)
	spriteBatchT1->SetRotation(m_deviceResources->ComputeDisplayRotation());
	spriteBatchT2->SetRotation(m_deviceResources->ComputeDisplayRotation());
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

    m_waveBank.reset(new WaveBank(m_audEngine.get(), L"assets\\adpcmdroid.xwb"));
    m_soundEffect.reset(new SoundEffect(m_audEngine.get(), L"assets\\MusicMono_adpcm.wav"));
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);

    //m_effect1->Play(true);
    //m_effect2->Play();
}

void DirectXTK3DSceneRenderer::Update(DX::StepTimer const& timer)
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

	//startButtonString = L"Start";

//#pragma region Enginepad
//	//EnginePad
//	auto statePlayerOne = EnginePad->GetState(0);
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

#pragma region Paralaxing background
	//Update Background
	//background->Update((float)timer.GetElapsedSeconds() * 100);
	//clouds->Update((float)timer.GetElapsedSeconds() * 300);
	//clouds2->Update((float)timer.GetElapsedSeconds() * 900);
#pragma endregion Handling the paralaxing backgrounds

	//auto test = timer.GetElapsedSeconds();

	//update the animation
	//animation->Update((float)timer.GetElapsedSeconds());
	//player->Update((float)timer.GetElapsedSeconds());


#pragma region Enemy AI
	// TODO: handle enemy AI using promises and Lambdas
std::vector<std::future<DirectX::XMFLOAT2>> futures;

//for (auto enemy : enemiesVector)
//{
//	futures.push_back( std::async(std::launch::async,
//		[]()
//	{
//		DirectX::XMFLOAT2 tempPos;
//		//TODO: Write code for very complicated AI here
//		
//		
//		return tempPos;

//	}) );
//}

for (auto &future : futures)
{
	//TODO:get results
	//auto enemiesIterator = enemiesVector.begin();

	DirectX::XMFLOAT2 tempPos;
	tempPos = future.get();
	//(*enemiesIterator).setPosition(tempPos);
	//enemiesIterator++;
}

#pragma endregion Handling Enemy AI using std::async and std::Future. Also using C++11 Lambdas

#pragma region Collisions
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

	//screen->Update((float)timer.GetElapsedSeconds());
	//buttons[0]->Update((float)timer.GetElapsedSeconds());
	screenManager->Update((float)timer.GetElapsedSeconds());

#pragma endregion Handling collision detection + simple EnginePad rumble on crash

}

// Updates the scene to be displayed.
void DirectXTK3DSceneRenderer::Update(std::vector<PlayerInputData>* playerInputs, unsigned int playersAttached)
{
	bool flagFromPressToRelasedClick = true;
	m_playersAttached = playersAttached;

	for (unsigned int i = 0; i < XINPUT_MAX_CONTROLLERS; i++)
	{
		std::wstring inputText = L"";

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
				inputText += L"\n FireDown(" + std::to_wstring(playerAction.NormalizedInputValue) + L") ";
				break;
			case PLAYER_ACTION_TYPES::INPUT_FIRE_DOWN:
				inputText += L"\n FireDown(" + std::to_wstring(playerAction.NormalizedInputValue) + L") ";
				if (flagFromPressToRelasedClick)
				{
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
						//TO DO: music
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"AuthorOptions"))
					{
						screenManager->setName(L"Author");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"BackOptions"))
					{
						screenManager->setName(L"Main");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"OfflineLevel"))
					{
						screenManager->setName(L"Play");
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"OnlineLevel"))
					{
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
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"DescriptionAuthor"))
					{
						//TO DO: ?
					}
					else if (screenManager->isClicked(playerAction.PointerRawX, playerAction.PointerRawY) == (L"BackAuthor"))
					{
						screenManager->setName(L"Options");
					}
					flagFromPressToRelasedClick = false;
				}
				break;
			case PLAYER_ACTION_TYPES::INPUT_CANCEL:

			case PLAYER_ACTION_TYPES::INPUT_FIRE_RELEASED:
				inputText += L"\n FirePressed(" + std::to_wstring(playerAction.NormalizedInputValue) + L") ";
				inputText += L"\n FireReleased(" + std::to_wstring(playerAction.NormalizedInputValue) + L") ";
				flagFromPressToRelasedClick = true;
				break;

			case PLAYER_ACTION_TYPES::INPUT_JUMP_PRESSED:
				inputText += L"\n JumpPressed(" + std::to_wstring(playerAction.NormalizedInputValue) + L") ";
				break;
			case PLAYER_ACTION_TYPES::INPUT_JUMP_DOWN:
				inputText += L"\n JumpDown(" + std::to_wstring(playerAction.NormalizedInputValue) + L") ";
				break;
			case PLAYER_ACTION_TYPES::INPUT_JUMP_RELEASED:
				inputText += L"\n JumpReleased(" + std::to_wstring(playerAction.NormalizedInputValue) + L") ";
				break;

			case PLAYER_ACTION_TYPES::INPUT_MOVE:
				inputText += L"\n MoveX:(" + std::to_wstring(playerAction.PointerThrowX) + L") ";
				inputText += L"\n MoveY:(" + std::to_wstring(playerAction.Y) + L") ";
				screenManager->game->player->move(playerAction.X, playerAction.Y);
				break;
			case PLAYER_ACTION_TYPES::INPUT_AIM:
				inputText += L"\n AimX(" + std::to_wstring(playerAction.PointerThrowX) + L") ";
				inputText += L"\n AimY(" + std::to_wstring(playerAction.Y) + L") ";
				break;
			case PLAYER_ACTION_TYPES::INPUT_BRAKE:
				inputText += L"\n Brake(" + std::to_wstring(playerAction.NormalizedInputValue) + L") ";
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

	D3D11_TEXTURE2D_DESC pDesc;
	Microsoft::WRL::ComPtr<ID3D11Resource> res;
	//1.) -----------------
	//m_texture->GetResource(&res);
	//((ID3D11Texture2D*)res.Get())->GetDesc(&pDesc); // Usually dangerous!

	//2.) -----------------
	m_texture->GetResource(res.GetAddressOf());
	m_texture2->GetResource(res.GetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> text2D;
	res.As(&text2D);
	text2D->GetDesc(&pDesc);

	auto height = pDesc.Height; //texture height
	auto width = pDesc.Width; //texture width

	auto windowSize = m_deviceResources->GetOutputSize(); // physical screen resolution
	auto logicalSize = m_deviceResources->GetLogicalSize(); //DPI dependent resolution

	if ((centerPosition.x != logicalSize.Width / 2) || (centerPosition.y != logicalSize.Height / 2))
	{
		screenManager->resize((logicalSize.Width/2) / centerPosition.x , (logicalSize.Height/2) / centerPosition.y);
		centerPosition.y = logicalSize.Height/2;
		centerPosition.x = logicalSize.Width/2;
	}

	// Draw sprites
	m_sprites->Begin();

	//background->Draw(m_sprites.get());
	//clouds->Draw(m_sprites.get());

	//Drawing walls

	/*for (auto wall : wallsVector)
	{
		wall.Draw(m_sprites.get());
	}*/

	//screen->Draw(m_sprites.get());
	//buttons[0]->Draw(m_sprites.get());
	screenManager->Draw(m_sprites.get());

	//wall->Draw(m_sprites.get());
	//wall2->Draw(m_sprites.get());
	//player->Draw(m_sprites.get());
	//m_font->DrawString(m_sprites.get(), startButtonString.c_str(), XMFLOAT2(300, 200), Colors::Black);
	//clouds2->Draw(m_sprites.get());

	//m_font->DrawString(m_sprites.get(), collisionString.c_str(), XMFLOAT2(100, 10), Colors::Yellow);
	
	m_sprites->End();
}

//this is the LOAD function
void DirectXTK3DSceneRenderer::CreateDeviceDependentResources()
{
	// Create DirectXTK objects
	auto device = m_deviceResources->GetD3DDevice();

	auto context = m_deviceResources->GetD3DDeviceContext();

	auto windowSize = m_deviceResources->GetOutputSize(); // physical screen resolution
	auto logicalSize = m_deviceResources->GetLogicalSize(); //DPI dependent resolution

	m_sprites.reset(new SpriteBatch(context));
	spriteBatchT1.reset(new SpriteBatch(context));
	spriteBatchT2.reset(new SpriteBatch(context));

	m_font.reset(new SpriteFont(device, L"assets\\italic.spritefont"));

	//player.reset(new Player(m_texture.Get()));

	/*DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"assets\\background.dds", nullptr, backgroundTexture.ReleaseAndGetAddressOf())
		);
	background.reset(new ScrollingBackground);
	background->Load(backgroundTexture.Get());*/


	/*DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"assets\\clouds.dds", nullptr, cloudsTexture.ReleaseAndGetAddressOf())
		);
	clouds.reset(new ScrollingBackground);
	clouds->Load(cloudsTexture.Get());

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"assets\\clouds2.dds", nullptr, cloudsTexture2.ReleaseAndGetAddressOf())
		);
	clouds2.reset(new ScrollingBackground);
	clouds2->Load(cloudsTexture2.Get());*/

	/*DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"assets\\enemyanimated.dds", nullptr, enemyTexture.ReleaseAndGetAddressOf())
		);*/
	//TODO: Instatiate enemies here
	/*Enemy enemyTemp(enemyTexture.Get());
	enemiesVector.push_back(enemyTemp);

	*/
	
	//Adding walls to vector
	//wallsVector.push_back(Wall(logicalSize, XMFLOAT2(300, 0), pipeTexture.Get()));
	//wallsVector.emplace_back(Wall(logicalSize, XMFLOAT2(logicalSize.Width, 0), pipeTexture.Get()));
	screenManager = std::unique_ptr<ScreenManager>(new ScreenManager());

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"assets\\button.dds", nullptr, m_texture.ReleaseAndGetAddressOf())
		);

	centerPosition.x = logicalSize.Width / 2.0;
	centerPosition.y = logicalSize.Height / 2.0;
	float oneUnitHeight = logicalSize.Height / 7.0;
	Screen* screen = new Screen(m_texture.Get(), new SpriteFont(device, L"assets\\italic.spritefont"));
	screen->setName(L"Main");
	screen->addElement(L"Start", L"StartMain", XMFLOAT2(centerPosition.x, centerPosition.y- oneUnitHeight));
	screen->addElement(L"Options", L"OptionsMain", XMFLOAT2(centerPosition.x, centerPosition.y));
	screen->addElement(L"Exit", L"ExitMain", XMFLOAT2(centerPosition.x, centerPosition.y + oneUnitHeight));
	screenManager->addScreen(screen);

	Screen* screen1 = new Screen(m_texture.Get(), new SpriteFont(device, L"assets\\italic.spritefont"));
	screen1->setName(L"Options");
	screen1->addElement(L"Music", L"MusicOptions", XMFLOAT2(centerPosition.x, centerPosition.y - oneUnitHeight));
	screen1->addElement(L"Author", L"AuthorOptions", XMFLOAT2(centerPosition.x, centerPosition.y));
	screen1->addElement(L"Back", L"BackOptions", XMFLOAT2(centerPosition.x, centerPosition.y + oneUnitHeight));
	screenManager->addScreen(screen1);

	Screen* screen2 = new Screen(m_texture.Get(), new SpriteFont(device, L"assets\\italic.spritefont"));
	screen2->setName(L"Level");
	screen2->addElement(L"Offline", L"OfflineLevel", XMFLOAT2(centerPosition.x, centerPosition.y - oneUnitHeight));
	screen2->addElement(L"Online", L"OnlineLevel", XMFLOAT2(centerPosition.x, centerPosition.y));
	screen2->addElement(L"Back", L"BackLevel", XMFLOAT2(centerPosition.x, centerPosition.y + oneUnitHeight));
	screenManager->addScreen(screen2);

	Screen* screen4 = new Screen(m_texture.Get(), new SpriteFont(device, L"assets\\italic.spritefont"));
	screen4->setName(L"Pause");
	screen4->addElement(L"Return", L"ReturnPause", XMFLOAT2(centerPosition.x, centerPosition.y));
	screen4->addElement(L"Exit", L"ExitPause", XMFLOAT2(centerPosition.x, centerPosition.y + oneUnitHeight));
	screenManager->addScreen(screen4);

	Screen* screen5= new Screen(m_texture.Get(), new SpriteFont(device, L"assets\\italic.spritefont"));
	screen5->setName(L"Author");
	screen5->addElement(L"It's me ;)", L"DescriptionAuthor", XMFLOAT2(centerPosition.x, centerPosition.y));
	screen5->addElement(L"Back", L"BackAuthor", XMFLOAT2(centerPosition.x, centerPosition.y + oneUnitHeight));
	screenManager->addScreen(screen5);

	Screen* screen3 = new Screen(m_texture.Get(), new SpriteFont(device, L"assets\\italic.spritefont"));
	screen3->setName(L"Play");	
	screen3->addElement(L"Pause", L"PausePlay", XMFLOAT2(centerPosition.x, oneUnitHeight));
	screenManager->addScreen(screen3);

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"assets\\brick2.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
		);

	screenManager->addBrickTexture(m_texture2.Get());
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"assets\\brick.dds", nullptr, m_texture.ReleaseAndGetAddressOf())
		);
	

	std::vector<int> numberTestureVector = std::vector<int>();
	std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVector = std::shared_ptr<std::vector<BRICK_BEHAVIOR>>(new std::vector<BRICK_BEHAVIOR>());
	
	int x1 = 16;
	int y1 = 9;
	int tab1[] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};

	for (int y = 0; y < y1; y++)
	{
		for (int x = 0; x < x1; x++)
		
		{
			if (tab1[(x*x1)+y] == 1)
				baehaviorTestureVector->push_back(BRICK_BEHAVIOR_NONE);
			else 
				baehaviorTestureVector->push_back(BRICK_BEHAVIOR_BLOCK);
		}
	}
		
	screenManager->addBrickTexture2(m_texture.Get());
	screenManager->setMapLevel(x1,y1, tab1, baehaviorTestureVector, logicalSize.Width, logicalSize.Height);

	screenManager->setName(L"Main");

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"assets\\shipanimated.dds", nullptr, m_texture.ReleaseAndGetAddressOf())
		);
	screenManager->game->addPlayer(m_texture.Get(), XMFLOAT2(centerPosition.x, centerPosition.y));

	//set windows size for drawing the background
	//background->SetWindow(logicalSize.Width, logicalSize.Height);
	//clouds->SetWindow(logicalSize.Width, logicalSize.Height);
	//clouds2->SetWindow(logicalSize.Width, logicalSize.Height);

	//Enginepad
	//EnginePad.reset(new EnginePad);
}

void DirectXTK3DSceneRenderer::ReleaseDeviceDependentResources()
{
    m_sprites.reset();
    m_font.reset();
    m_texture.Reset();
	//backgroundTexture.Reset();
	//cloudsTexture.Reset();
	//cloudsTexture2.Reset();
	//pipeTexture.Reset();
	//enemyTexture.Reset();
}