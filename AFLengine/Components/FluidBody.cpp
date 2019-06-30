#include <FluidBody.h>
#include <Time_.h>
#include <Physics.h>

#include <Debug.h>
#include <SceneObject.h>
#include <Transform.h>
#include <Collider.h>

#include <glutAdapter.h>

BOOST_CLASS_EXPORT_IMPLEMENT (FluidBody)


FluidBody::FluidBody ()
: Body (UpdatePass::FrameStart_Early)
, neighbourRadius (1.2)
, restDensity (100.0)
, viscosityLinearDependence (2.0)
, viscosityQuadraticDependence (0.01)
, stiffness (0.3)
, nearStiffness (0.6)
, particles () {}

void FluidBody::update (UpdatePass::Pass pass) {

    applyExternalForces ();
    applyViscosity ();
    advanceParticles ();
    updateNeighbors ();
    doubleDensityRelaxation ();

}

void FluidBody::drawGizmos () {

    glLineWidth (4);

    glColor3 (Color::RED);

    glBegin (GL_LINES);

    for (auto p : particles) {

        ParticleRef particle = p.second;

        Vector2 center = particle->collider->getGlobalCenterPosition();
        Vector2 velocityEndPoint = center + particle->velocity;

        glVertex2d (center);
        glVertex2d (velocityEndPoint);

    }

    glEnd ();

}

Vector2 FluidBody::getGlobalVelocity (Collider* collider) {
    return particles[collider]->velocity;
}

double FluidBody::getGlobalAngularVelocity (Collider* collider) {
    return 0.0;
}

void FluidBody::advance (Collider* collider, double timeOffset) {

    collider->positionOffset += timeOffset * particles[collider]->velocity;

}

void FluidBody::translateCollider (Collider* collider, Vector2 offset) {

    collider->positionOffset += getObject ()->transform->getTransformationMatrix ().delocalizeVector (offset);

}

void FluidBody::applyImpulse (Collider* collider, Vector2 impulse, Vector2 point) {

    // update velocity
    Vector2 deltaVelocity = impulse / mass;
    particles[collider]->velocity += deltaVelocity;

}

double FluidBody::getMomentOfInertia (Collider* collider) {

    return collider->momentOfInertiaCoefficient () * mass;

}

void FluidBody::applyExternalForces () {

    double deltaTime = Time::getInstance ().getDeltaTime ();
    Vector2 gravitationalAcceleration = Physics::getInstance ().gravitationalAcceleration * gravitationalCoefficient;

    std::unordered_map<Collider*, ParticleRef> lastFrameParticles(particles);
    particles.clear ();
    particleGrid.clear ();

    SceneObjectRef object = getObject ();
    int colliderCount = object->getComponentCount (Component::Type_Collider);
    for (int i = 0; i < colliderCount; ++i) {

        ComponentRef component = object->getComponent (Component::Type_Collider, i);
        Collider* collider = std::static_pointer_cast<Collider> (component).get();

        ParticleRef particle;
        if (lastFrameParticles.find(collider) != lastFrameParticles.end()) {
            particle = lastFrameParticles[collider];
        }
        else {
            particle = ParticleRef (new Particle (collider));
        }

        particle->velocity += deltaTime * gravitationalAcceleration * gravitationalCoefficient;

        particles[collider] = particle;

    }
}

void FluidBody::applyViscosity () {

    double deltaTime = Time::getInstance ().getDeltaTime ();

    for (auto p : particles) {

        ParticleRef particle = p.second;

        for (auto n : particle->neighbours) {

            ParticleRef neighbour = n.lock ();
            if (!neighbour) {
                Debug_Log (Debug::Level_Warning, "[Obj %d] Missing neighbour particle", getObject ()->objectID);
                continue;
            }

            Vector2 deltaPos = neighbour->collider->positionOffset - particle->collider->positionOffset;
            double relativeVelocity = (particle->velocity - neighbour->velocity).dotProduct (deltaPos);

            if (relativeVelocity < 0)
                continue;

            double length = deltaPos.magnitude ();
            if (length < FLT_EPSILON)
                length = 0.1;
            relativeVelocity /= length;
            deltaPos /= length;

            double proximity = 1 - length / neighbourRadius;

            Vector2 velocityCorrection = 0.5 * deltaTime * proximity
                * (viscosityLinearDependence * relativeVelocity + viscosityQuadraticDependence * relativeVelocity * relativeVelocity)
                * deltaPos;

            particle->velocity -= velocityCorrection;
        }

    }

}

void FluidBody::advanceParticles () {

    double deltaTime = Time::getInstance ().getDeltaTime ();

    for (auto p : particles) {

        ParticleRef particle = p.second;
        advance (particle->collider, deltaTime);
        
        IntVector2 gridPosition = particle->collider->positionOffset / neighbourRadius;
        particleGrid[gridPosition].push_back (particle);

    }
}

void FluidBody::updateNeighbors () {

    for (auto p : particles) {

        ParticleRef particle = p.second;
        particle->neighbours.clear ();

        IntVector2 gridPosition = particle->collider->positionOffset / neighbourRadius;

        for (auto neighbourPosition : gridPosition.getNeighbours ()) {

            std::list<ParticleRef> potentialNeighbours = particleGrid[neighbourPosition];
            for (auto other : potentialNeighbours) {

                if (particle == other)
                    continue;

                if ((particle->collider->positionOffset - other->collider->positionOffset).magnitudeSqr () < neighbourRadius * neighbourRadius) {
                    particle->neighbours.push_back (other);
                }

            }
        }
    }
}

void FluidBody::doubleDensityRelaxation () {

    double deltaTime = Time::getInstance ().getDeltaTime ();

    for (auto p : particles) {

        ParticleRef particle = p.second;

        double density = 0.0;
        double nearDensity = 0.0;

        for (auto n : particle->neighbours) {

            ParticleRef neighbour = n.lock ();

            Vector2 deltaPos = particle->collider->positionOffset - neighbour->collider->positionOffset;
            double proximity = 1 - deltaPos.magnitude() / neighbourRadius;

            density += proximity * proximity;
            nearDensity += proximity * proximity * proximity;

        }

        double pressure = stiffness * (density - restDensity);
        double nearPressure = nearStiffness * pressure;
        Vector2 positionCorrection;

        for (auto n : particle->neighbours) {

            ParticleRef neighbour = n.lock ();

            Vector2 deltaPos = particle->collider->positionOffset - neighbour->collider->positionOffset;
            double length = deltaPos.magnitude ();
            if (length < FLT_EPSILON)
                length = 0.1;
            double proximity = 1 - length / neighbourRadius;
            deltaPos /= length;

            Vector2 displacement = 0.5 * deltaTime * deltaTime
                * (pressure * proximity + nearPressure * proximity * proximity)
                * deltaPos;

            neighbour->collider->positionOffset += displacement;
            neighbour->velocity += displacement / deltaTime;
            positionCorrection -= displacement;

        }

        particle->collider->positionOffset += positionCorrection;
        particle->velocity += positionCorrection / deltaTime;
    }
}
