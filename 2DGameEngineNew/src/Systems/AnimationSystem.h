#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include <SDL.h>

class AnimationSystem : public System {
	public:
		AnimationSystem() {
			RequireComponent<SpriteComponent>();
			RequireComponent<AnimationComponent>();
		}


		void Update(){
			for (auto entity : GetSystemEntities()) {
				auto& animation = entity.GetComponent<AnimationComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();

				//changing the frame so that the image changes
				animation.currentFrame = static_cast<int>(((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000.0)) % animation.numFrames;
				sprite.srcRect.x = animation.currentFrame * sprite.width;


			}
		}


};



#endif