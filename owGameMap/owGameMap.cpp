#include "stdafx.h"

// General
#include "owGameMap.h"

// Additional
#include "WMO/WMOsManager.h"
#include "Map.h"
#include "GameState/GameState_Map.h"

extern CLog* gLogInstance;

class COpenWoWMapAndWMOPlguin
	: public IznPlugin
	, public ISceneNodeCreator
	, public IGameStateCreator
{
public:
	COpenWoWMapAndWMOPlguin(IBaseManager* BaseManager)
		: m_BaseManager(BaseManager)
	{}
	virtual ~COpenWoWMapAndWMOPlguin()
	{}



	//
	// IznPlugin
	//
	bool Initialize() override
	{
		gLogInstance = std::dynamic_pointer_cast<CLog>(GetManager<ILog>(m_BaseManager)).get();

		GetManager<IFilesManager>(m_BaseManager)->RegisterFilesStorage(std::make_shared<CMPQFilesStorage>("D:\\_games\\World of Warcraft 1.12.1\\Data\\", IFilesStorageEx::PRIOR_HIGH));

		OpenDBs(m_BaseManager);

		std::shared_ptr<IWMOManager> wmoManager = std::make_shared<WMOsManager>(m_BaseManager);
		AddManager<IWMOManager>(m_BaseManager, wmoManager);

		return true;
	}
	void Finalize() override
	{
	
	}
	std::string GetName() const override
	{
		return "Open WoW Map & WMO object plugin.";
	}
	std::string GetDescription() const override
	{
		return "";
	}



	//
	// ISceneNodeCreator
	//
	size_t GetSceneNodesCount() const override
	{
		return 1;
	}
	std::string GetSceneNodeTypeName(size_t Index) const override
	{
		if (Index == 0)
		{
			return "WoWMap";
		}

		return nullptr;
	}
	std::shared_ptr<ISceneNode> CreateSceneNode(std::weak_ptr<ISceneNode> Parent, size_t Index) const override
	{
		if (Index == 0)
		{
			return Parent.lock()->CreateSceneNode<CMap>(m_BaseManager);
		}

		return nullptr;
	}



	//
	// IGameStateCreator
	//
	size_t GetGameStatesCount() const override
	{
		return 1;
	}
	std::string GetGameStateName(size_t Index) const override
	{
		return "GameState_Map";
	}
	GameStatePriority GetGameStatePriority(size_t Index) const override
	{
		return 5;
	}
	std::shared_ptr<IGameState> CreateGameState(size_t Index, std::shared_ptr<IRenderWindow> RenderWindow) const override
	{
		return std::make_shared<CGameState_Map>(m_BaseManager, RenderWindow);
	}

private:
	IBaseManager* m_BaseManager;
	std::shared_ptr<IRenderDevice> m_CachedRenderDevice;
};



IznPlugin* plugin = nullptr;
extern "C" __declspec(dllexport) IznPlugin* WINAPI GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new COpenWoWMapAndWMOPlguin(BaseManager);
	}

	return plugin;
}