#include <Particle.h>

Particle::Particle ()
: Particle (nullptr) {}

Particle::Particle (Collider* collider)
: velocity ()
, neighbours () {
    this->collider = collider;
}