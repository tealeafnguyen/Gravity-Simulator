#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Point.h"


Point::Point()
{
    srand(time(0));
    x = rand() % 800;
    y = rand() % 600;
}

Point::Point(double initx, double inity)
{
    x = initx;
    y = inity;
}

double Point::getX() const
{
    return x;
}
double Point::getY() const
{
    return y;
}

void Point::printPos()
{
    std::cout<<"x: "<<x<<" y: "<<y<<std::endl;
}

void Point::invertvX()
{
    x = -x;
}
void Point::invertvY()
{
    y = -y;
}

Point::~Point()
{
    //dtor
}
