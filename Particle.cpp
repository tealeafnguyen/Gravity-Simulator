#include <cmath>
#include <cstdlib>
#include <ctime>
#include <tgmath.h>
#include <iostream>

#include "Particle.h"

Point position;
Point velocity;
double mass;
double radius;

Particle::Particle()
{
    srand(time(0));
    Point pos(rand() % 800, rand() % 600);
    Point velo(0.0, 0.0);
    position = pos;
    mass = ((double) rand() / RAND_MAX) * 1.9e27 + 1.0;
    radius = giveRad();
}

Particle::~Particle()
{
    //dtor
}

Particle::Particle(const Particle& p)
{
    position = p.getPos();
    velocity = p.getVelocity();
    mass = p.getMass();
    radius = p.getRadius();
}

Particle::Particle(Point& nposition, double& nmass)
{
    position = nposition;
    mass = nmass;
    radius = giveRad();
    Point velo(0.0, 0.0);
    velocity = velo;
}

Particle:: Particle(Point& nposition)
{
    srand(time(0));
    position = nposition;
    mass = ((double) rand() / RAND_MAX) * 1.9e27 + 1.0;
    radius = giveRad();
    Point velo(0.0, 0.0);
    velocity = velo;
}

Point Particle::getPos() const
{
    return position;
}

Point Particle::getVelocity() const
{
    return velocity;
}
double Particle::getMass() const
{
    return mass;
}
double Particle::getRadius() const
{
    return radius;
}


void Particle::invertX()
{
    velocity.invertvX();
}

void Particle::invertY()
{
    velocity.invertvY();
}

double Particle::giveRad()
{
    if(mass >= 1e27)
    {
        return 5;
    }
    else if(mass < 1e27 && mass >= 1e26)
    {
        return 3;
    }
    else
    {
        return 1;
    }
}

void Particle::setPos(Point& newPos)
{
    Point tempPos = newPos;

    position = newPos;
}
void Particle::setVelocity(Point& newVeloc)
{
    Point tempVelo = newVeloc;

    velocity = tempVelo;
}


void Particle::printPosition()
{
    position.printPos();
}
