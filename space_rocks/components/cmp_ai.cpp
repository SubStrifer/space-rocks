#include "cmp_ai.h"
#include "maths.h"

// Controls enemy behaviour
AiComponent::AiComponent(Entity* p) : Component(p) {
	
	_shipComponent = _parent->getComponents<ShipComponent>()[0];
	_players = gameScene.ents.find("Player");
}

AiComponent::~AiComponent() {
	_shipComponent.reset();
	_target.reset();

	for (auto p : _players)
		p.reset();
}

void AiComponent::update(double dt) {

	// Return if there's no players alive
	if (_players.size() == 0)
		return;

	sf::Vector2f position = _parent->getPosition();

	// Assign the closest player
	if (_target == NULL)
	{
		_target = _players.front();

		float shortestDistance = length(position - _target->getPosition());
		for (std::shared_ptr<Entity> player : _players)
		{
			float newDistance = length(position - player->getPosition());
			if (newDistance < shortestDistance)
			{
				shortestDistance = newDistance;
				_target = player;
			}
		}
	
	}
	
	// Calculating angle to player
	float distance = length(position - _target->getPosition());
	Vector2f direction = _target->getPosition() - position;
	float targetAngle = rad2deg(std::atan2f(direction.y, direction.x)) + 90.0f;

	_shipComponent->rotate(_parent->getRotation() < targetAngle ? true : false);
	
	// Too far away
	if (distance > 512)
	{
		_shipComponent->thrust(dt);
	}
	else
	{
		_shipComponent->shoot();
	}
	
}
