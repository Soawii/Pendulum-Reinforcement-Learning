#include <box2d/box2d.h>
#include "DoublePendulum.hpp"
#include "conf.hpp"
#include <iostream>

DoublePendulum::DoublePendulum(b2Vec2 gravity, float jointLength, float weightMass, float weightRadius, float angles[2], int weightAmount) {
    m_weightAmount = weightAmount;
    
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = gravity;
    worldDef.enableContinuous = false;
    m_world = b2CreateWorld(&worldDef);
    b2World_EnableWarmStarting(m_world, true);
    
    b2BodyDef groundDef = b2DefaultBodyDef();
    groundDef.type = b2_staticBody;
    groundDef.position = {0.0f, 0.0f};
    b2BodyId groundBody = b2CreateBody(m_world, &groundDef);

    b2Circle circle = { { 0.0f, 0.0f }, weightRadius };

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = weightMass / (conf::constants::PI * weightRadius * weightRadius);
    shapeDef.filter.maskBits = 0;

	b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
	jointDef.length = jointLength;
	jointDef.minLength = jointLength;
	jointDef.maxLength = jointLength;
	jointDef.enableSpring = false;
	jointDef.enableLimit = false;
    jointDef.collideConnected = false;

    b2MassData massData;
    massData.mass = 1000000.0f;
    massData.center = {0.0f, 0.0f};
    massData.rotationalInertia = 1.0f;
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    m_anchor = b2CreateBody( m_world, &bodyDef );
    b2Body_SetMassData(m_anchor, massData);

    b2PrismaticJointDef pj = b2DefaultPrismaticJointDef();
    pj.bodyIdA = groundBody;              // static
    pj.bodyIdB = m_anchor;                // now dynamic
    pj.localAnchorA = {0.0f,0.0f};
    pj.localAnchorB = {0.0f,0.0f};
    pj.localAxisA = {1.0f,0.0f}; // X-axis
    pj.enableLimit = true;
    pj.lowerTranslation = conf::sim::rangeLeft;
    pj.upperTranslation = conf::sim::rangeRight;
    b2JointId joint = b2CreatePrismaticJoint(m_world, &pj);

    b2BodyId prevBodyId = m_anchor;

    b2Vec2 prevPos = {0.0f, 0.0f};
    for (int i = 0; i < m_weightAmount; i++) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.angularDamping = 0.0f;
        bodyDef.linearDamping = 0.0f;
        bodyDef.position = { prevPos.x + jointLength * cos(angles[i]), prevPos.y + jointLength * sin(angles[i]) };
        m_weights[i] = b2CreateBody(m_world, &bodyDef);
        b2CreateCircleShape(m_weights[i], &shapeDef, &circle);

        b2Vec2 pivotA = { prevPos.x, prevPos.y };
        b2Vec2 pivotB = { prevPos.x + jointLength * cos(angles[i]), prevPos.y + jointLength * sin(angles[i]) };
        jointDef.bodyIdA = prevBodyId;
        jointDef.bodyIdB = m_weights[i];
        jointDef.localAnchorA = b2Body_GetLocalPoint(jointDef.bodyIdA, pivotA);
        jointDef.localAnchorB = b2Body_GetLocalPoint(jointDef.bodyIdB, pivotB);
        m_joints[i] = b2CreateDistanceJoint(m_world, &jointDef);

        prevBodyId = m_weights[i];
        prevPos = pivotB;
    }
}

float DoublePendulum::getBasePosition() {
    return b2Body_GetPosition(m_anchor).x;
}

void DoublePendulum::step(float dt, int subStepCount) {
    b2World_Step(m_world, dt, subStepCount);
}   
void DoublePendulum::applyForceToBase(b2Vec2 force) {
    b2Body_ApplyForceToCenter(m_anchor, force, true);
}
void DoublePendulum::setBaseVelocity(float velocity) {
    b2Body_SetLinearVelocity(m_anchor, {velocity, 0.0f});
}
void DoublePendulum::setBaseGoalVelocity(float velocity) {
    m_baseGoalVelocity = velocity;
}
void DoublePendulum::resetVelocities() {
    for (int i = 0; i < m_weightAmount; i++) {
        b2Body_SetLinearVelocity(m_weights[i], {0.0f, 0.0f});
    }
}

void DoublePendulum::updateVelocity(float dt) {
    float dv = dt * conf::sim::baseMaxAcceleration;
    float direction = m_baseGoalVelocity - m_baseVelocity;
    if (abs(direction) > 0.0001) {
        m_baseVelocity += dv * direction;
        m_baseVelocity = std::min(m_baseVelocity, conf::sim::baseGoalVelocity);
        m_baseVelocity = std::max(m_baseVelocity, -conf::sim::baseGoalVelocity);
    }
    setBaseVelocity(m_baseVelocity);
}
void DoublePendulum::update(float dt) {
    step(dt, conf::sim::substepCount);
    updateVelocity(dt);
}