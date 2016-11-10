#ifndef PARTICLE_H
#define PARTICLE_H



#include "Point.h"



class Particle
{
    public:
        Particle();
        Particle(const Particle& p);
        Particle(Point& nposition, double& nmass);
        Particle(Point& nposition);
        Particle(Point& nposition, double& nmass, double& nradius);

        virtual ~Particle();

        Point getPos() const;
        Point getVelocity() const;
        double getMass() const;
        double getRadius() const;

        double giveRad();
        void setPos(Point& newPos);
        void setVelocity(Point& newVeloc);
        void setVelocity(double x);

        void changePos(double x, double y);
        void invertX();
        void invertY();


        void printPosition();



    protected:
    private:
        Point position;
        Point velocity;
        double mass;
        double radius;

};

#endif // PARTICLE_H
