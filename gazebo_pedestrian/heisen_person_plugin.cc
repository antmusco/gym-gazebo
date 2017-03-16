#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <random>

#define MEAN 0.0
#define VAR  10.0
#define DELAY_TIME 1000

namespace gazebo {
  
  class HeisenPerson : public ModelPlugin {

    public: HeisenPerson() : rand_generator(std::random_device{}()), 
      normal_dist(MEAN, VAR) {

    }

    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/) {

      printf("Loading heisen_person plugin..\n");
      // Store the pointer to the model
      this->model = _parent;
      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&HeisenPerson::OnUpdate, this, _1));

      // Initialzie the pose randomly.
      this->updatePose();

    }

    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/) {

      // Randomly relocate.
      if(this->delay >= DELAY_TIME) {
        updatePose();
      }
      this->delay++;

    }

    // Places the model randomly (gaussian) around the origin.
    private: void updatePose() {
      double x = this->normal_dist(this->rand_generator);
      double y = this->normal_dist(this->rand_generator);
      math::Pose newPose(x, y, 0.0, 0.0, 0.0, 0.0);
      this->model->SetRelativePose(newPose, true, true);
      this->delay = 0;
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;

    // Random number generator with a gaussian distribution.
    private: std::default_random_engine rand_generator;
    private: std::normal_distribution<double> normal_dist;

    // Delay in updating.
    private: int delay;

  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(HeisenPerson)
}
