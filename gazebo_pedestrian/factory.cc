#include "factory.hh"
#include <random>
#include <fstream>
#include <string>

#define NUM_PERSONS 10

using namespace gazebo;

Factory::Factory() {

  printf("initializing...");
  if(person_visuals == nullptr) {
    person_visuals = new std::string[NUM_PERSONS];
    for(int i = 0; i < NUM_PERSONS; ++i) { 
      person_visuals[i] = nullptr;
    }
    printf("done.\n");
  }

}

Factory::~Factory() {
  delete[] person_visuals;
  person_visuals = nullptr;
}

void Factory::Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/) {

  std::ifstream person_ifs("models/heisen_person/model.sdf");
  std::string person_sdf_str(
      (std::istreambuf_iterator<char>(person_ifs)),
      (std::istreambuf_iterator<char>())
  );
  sdf::SDF person_sdf;
  person_sdf.SetFromString(person_sdf_str);
  sdf::ElementPtr person_model = person_sdf.Root()->GetElement("model");
  printf("Got model\n");
  sdf::ElementPtr person_visual = person_model->GetElement("link")->GetElement("visual");
  printf("Got visual\n");
  std::string base_name("heisen_person_");
  std::string base_visual("heisen_person_visual_");

  // Insert NUM_PERSONS models into the gazebo.
  for(int i = 0; i < NUM_PERSONS; ++i) {
    std::string name = base_name + std::to_string(i+1);
    std::string visual = base_visual + std::to_string(i+1);
    person_model->GetAttribute("name")->SetFromString(name);
    person_visual->GetAttribute("name")->SetFromString(visual);
    person_visuals[i] = visual;
    _parent->InsertModelSDF(person_sdf);
  }
    
  // Insert single camera.
  std::ifstream cam_ifs("models/heisen_camera/model.sdf");
  std::string cam_sdf_str(
      (std::istreambuf_iterator<char>(cam_ifs)),
      (std::istreambuf_iterator<char>())
  );
  sdf::SDF camera_sdf;
  camera_sdf.SetFromString(cam_sdf_str);
  sdf::ElementPtr camera_model = camera_sdf.Root()->GetElement("model");
  camera_model->GetAttribute("name")->SetFromString("heisen_camera");
  _parent->InsertModelSDF(camera_sdf);

}

// Register this plugin with the simulator
GZ_REGISTER_WORLD_PLUGIN(Factory)
