//--------------------------------------------------------------------------------------
// File: DirectXTK3DSceneRenderer.h
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

#pragma once
#include <future>
#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"
#include "Audio.h"
//#include "CommonStates.h"
//#include "Effects.h"
//#include "GeometricPrimitive.h"
//#include "Model.h"
//#include "PrimitiveBatch.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
//#include "VertexTypes.h"
#include "AnimatedTexture.h"
#include "ScrollingBackground.h"
#include "Wall.h"
#include "GamePad.h"
#include "TextButton.h"
#include "Screen.h"
#include "Common/DeviceResources.h"
#include "Common/StepTimer.h"
#include "Common/InputManager.h"
#include "Common/OverlayManager.h"
#include "DirectXTK\Inc\SimpleMath.h"
#include "ScreenManager.h"
#include "Game.h"
#include "Level.h"


namespace SimpleSample
{
	class DirectXTK3DSceneRenderer
	{
	public:
		DirectXTK3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
        void CreateAudioResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer, std::vector<PlayerInputData>* playerInput, unsigned int playersAttached);
		void Render();
		// Signals a new audio device is available
		void NewAudioDevice();

	private:
		// Cached player metadata.
		unsigned int m_playersAttached;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

        // Not usefull right now
		//void XM_CALLCONV DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);
		
		//Sound
		bool																	m_playMusic;
        float                                                                   m_audioTimerAcc;
        uint32_t                                                                m_audioEvent;
		std::unique_ptr<DirectX::AudioEngine>                                   m_audEngine;
        std::unique_ptr<DirectX::WaveBank>                                      m_waveBank;
        std::unique_ptr<DirectX::SoundEffect>                                   m_soundEffect;
        std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect1;
        std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect2;
		// Variables used with the rendering loop.
        bool                                                                    m_retryDefault;
		bool																	flagFromPressToRelasedClick;
		float																	scaleX;
		float																	scaleY;
		XMFLOAT2																centerPosition;
		Windows::Foundation::Size												logicalSize;
		std::shared_ptr<DirectX::SpriteFont>                                    m_font;
		std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
		std::unique_ptr<ScreenManager>											screenManager;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture2;
		std::shared_ptr<Level>													level;
		//std::unique_ptr<GamePad>												GamePad;
	};
}

