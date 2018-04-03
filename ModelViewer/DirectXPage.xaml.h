﻿//
// DirectXPage.xaml.h
// Declaration of the DirectXPage class.
//

#pragma once

#include "DirectXPage.g.h"

#include "Common\DeviceResources.h"
#include "ModelViewerMain.h"
#include "SceneManager.h"

namespace ModelViewer
{
	using namespace Windows::UI::Xaml::Input;
	using namespace Windows::UI::Core;
	using namespace Platform;
	using namespace ViewModels;
	using namespace TreeViewControl;

	/// <summary>
	/// A page that hosts a DirectX SwapChainPanel.
	/// </summary>
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		virtual ~DirectXPage();

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

		TreeNode^ CreateMeshNode(String^ name);
		TreeNode^ CreateContainerNode(String^ name);

		property DirectXPageViewModel ^ ViewModel;

	private:
		class SceneUpdateProxy
		{
		public:
			SceneUpdateProxy(DirectXPage ^page) :
				owner(page)
			{
				SceneManager::Instance().RegisterForUpdates(boost::bind(&SceneUpdateProxy::NotifySceneChanges, this, _1));
			}

			void NotifySceneChanges(Observable const& scene)
			{
				owner->NotifySceneChanges(scene);
			}

		private:
			DirectXPage ^ owner;
		};

		SceneUpdateProxy updates;

		void NotifySceneChanges(Observable const& scene);
		TreeNode^ AddTreeItemsRecursive(GraphNode& node, TreeNode^ parent);

		// XAML low-level rendering event handler.
		void OnRendering(Object^ sender, Object^ args);

		// Window event handlers.
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Other event handlers.
		void AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		// Independent input handling functions.
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

		// Resources used to render the DirectX content in the XAML page background.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<ModelViewerMain> m_main; 
		bool m_windowVisible;
		bool m_controlPressed;
		void confirmColor_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void cancelColor_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}

