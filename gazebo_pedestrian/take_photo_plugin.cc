#include "take_photo_plugin.hh"
#include "factory_plugin.hh"

using namespace gazebo;

// Constructor.
CameraPlugin::CameraPlugin() : 
    SensorPlugin(), 
    width(0), 
    height(0), 
    depth(0), 
    frame_no(0), 
    pic_no(0) {

  // Empty.

}

// Destructor.
CameraPlugin::~CameraPlugin() {
  this->parentSensor.reset();
  this->camera.reset();
}

/**
 * Loads the CameraPlugin into Gazebo.
 *
 * @param _sensor The sensor pointed to.
 */
void CameraPlugin::Load(sensors::SensorPtr _sensor, sdf::ElementPtr /*_sdf*/) {

  if (!_sensor) {
    gzerr << "Invalid sensor pointer.\n";
  }

  // The sensor should be a CameraSensor, but if not error out.
  this->parentSensor = std::dynamic_pointer_cast<
      sensors::CameraSensor>(_sensor);
  if (!this->parentSensor) {
    gzerr << "CameraPlugin requires a CameraSensor.\n";
  }

  // Grab the camera and the camera image properties.
  this->camera = this->parentSensor->Camera();
  this->width  = this->camera->ImageWidth();
  this->height = this->camera->ImageHeight();
  this->depth  = this->camera->ImageDepth();
  this->format = this->camera->ImageFormat();

  // Bind the update function to the camera.
  this->newFrameConnection = this->camera->ConnectNewImageFrame(
      std::bind(&CameraPlugin::OnNewFrame, this,
        std::placeholders::_1, std::placeholders::_2, 
        std::placeholders::_3, std::placeholders::_4,
        std::placeholders::_5));

  // Activate the sensor.
  this->parentSensor->SetActive(true);

}

void CameraPlugin::OnNewFrame(const unsigned char * _image,
    unsigned int _width, unsigned int _height, unsigned int _depth, 
    const std::string &_format) {

  // One frame per second.
  if(++frame_no >= 0) {

    // Count visuals.
    int visuals_count = 0;
    for(int i = 0; i < NUM_PERSONS; ++i) {
      if(this->camera->IsVisible(Factory::person_visuals[i])) {
        visuals_count++;
      }
    }

    // Only keep images where we see someone.
    if(visuals_count > 0) {
      // Construct Filename.
      std::string name = "img/capture_" + std::to_string(++(this->pic_no)) +
          "_sees_" + std::to_string(visuals_count) + ".jpg";
      // Save image.
      rendering::Camera::SaveFrame(_image, this->width, this->height,
          this->depth, this->format, name);
    }

    // We've got enough.
    if(this->pic_no >= 1000) {
      gazebo::shutdown();
    }

    // Reset frame number.
    frame_no = 0;

  }
 
}

GZ_REGISTER_SENSOR_PLUGIN(CameraPlugin)
