#ifndef POINT_H
#define POINT_H


class Point
{
    public:
        Point();
        Point(double initx, double inity);
        double getX() const;
        double getY() const;
        virtual ~Point();
        void printPos();
        void invertvX();
        void invertvY();

    protected:
    private:
        double x;
        double y;
};

#endif // POINT_H
