#ifndef FACTORY_PLUGIN_H
#define FACTORY_PLUGIN_H

#include <ignition/math/Pose3.hh>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"
#include <string>

#define NUM_PERSONS 100
#define NUM_OBJECTS 100

namespace gazebo {

struct ObjectQuantity {
  int num;
  const char* name;
  const char* mesh;
};


class Factory : public WorldPlugin {

  public: Factory();
  public: ~Factory();
  public: void Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/);
  // Static array of visuals.
  public: static std::string person_visuals[];

};

}

#endif
