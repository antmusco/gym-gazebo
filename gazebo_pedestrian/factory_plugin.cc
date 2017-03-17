#include "factory_plugin.hh"
#include <random>
#include <fstream>
#include <string>

using namespace gazebo;


#define OBJ_COUNT 1
const struct ObjectQuantity OBJS[] = {
  {15, "cone", "construction_cone/meshes/construction_cone.dae"},
};

std::string Factory::person_visuals[NUM_PERSONS];

Factory::Factory() {

}

Factory::~Factory() {

}

void Factory::Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/) {

  // Load the model sdf string.
  std::ifstream person_ifs("models/heisen_person/model.sdf");
  std::string person_sdf_str(
      (std::istreambuf_iterator<char>(person_ifs)),
      (std::istreambuf_iterator<char>())
  );
  
  // Extract the appropriate pointers for the model and the visual.
  sdf::SDF person_sdf;
  person_sdf.SetFromString(person_sdf_str);
  sdf::ElementPtr person_model  = person_sdf.Root()->GetElement("model");
  sdf::ElementPtr person_link   = person_model->GetElement("link");
  sdf::ElementPtr person_visual = person_link->GetElement("visual");
  std::string base_model_name("heisen_person_");
  std::string base_visual_name("heisen_person_visual_");
  std::string link_name = person_link->GetAttribute("name")->GetAsString();

  // Insert NUM_PERSONS models into the gazebo.
  for(int i = 0; i < NUM_PERSONS; ++i) {
    std::string index = std::to_string(i+1);
    std::string model_name  = base_model_name + index;
    std::string visual_name = base_visual_name + index;
    person_model->GetAttribute("name")->SetFromString(model_name);
    person_visual->GetAttribute("name")->SetFromString(visual_name);
    person_visuals[i] = model_name + "::" + link_name + "::" + visual_name;
    _parent->InsertModelSDF(person_sdf);
  }

  // Now we can insert random objects.
  sdf::ElementPtr object_collision = person_link->GetElement("collision")
      ->GetElement("geometry")->GetElement("mesh")->GetElement("uri");
  sdf::ElementPtr object_visual = person_link->GetElement("visual")
      ->GetElement("geometry")->GetElement("mesh")->GetElement("uri");
  sdf::ElementPtr collision_name = person_link->GetElement("collision");

  std::string mesh_base = "model://";
  for(int i = 0; i < OBJ_COUNT; ++i) {
    for(int j = 0; j < OBJS[i].num; ++j) {
      int index = j+1;
      std::string model_name = std::string(OBJS[i].name) + std::to_string(index);
      person_model->GetAttribute("name")->SetFromString(model_name);
      person_visual->GetAttribute("name")->SetFromString(model_name + "_visual");
      collision_name->GetAttribute("name")->SetFromString(model_name + "_collision");
      std::cout << person_sdf.ToString() << std::endl;
      object_collision->Set(std::string(mesh_base + OBJS[i].mesh));
      object_visual->Set(std::string(mesh_base + OBJS[i].mesh));
      _parent->InsertModelSDF(person_sdf);
    }
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
