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

  void SetPosition(vector<double> p) {
    this->position = p;
  }
};

void printV(vector<double> v) {
  for(auto &&it = v.begin(); it != v.end(); ++it) {
    cout << *it << ' ';
  }
  cout << endl;
}

int main() {
  double dt = 0.1;
  vector<double> p{0, 0};
  vector<double> v{1, 1};
  Particle p1{p,v,2};

  double time = 0;
  while (time < 1) {
    auto pos = p1.GetPosition();
    auto vel = p1.GetVelocity();

    double px = pos.at(0);
    double py = pos.at(1);
    double vx = vel.at(0);
    double vy = vel.at(1);

    double newX = px + vx*dt;
    double newY = py + vy*dt;
    p1.SetPosition( vector<double>{newX, newY} );

    printV( p1.GetPosition() );
    time += dt;
  }

  return 0;
}