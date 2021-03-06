#pragma once

#include "cmp_sprite.h"
#include "ecm.h"
#include <Box2D/Dynamics/b2Body.h>

// Physics Component
class PhysicsComponent : public Component {
protected:
  b2Body* _body;
  const bool _dynamic;
  b2Fixture* _fixture;
  float _maxSpeed;

public:
  PhysicsComponent(Entity* p, bool dyn, const sf::Vector2f& size);


  void setFixtureDef(b2FixtureDef fix);
  b2Fixture* const getFixture() const;
  bool isTouching(const PhysicsComponent& pc) const;
  bool isTouching(const PhysicsComponent& pc, b2Contact const* bc) const;
  std::vector<const b2Contact*> getTouching() const;
  const sf::Vector2f getVelocity() const;
  void setRestitution(float r);
  void setFriction(float r);
  void setMass(float m);
  void setMaxSpeed(float maxSpeed) { _maxSpeed = maxSpeed; }
  void update(double dt) override;
  void render() override;
  void impulse(const sf::Vector2f& i);
  void impulseRelative(sf::Vector2f& i);
  void dampen(const sf::Vector2f& s);
  void setVelocity(const sf::Vector2f& v);
  void setAngularVelocity(const float a);
  void teleport(const sf::Vector2f& v);
  void setAngle(const float angle);
  void dump();
  void setLinearDampening(float f);
  void setAngularDampening(float f);
  b2Body* getBody();
  ~PhysicsComponent() override;
};
