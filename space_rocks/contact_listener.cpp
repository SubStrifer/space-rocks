#include "contact_listener.h"

std::default_random_engine rndG((int)time(NULL));
std::uniform_real_distribution<float> zeroOne(0.0f, 1.0f);

//Contact Listener
void MyContactListener::BeginContact(b2Contact* contact)
{
	b2Filter filterA = contact->GetFixtureA()->GetFilterData();
	b2Filter filterB = contact->GetFixtureB()->GetFilterData();

	b2Vec2 posA = contact->GetFixtureA()->GetBody()->GetPosition();
	b2Vec2 posB = contact->GetFixtureB()->GetBody()->GetPosition();
	auto entityA = static_cast<Entity*> (contact->GetFixtureA()->GetBody()->GetUserData());
	auto entityB = static_cast<Entity*> (contact->GetFixtureB()->GetBody()->GetUserData());

	//Bullet x Object Collisions
	if (filterA.groupIndex == 1 ^ filterB.groupIndex == 1)
	{
		//Make bullet always A
		if (filterB.groupIndex == 1)
		{
			std::swap(filterA, filterB);
			std::swap(posA, posB);
			std::swap(entityA, entityB);
		}

		//Damage object
		entityB->getComponents<DestructibleComponent>()[0]->damage(
			entityA->getComponents<BulletComponent>()[0]->getDamage()
		);

		//If player, play sound
		if(filterB.categoryBits == PLAYER_SHIP)
			audioManager.playSound("player_hurt");

		//Else if Player Bullet x Asteroid/Ship 
		else if (filterA.categoryBits == PLAYER_BULLET && (filterB.categoryBits == ASTEROIDS || filterB.categoryBits == ENEMY_SHIP))
		{
			//If Object was destroyed
			if (entityB->getComponents<DestructibleComponent>()[0]->getHp() <= 0.0f)
			{
				coinDrop(entityB);
				scoreAdd(entityB, true);
			}
			else
			{
				scoreAdd(entityB, false);
			}
		}


		//Destroy bullet
		entityA->getComponents<DestructibleComponent>()[0]->damage(10.0f);
	}

	//Bullet x Bullet Collisions
	else if (filterA.groupIndex == 1 && filterB.groupIndex == 1)
	{
		//Destroy bullets
		audioManager.playSound("bullet_x_bullet");
		entityA->getComponents<DestructibleComponent>()[0]->damage(10.0f);
		entityB->getComponents<DestructibleComponent>()[0]->damage(10.0f);
	}

	//Player x Object Collisions
	else if (filterA.categoryBits == PLAYER_SHIP ^ filterB.categoryBits == PLAYER_SHIP)
	{
		//Make player ship always A
		if (filterB.categoryBits == PLAYER_SHIP)
		{
			std::swap(filterA, filterB);
			std::swap(posA, posB);
			std::swap(entityA, entityB);
		}

		//If Player x Bullet, do nothing (as bullet impact already creates particles)
		if (filterB.groupIndex == 1)
			return;

		//Damage player
		entityA->getComponents<DestructibleComponent>()[0]->damage(1.0f);

		//Damage enemy ship
		if (filterB.categoryBits == ENEMY_SHIP)
		{
			entityB->getComponents<DestructibleComponent>()[0]->damage(1.0f);
			//If ENEMY SHIP KILLED BY MELEE drop coins
			if (entityB->getComponents<DestructibleComponent>()[0]->getHp() <= 0.0f)
			{
				coinDrop(entityB);
				scoreAdd(entityB, true);
			}
			//else hit but not killed by melee
			else
				scoreAdd(entityB, false);
		}

		//Damage asteroid
		if (filterB.categoryBits == ASTEROIDS)
		{
			entityB->getComponents<DestructibleComponent>()[0]->damage(1.0f);

			//If ASTEROID KILLED BY MEELE drop coins
			if (entityB->getComponents<DestructibleComponent>()[0]->getHp() <= 0.0f)
			{
				scoreAdd(entityB, true);
				coinDrop(entityB);
			}
			else
				scoreAdd(entityB, false);
		}

		//Play audio
		audioManager.playSound("player_hurt");
	}
}

void MyContactListener::scoreAdd(Entity * entityB, bool killed)
{
	if (killed)
	{
		player1->getComponents<PlayerComponent>()[0]->addScore(entityB->getComponents<DestructibleComponent>()[0]->getScoreValue());
	}
	else
		player1->getComponents<PlayerComponent>()[0]->addScore(entityB->getComponents<DestructibleComponent>()[0]->getScoreValue() * 0.05f);
}

void MyContactListener::coinDrop(Entity * entityB)
{
	//Coin drop
	if (zeroOne(rndG) < entityB->getComponents<DestructibleComponent>()[0]->getCoinChance())
	{
		player1->getComponents<PlayerComponent>()[0]->addCoins(entityB->getComponents<DestructibleComponent>()[0]->getCoinValue());
		audioManager.playSound("pickup_coin");
	}
}
