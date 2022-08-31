#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include "spdlog/spdlog.h"
#include <SDL.h>

class RenderSystem : public System {
public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
		
		//struct that contains the components of the entity
		struct RenderableEntity {
			TransformComponent transformComponent;
			SpriteComponent spriteComponent;
		};

		//sort entities of the system by z-index
		std::vector<RenderableEntity> renderableEntities;
		for (auto entity : GetSystemEntities()) {
			RenderableEntity renderableEntity;
			renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
			renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
			//push into the vector
			renderableEntities.emplace_back(renderableEntity);
		}
		//sort the vector by the z index. This will tell the computer what order the images will be in
		std::sort(renderableEntities.begin(), renderableEntities.end(), 
			[](const RenderableEntity& a, const RenderableEntity& b) {
				//this part compares them
				return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
			});

		//loop through all entities that the system is interested in now that it is sorted
		for (auto entity : renderableEntities) {
			const auto transform = entity.transformComponent;
			const auto sprite = entity.spriteComponent;

			//set the source rectangle of our original sprite texture
			SDL_Rect srcRect = sprite.srcRect;

			//set the destination rectangle with the x,y position to be rendered
			SDL_Rect dstRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			
			SDL_RenderCopyEx(
				renderer,
				assetStore->GetTexture(sprite.assetId),
				&srcRect,
				&dstRect,
				transform.rotation,
				NULL,
				SDL_FLIP_NONE
			);

		}
	}
};

#endif