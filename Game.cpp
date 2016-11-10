#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Game.h"

Game::Game() : start(0), last(0), current(0), good(true), running(false),
        width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT),
        particles(std::vector<Particle>())
{
    // Seed the random number generator
    srand(time(0));

    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        good = false;
        return;
    }

    // initialize SDL window
    window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        good = false;
        return;
    }

    // initialize SDL renderer
    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        good = false;
        return;
    }

    // initialize particle texture
    SDL_Surface* bmp = SDL_LoadBMP("particle.bmp");
    if (bmp == NULL)
    {
        good = false;
        return;
    }
    particleTexture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (particleTexture == NULL)
    {
        good = false;
        return;
    }

    createParticles();

    // initialize our particles
//    for (int i = 0; i < PARTICLE_COUNT; ++i)
//    {
//        particles.push_back(randomParticle());
//    }
}


//This is only here because my compiler doesn't make rand() work properly and would give me the same number during the 25 loops
//for making a random particle. This method is solely for testing.
void Game::createParticles()
{
    double xaxis[] = {241, 432, 532, 482, 191, 201, 93, 10, 711, 684, 394, 411, 522, 650, 111, 444, 666, 777, 210, 50, 75, 680, 161, 543, 479};
    double yaxis[] = {241, 432, 532, 482, 191, 201, 93, 10, 711, 684, 394, 411, 522, 650, 111, 444, 666, 777, 210, 50, 75, 680, 161, 543, 479};
    for(int i = 0; i < PARTICLE_COUNT; i++)
    {
        Point pos(xaxis[i], yaxis[i]);
        double def = 1.9e27;
        Particle p(pos, def);
        particles.push_back(p);
    }
}

Game::~Game()
{
    if (!good)
    {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }
    if (particleTexture != NULL)
    {
        SDL_DestroyTexture(particleTexture);
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

int Game::operator()()
{
    if (!good)
    {
        return -1;
    }
    running = true;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    start = SDL_GetTicks();
    last = start;
    while (running) // every iteration is 1 frame
    {
        current = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }
        update((current - last) / 1000.0);
        render();
        last = current;
    }
    return 0;
}

void Game::update(double dt)
{
    std::cout << dt << " sec since last frame.\n";

//    if(doOnce == false)
//    {
        calcAccel(dt);
//        doOnce = true;
//    }
//    checkVelocity();

    moveBasedOnVel(dt);
    collisionAlternative();
    bounce();


}


//This is the vector based version of elastic collision that doesn't use trig, but uses dot products
void Game::collisionAlternative()
{
    double xDist, yDist;
    for(int i = 0; i < particles.size(); i++)
    {
        Particle tempA(particles[i]);
        for(int j = i + 1; j < particles.size(); j++)
        {
            Particle tempB(particles[j]);
            xDist = tempA.getPos().getX() - tempB.getPos().getX();
            yDist = tempA.getPos().getY() - tempB.getPos().getY();
            double distanceSqrd = xDist*xDist + yDist*yDist;
            if(distanceSqrd <= (tempA.getRadius() + tempB.getRadius())*(tempA.getRadius() + tempB.getRadius()))
            {
                double xVelocity = tempB.getVelocity().getX() - tempA.getVelocity().getX();
                double yVelocity = tempB.getVelocity().getY() - tempA.getVelocity().getY();
                double dotProduct = xDist*xVelocity + yDist*yVelocity;

                if(dotProduct > 0)
                {
                    double collisionScale = dotProduct / distanceSqrd;
                    double xCollision = xDist * collisionScale;
                    double yCollision = yDist * collisionScale;

                    double combinedMass = tempA.getMass() + tempB.getMass();
                    double collisionWA = 2 * tempB.getMass()/combinedMass;
                    double collisionWB = 2 * tempA.getMass()/combinedMass;

                    Point vA(tempA.getVelocity().getX() + collisionWA*xCollision,
                             tempA.getVelocity().getY() + collisionWA*yCollision);
                    Point vB(tempB.getVelocity().getX() - collisionWB*xCollision,
                             tempB.getVelocity().getY() - collisionWB*yCollision);
                    particles[i].setVelocity(vA);
                    particles[j].setVelocity(vB);
                }
            }
        }
    }
}

void Game::bounce()
{
    for(int i = 0; i < particles.size(); i++)
    {
        if(particles[i].getPos().getX() >= 800.0 || particles[i].getPos().getX() <= 0.0)
        {
            particles[i].invertX();
        }
        if(particles[i].getPos().getY() >= 600.0 || particles[i].getPos().getY() <= 0.0)
        {
            particles[i].invertY();
        }
    }
}

void Game::checkVelocity()
{
    for(int i = 0; i < particles.size(); i++)
    {
        std::cout<<"particle number: "<<i<<" x velo: "<<particles[i].getVelocity().getX()<<" y velo: "<<particles[i].getVelocity().getY()<<std::endl;
    }
}

void Game::invertVelocity()
{
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i].invertX();
        particles[i].invertY();
    }
}


void Game::moveBasedOnVel(double dt) //deal with this shit later
{
    for(int i = 0; i < particles.size(); i++)
    {
        Point nPos((capPositionX(particles[i].getPos().getX() + (particles[i].getVelocity().getX()*dt))),
                  (capPositionY(particles[i].getPos().getY() + particles[i].getVelocity().getY()*dt)));
        particles[i].setPos(nPos);
//        particles[i].printPosition();
    }
}

double Game::capPositionX(double pos)
{
    if (pos > 800.0)
        return 800.0;
    else if(pos < 0.0)
        return 0.0;
    else
        return pos;
}

double Game::capPositionY(double pos)
{
    if (pos > 600.0)
        return 600.0;
    else if(pos < 0.0)
        return 0.0;
    else
        return pos;
}

void Game::calcAccel(double dt)
{
    for(int i = 0; i < particles.size(); i++)
    {
        double accelx = 0;
        double accely = 0;
        for(int j = 0; j < particles.size(); j++)
        {

            if(j != i)
            {
                accelx += accelerationMathx(particles[i],particles[j]);
                accely += accelerationMathy(particles[i],particles[j]);

//                std::cout<<accelx<<" "<<accely<<std::endl;
//                std::cin.get();
            }
        }

        Point newVelo(capVelocity(particles[i].getVelocity().getX() + (accelx*dt)), capVelocity(particles[i].getVelocity().getY() + (accely*dt)));
        particles[i].setVelocity(newVelo);

//        std::cout<<"velocity x of that particle after calc"<<particles[i].getVelocity().getX()<<std::endl;

//        std::cin.get();
    }
}

double Game::capVelocity(double velo)
{
    if(velo > 250.0)
        return 250.0;
    else if(velo < -250.0)
        return -250.0;
    else
        return velo;
}

double Game::accelerationMathx(Particle p1, Particle p2)
{
    if(p1.getPos().getX() < p2.getPos().getX())
        return ((G*p2.getMass())/(pow(xdistance(p1,p2),2)));
    else if(p1.getPos().getX() > p2.getPos().getX())
        return -((G*p2.getMass())/(pow(xdistance(p1,p2),2)));
    else
        return 0;
}

double Game::accelerationMathy(Particle p1, Particle p2)
{
    if(p1.getPos().getY() > p2.getPos().getY())
        return ((G*p2.getMass())/pow(ydistance(p1,p2),2));
    else if(p1.getPos().getY() < p2.getPos().getY())
        return -((G*p2.getMass())/pow(ydistance(p1,p2),2));
    else
        return 0;
}

double Game::xdistance(Particle p1, Particle p2)
{
    double x = p2.getPos().getX() - p1.getPos().getX();
    x += p1.getRadius() + p2.getRadius();

    return x;
}

double Game::ydistance(Particle p1, Particle p2)
{
    double y = p2.getPos().getY() - p1.getPos().getY();
    y += p1.getRadius() + p2.getRadius();

    return y;
}

void Game::render()
{
    SDL_RenderClear(renderer);

    // rendering here would place objects beneath the particles

    for (const Particle& p : particles)
    {
        drawParticle(p);
    }

    // rendering here would place objects on top of the particles

    SDL_RenderPresent(renderer);
}

void Game::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    // Add your own event handling here if desired
    case SDL_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void Game::drawParticle(const Particle& p)
{
    SDL_Rect dst;
    double shift = p.getRadius();
    dst.x = (int) (p.getPos().getX() - shift);
    dst.y = (int) (p.getPos().getY() - shift);
    dst.w = shift * 2;
    dst.h = shift * 2;
    SDL_RenderCopy(renderer, particleTexture, NULL, &dst);
}

Particle Game::randomParticle() const
{

    Point pos(rand() % width, rand() % height);

    // using some percentage of the mass of Jupiter
    double mass = ((double) rand() / RAND_MAX) * 1.9e27 + 1.0;

    return Particle(pos, mass);
}
