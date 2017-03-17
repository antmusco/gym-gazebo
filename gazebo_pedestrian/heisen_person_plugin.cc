#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <random>

#define X_MEAN 0.0
#define X_VAR  25.0
#define Y_MEAN 0.0
#define Y_VAR  25.0
#define W_LO   0.0
#define W_HI   6.28318
#define DELAY  0

namespace gazebo {
  
/**
 * Person model that spontaneously teleoprts every second.
 */
class HeisenPerson : public ModelPlugin {

  /**
   * Default constructor.
   */
  public: HeisenPerson() : 
      rand_generator(std::random_device{}()),
      x_dist(X_MEAN, X_VAR),
      y_dist(Y_MEAN, Y_VAR),
      w_dist(W_LO,   W_HI) {

    // Empty.

  }

  /**
   * Loads the HeisenPerson plugin for the model.
   */
  public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/) {

    // Store the pointer to the model
    this->model = _parent;

    // Listen to the update event. This event is broadcast every simulation
    // iteration.
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
        boost::bind(&HeisenPerson::OnUpdate, this, _1));

    // Initialzie the pose randomly.
    this->updatePose();

  }

  // Called by the world update start event
  public: void OnUpdate(const common::UpdateInfo & /*_info*/) {

    // Randomly relocate.
    if(this->delay >= DELAY) {
      updatePose();
    }
    this->delay++;

  }

  // Places the model randomly around the origin, with a random orientation.
  private: void updatePose() {
    double x = this->x_dist(this->rand_generator);
    double y = this->y_dist(this->rand_generator);
    double w = this->w_dist(this->rand_generator);
    math::Pose newPose(x, y, 0.0, 0.0, 0.0, w);
    this->model->SetRelativePose(newPose, true, true);
    this->delay = 0;
  }

  // Pointer to the model
  private: physics::ModelPtr model;

  // Pointer to the update event connection
  private: event::ConnectionPtr updateConnection;

  // Random number generator with a gaussian distribution.
  private: std::default_random_engine             rand_generator;
  private: std::normal_distribution<double>       x_dist;
  private: std::normal_distribution<double>       y_dist;
  private: std::uniform_real_distribution<double> w_dist;

  // Delay in updating.
  private: int delay;

};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(HeisenPerson)
}
