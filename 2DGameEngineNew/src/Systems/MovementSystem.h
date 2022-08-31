#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "spdlog/spdlog.h"

class MovementSystem : public System {
	public:
		MovementSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<RigidBodyComponent>();
		}
		
		void Update(double deltaTime) {
			//loop through all entities that the system is interested in
			for (auto entity : GetSystemEntities()) {
				auto& transform = entity.GetComponent<TransformComponent>();
				const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

				//casting deltaTime to a float since compiler no likey when it isn't
				transform.position.x += rigidbody.velocity.x * (float)deltaTime;
				transform.position.y += rigidbody.velocity.y * (float)deltaTime;

				/*::info("Entity id = " +
					std::to_string(entity.GetId()) + 
					" position is now (" + 
					std::to_string(transform.position.x) +
					", " + 
					std::to_string(transform.position.y) + ")"
				);*/
			}
		}
};







#endif