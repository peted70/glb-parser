#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	_sceneNode = make_shared<RootNode>(-1);
}

void SceneManager::Notify(const Observable & data)
{
	NotificationManager::Instance().Notify(data);
}

shared_ptr<RootNode> SceneManager::Current()
{
	return _sceneNode;
}

void SceneManager::SetDevResources(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	_deviceResources = deviceResources;
}
