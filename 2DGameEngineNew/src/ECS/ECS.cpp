#include "ECS.h"
#include "spdlog/spdlog.h"
#include <algorithm>



int IComponent::nextId = 0;


int Entity::GetId() const {
	return id;
}


void System::AddEntityToSystem(Entity entity) {
	entities.push_back(entity);
}


void System::RemoveEntityFromSystem(Entity entity) {

	//modern way of interating values to then remove the entity
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
		return entity == other;
	}), entities.end());

}


std::vector<Entity> System::GetSystemEntities() const {
	return entities;
}


const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity Registry::CreateEntity() {
	int entityId;

	entityId = numEntities++;

	Entity entity(entityId);
	entity.registry = this;
	//inserting the entity into the set that will recognize what needs to be added
	entitiesToBeAdded.insert(entity);

	if (entityId >= static_cast<int>(entityComponentSignatures.size())) {
		entityComponentSignatures.resize(entityId + 1);
	}
	

	spdlog::info("Entity created with id = " + std::to_string(entityId));

	return entity;
}

void Registry::AddEntityToSystems(Entity entity) {
	const auto entityId = entity.GetId();

	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	//loop all the systems
	for (auto& system : systems){
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		//if both the signature and componenet signature are both 1s
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested) {
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::Update() {
	//add the entities that are waiting to be created to the active Systems
	
	for (auto entity : entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();



	//remove the entities that are waiting to be killed from the active Systems
}