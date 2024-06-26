#pragma once

#include <cassert>

#include "../entity/Entity.h"
#include "../component/Component.h"
#include "../system/System.h"

class World;

class SystemManager
{
public:
	SystemManager(World* aWorld)
	{
		mySystemSignatures = {};
		mySystems = {};
		myWorld = aWorld;
	}

	~SystemManager()
	{
		for (size_t i = 0; i < mySystems.size(); i++)
		{
			mySystems[i]->Reset();
			delete mySystems[i];
		}
		mySystems.clear();
		mySystemSignatures.clear();
		myWorld = nullptr;
	}

	void Reset()
	{
		for (size_t i = 0; i < mySystems.size(); i++)
		{
			mySystems[i]->Reset();
		}
	}

	void Update(SceneUpdateContext& aContext)
	{
		for (size_t i = 0; i < mySystems.size(); i++)
		{
			mySystems[i]->Update(aContext);
		}
	}

	void Render()
	{
		for (size_t i = 0; i < mySystems.size(); i++)
		{
			mySystems[i]->Render();
		}
	}

	void Init()
	{
		for (size_t i = 0; i < mySystems.size(); i++)
		{
			mySystems[i]->Init();
		}
	}

	template<typename T, typename... Param>
	T* RegisterSystem(Param... args)
	{
		assert(mySystems.size() > 0 && T::systemId == 0 || mySystems.size() == 0 && "Registering system more than once.");

		T::systemId = static_cast<sid_t>(mySystems.size());
		mySystemSignatures.push_back({});
		T* system = new T(myWorld, args...);
		mySystems.push_back(system);

		return system;
	}

	// This function should probably not be used in the game code, it is only used for testing purposes
	template<typename T>
	T* GetSystem()
	{
		assert(T::systemId < mySystems.size() && "System not found.");
		return &mySystems[T::systemId];
	}

	template<typename T>
	void SetSignature(const ComponentSignature& aSignature)
	{
		assert(T::systemId < mySystemSignatures.size() && "System not found.");
		mySystemSignatures[T::systemId] = aSignature;
	}

	void OnEntityDestroyed(const Entity& aEntity)
	{
		for (size_t i = 0; i < mySystems.size(); i++)
		{
			mySystems[i]->EraseEntity(aEntity);
		}
	}

	void OnEntitySignatureChanged(const Entity& aEntity, const ComponentSignature& aSignature)
	{
		for (size_t i = 0; i < mySystems.size(); i++)
		{
			ComponentSignature& systemSignature = mySystemSignatures.at(mySystems[i]->GetSystemId());

			if ((aSignature & systemSignature) == systemSignature)
			{
				mySystems[i]->InsertEntity(aEntity);
				continue;
			}

			mySystems[i]->EraseEntity(aEntity);
		}
	}
private:
	World* myWorld;

	std::vector<ComponentSignature> mySystemSignatures; // used for filtering entities
	std::vector<ISystem*> mySystems;
};
