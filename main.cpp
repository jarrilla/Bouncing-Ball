/**
 * Simple collision simulator.
 * At every time step we check our array of particles to see if any of them have collided.
 * A collision between A & B is determined if A's area intersects B's area (using simple circular area assumption).
 * If they collided, use conservation of momentum (assume fully elastic) to determine final velocity.
 * 
 * TODO:
 * - Consider having a N x N matrix every time step that crosses off which interactions have been checked, so we don't do repeat calculations.
 * - Add initial temperature conditions. Randomly distribute initial velocities according to some distribution.
 * - Think about how to simulate a constant heat source (so that temperature doesn't just drop to zero over time).
 * */

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class Particle {
private:
  vector<double> position;
  vector<double> velocity;
  double radius;

public:
  Particle(
    vector<double> position,
    vector<double> velocity,
    double radius
  ) {
    this->position = position;
    this->velocity = velocity;
    this->radius = radius;
  }

// GETTERS
  vector<double> GetPosition() const { return position; }
  vector<double> GetVelocity() const { return velocity; }

  double GetPx() const { return position.at(0); }
  double GetPy() const { return position.at(1); }

  double GetVx() const { return velocity.at(0); }
  double GetVy() const { return velocity.at(1); }

  double GetRadius() const { return radius; }

// MUTATORS
  void SetPx(double px) { (this->position).at(0) = px; }
  void SetPy(double py) { (this->position).at(1) = py; }

  void SetVx(double vx) { (this->velocity).at(0) = vx; }
  void SetVy(double vy) { (this->velocity).at(1) = vy; }
};

// Box bounds
int MIN_X = 0, MAX_X = 1, MIN_Y = 0, MAX_Y = 1;

// time step
double Dt = 0.001;

// number of particles
int N = 2;

void printV(vector<double> v) {
  cout << '(' << v.at(0) << ", " << v.at(1) << ')' << endl;
}

/**
 * Flip the specified component of the velocity of a given particle.
 * */
inline void flipVx(Particle& p) { p.SetVx( -p.GetVx() ); }
inline void flipVy(Particle& p) { p.SetVy( -p.GetVy() ); }

/**
 * Compute the Eucledian distance between p1 & p2
 * */
double getDistance(const Particle& p1, const Particle& p2) {
  double delta_x = p1.GetPx() - p2.GetPx();
  double delta_y = p1.GetPy() - p2.GetPy();

  return sqrt( delta_x*delta_x + delta_y*delta_y );
}

/**
 * Check all particles against collisions, sequentially.
 * 1. Check if particle has "hit" a wall (check radius against bounds)
 *   - Flip velocity vector.
 * 2. If still in bounds, check its radius against all other particles in the system.
 *   - Calculate new trajectory using conservation of momentum.
 * */
void checkCollision(int testIndex, vector<Particle>& particles) {
  
  auto& testP = particles.at(testIndex);
  const double px = testP.GetPx(), py = testP.GetPy();
  const double vx = testP.GetVx(), vy = testP.GetVy();
  const double pr = testP.GetRadius();
  
  // new position candidates
  double t_px = px + vx*Dt;
  double t_py = py + vy*Dt;

  // if we hit a bound, flip velocity vector
  bool hit = false;
  if (t_px + pr > MAX_X || t_px - pr < MIN_X) {
    hit = true;
    flipVx(testP);
  }
  else testP.SetPx(t_px);

  if (t_py + pr > MAX_Y || t_py - pr < MIN_Y) {
    hit = true;
    flipVy(testP);
  }
  else testP.SetPy(t_py);

  // If we hit a bound, we're done with this particle
  if (hit) {
    // NOTE: not sure this is actually the case.. we may still have to check other collisions
    // Think about this....
    return;
  };


  // still in bounds, check collision against other particles
  for (int i = 0; i < particles.size(); i++) {
    if (i == testIndex) continue;

    auto p = particles.at(i);
    auto pp = p.GetPosition();

    // this will probably never happen...
    // we need to add a hit-radius to every particle
    if (t_px == pp.at(0)) {
      hit = true;
      flipVx(testP);
    }
    else testP.SetPx(t_px);

    if (t_py == pp.at(1)) {
      hit = true;
      flipVy(testP);
    }
    else testP.SetPy(t_py);

    if (hit) {
      cout << "particle collision: " << testIndex << "+" << i << endl;
      return;
    }
  }
}

int main() {
  srand( (unsigned)time(NULL) );
  vector<Particle> particles;
  for (int i = 0; i < N; i++) {
    double px = (double)rand() / RAND_MAX *MAX_X;
    double py = (double)rand() / RAND_MAX *MAX_Y;

    double vx = (double)rand() / RAND_MAX * 3;
    double vy = (double)rand() / RAND_MAX * 3;

    cout << i << ": p=(" << px << ", " << py << "); v=(" << vx << ", " << vy << ")" << endl;

    Particle p{ {px, py}, {vx, vy}, 2 };
    particles.push_back(p);
  }

  // start sim loop
  double time = 0;
  while (time < 100) {

    for (int i = 0; i < N; i++) {
      checkCollision(i, particles);
    }

    time += Dt;
  }

  return 0;
}