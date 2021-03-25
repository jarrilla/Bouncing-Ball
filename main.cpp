#include <iostream>
#include <vector>

using namespace std;

class Particle {
private:
  vector<double> position;
  vector<double> velocity;
  double mass;

public:
  Particle(
    vector<double> position,
    vector<double> velocity,
    double mass
  ) {
    this->position = position;
    this->velocity = velocity;
    this->mass = mass;
  }

  vector<double> GetPosition() {
    return position;
  }

  vector<double> GetVelocity() {
    return velocity;
  }

  void SetVx(double vx) {
    (this->velocity).at(0) = vx;
  }

  void SetVy(double vy) {
    (this->velocity).at(1) = vy;
  }

  void SetPx(double px) {
    (this->position).at(0) = px;
  }

  void SetPy(double py) {
    (this->position).at(1) = py;
  }
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

void flip_vx(Particle& p) {
  p.SetVx( -p.GetVelocity().at(0) );
}

void flip_vy(Particle& p) {
  p.SetVy( -p.GetVelocity().at(1) );
}

void checkCollision(int testIndex, vector<Particle>& particles) {
  auto& testP = particles.at(testIndex);
  
  auto currPos = testP.GetPosition();
  double px = currPos.at(0);
  double py = currPos.at(1);

  auto currVel = testP.GetVelocity();
  double vx = currVel.at(0);
  double vy = currVel.at(1);
  
  double t_px = px + vx*Dt;
  double t_py = py + vy*Dt;

  // cout << "test p=(" << t_px <<", " << t_py <<")"<<endl;

  // if we hit a bound, reflect
  bool hit = false;
  if (t_px > MAX_X || t_px < MIN_X) {
    hit = true;
    flip_vx(testP);
  }
  else testP.SetPx(t_px);

  if (t_py > MAX_Y || t_py < MIN_Y) {
    hit = true;
    flip_vy(testP);
  }
  else testP.SetPy(t_py);

  if (hit) {
    // cout << "p" << testIndex << " bound collision" << endl;
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
      flip_vx(testP);
    }
    else testP.SetPx(t_px);

    if (t_py == pp.at(1)) {
      hit = true;
      flip_vy(testP);
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