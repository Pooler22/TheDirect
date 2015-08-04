//--------------------------------------------------------------------------------------
// File: SimpleSampleMain.h
//
// This is a simple Windows Store app for Windows 8.1 Preview showing use of DirectXTK
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

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Common\InputManager.h"
#include "Common\OverlayManager.h"


#include "Content\DirectXTK3DSceneRenderer.h"
#include "Content\SampleDebugTextRenderer.h"
#include "Content\SampleVirtualControllerRenderer.h"



// Renders Direct2D and 3D content on the screen.
namespace SimpleSample
{
	class SimpleSampleMain : public DX::IDeviceNotify
	{
	public:
		SimpleSampleMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~SimpleSampleMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

        // Signals a new audio device is available
        void NewAudioDevice();

		// IDeviceNotify
		virtual void OnDeviceLost();
        virtual void OnDeviceRestored();

	private:
		void InitializeTouchRegions();
		void ProcessInput(std::vector<PlayerInputData>* playerActions);
		
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		std::unique_ptr<DirectXTK3DSceneRenderer> m_sceneRenderer;

		std::shared_ptr<SampleDebugTextRenderer>         m_debugTextRenderer;
		std::shared_ptr<SampleVirtualControllerRenderer> m_virtualControllerRenderer;

		// Input, overlay managers
		std::unique_ptr<InputManager>      m_inputManager;
		std::shared_ptr<OverlayManager>    m_overlayManager;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Tracks which players are connected (0...3).
		unsigned int m_playersConnected;

		// Tracks the touch region ID, allowing you to enable/disable touch regions.
		// Note to developer: Expand this array if you add more touch regions, e.g. for a menu.
		unsigned int m_touchRegionIDs[3];



	};
}