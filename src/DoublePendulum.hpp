#pragma once
#include <box2d/box2d.h>

class DoublePendulum {
public:
    DoublePendulum(b2Vec2 gravity, float jointLength, float weightMass, float weightRadius, float angles[2], int weights);
    void step(float dt, int subStepCount);

    float getBasePosition();
    void setBasePosition();

    void applyForceToBase(b2Vec2 force);
    void setBaseVelocity(float velocity);
    void setBaseGoalVelocity(float velocity);
    void resetVelocities();

    void updateVelocity(float dt);
    void update(float dt);
    
    int m_weightAmount;
    b2WorldId m_world;
    b2BodyId m_anchor;
    b2BodyId m_weights[10];
    b2JointId m_joints[10];

    float m_baseVelocity = 0.0f;
    float m_baseGoalVelocity = 0.0f;
};