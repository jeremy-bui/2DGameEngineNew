#pragma once
#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "spdlog/spdlog.h"
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem : public System
{
	public:
		CollisionSystem()
		{
			RequireComponent<TransformComponent>();
			RequireComponent<BoxColliderComponent>();
		}

		void Update()
		{
			//TODO:
			//Check all entities that have a boxcollider
			//to see if they are colliding with each other
			//AABB - Axis-Aligned Bounding Boxes

			auto entities = GetSystemEntities();
			
			//Loop all entities that the system is interested in
			for (auto i = entities.begin(); i != entities.end(); i++)
			{
				Entity a = *i;
				auto aTransform = a.GetComponent<TransformComponent>();
				auto aCollider = a.GetComponent<BoxColliderComponent>();

				//loop all the entities that still need to be checked (to the right of i or left to be checked)
				for (auto j = i; j != entities.end(); j++)
				{
					Entity b = *j;

					//bypass if we are trying to test them same entity
					if (a == b)
					{
						continue;
					}

					auto bTransform = b.GetComponent<TransformComponent>();
					auto bCollider = b.GetComponent<BoxColliderComponent>();

					//perform the AABB collision check between a and b
					bool collisionHappened = CheckAABBCollision(
						aTransform.position.x + aCollider.offset.x,
						aTransform.position.y + aCollider.offset.y,
						aCollider.width,
						aCollider.height,
						bTransform.position.x + bCollider.offset.x,
						bTransform.position.y + bCollider.offset.y,
						bCollider.width,
						bCollider.height
					);

					if (collisionHappened)
					{
						spdlog::info("Entity " + std::to_string(a.GetId()) + " is colliding with entity " + std::to_string(b.GetId()));
					}


				}
			}
		}

		bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
		{
			return (
				aX < bX + bW &&
				aX + aW > bX &&
				aY < bY + bH &&
				aY + aH > bY
				);
		}
};



#endif