#ifndef GAZEBO_PLUGINS_CAMERAPLUGIN_HH_
#define GAZEBO_PLUGINS_CAMERAPLUGIN_HH_

#include <string>

#include "gazebo/common/Plugin.hh"
#include "gazebo/sensors/CameraSensor.hh"
#include "gazebo/rendering/Camera.hh"
#include "gazebo/util/system.hh"

namespace gazebo {

/**
 * Plugin which takes photos from a camera every second.
 */
class GAZEBO_VISIBLE CameraPlugin : public SensorPlugin {

  // Constructor/Destructor.
  public: CameraPlugin();
  public: virtual ~CameraPlugin();

  // Load the camera plugin.
  public: virtual void Load(sensors::SensorPtr _sensor, 
      sdf::ElementPtr _sdf);

  // On every frame.
  public: virtual void OnNewFrame(const unsigned char *_image, 
      unsigned int _width, unsigned int _height, unsigned int _depth, 
      const std::string &_format);

  // Width, height, and depth of the image.
  protected: unsigned int width, height, depth;
  // Format of the image.
  protected: std::string format;
  // Frame number (0-30, 30 per second).
  protected: unsigned int frame_no;
  // Number of pictures taken.
  protected: unsigned int pic_no;

  // Pointer to the CameraSensor.
  protected: sensors::CameraSensorPtr parentSensor;
  // Pointer to the Camera.
  protected: rendering::CameraPtr camera;

  // ???
  private: event::ConnectionPtr newFrameConnection;

};

}
#endif
