#pragma once
#include <box2d/box2d.h>
#include <vector>

class DoublePendulum {
public:
    DoublePendulum(b2Vec2 gravity, float jointLength, float weightMass, float weightRadius, int weightAmount, std::vector<float> angles);
    void step(float dt, int subStepCount);

    float getBasePosition();
    void setBasePosition();

    b2BodyId getInteresectingBody(b2Vec2 p);

    void applyForceToBase(b2Vec2 force);
    void setBaseVelocity(float velocity);
    void setBaseGoalVelocity(float velocity);
    void resetVelocities();

    void updateVelocity(float dt);
    void update(float dt);
    
    float m_weightRadius;
    int m_weightAmount;
    b2WorldId m_world;
    b2BodyId m_anchor;
    b2BodyId m_weights[10];
    b2JointId m_joints[10];

    float m_baseVelocity = 0.0f;
    float m_baseGoalVelocity = 0.0f;
};