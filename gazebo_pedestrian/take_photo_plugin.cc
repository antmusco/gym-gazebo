#include "take_photo_plugin.hh"
#include "factory.hh"

using namespace gazebo;


CameraPlugin::CameraPlugin() : 
  SensorPlugin(), width(0), height(0), depth(0), frame_no(0), pic_no(0) {
}

CameraPlugin::~CameraPlugin() {
  this->parentSensor.reset();
  this->camera.reset();
}

void CameraPlugin::Load(sensors::SensorPtr _sensor, sdf::ElementPtr /*_sdf*/) {

  if (!_sensor) {
    gzerr << "Invalid sensor pointer.\n";
  }

  this->parentSensor =
    std::dynamic_pointer_cast<sensors::CameraSensor>(_sensor);

  if (!this->parentSensor) {
    gzerr << "CameraPlugin requires a CameraSensor.\n";
  }

  this->camera = this->parentSensor->Camera();

  if (!this->parentSensor) {
    gzerr << "CameraPlugin not attached to a camera sensor\n";
    return;
  }

  this->width = this->camera->ImageWidth();
  this->height = this->camera->ImageHeight();
  this->depth = this->camera->ImageDepth();
  this->format = this->camera->ImageFormat();

  this->newFrameConnection = this->camera->ConnectNewImageFrame(
      std::bind(&CameraPlugin::OnNewFrame, this,
        std::placeholders::_1, std::placeholders::_2, 
        std::placeholders::_3, std::placeholders::_4, 
        std::placeholders::_5));

  this->parentSensor->SetActive(true);

}

void CameraPlugin::OnNewFrame(const unsigned char * _image,
    unsigned int _width, unsigned int _height, unsigned int _depth, 
    const std::string &_format) {

  // One frame per second.
  if(++frame_no == 30) {

    // Count visuals.
    int visuals_count = 0;
    for(int i = 0; i < NUM_PERSONS; ++i) {
      if(this->camera->IsVisible(person_visuals[i])) {
        visuals_count++;
      }
    }

    // Construct Filename.
    std::string name = "img/capture_" + std::to_string(++this->pic_no) + 
        "_sees_" + std::to_string(visuals_count) + ".jpg";

    // Save image.
    rendering::Camera::SaveFrame(_image, this->width,
      this->height, this->depth, this->format, name);

    // Reset frame number.
    frame_no = 0;
  }
 
}

GZ_REGISTER_SENSOR_PLUGIN(CameraPlugin)
