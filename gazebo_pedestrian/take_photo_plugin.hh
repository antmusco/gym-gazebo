#ifndef GAZEBO_PLUGINS_CAMERAPLUGIN_HH_
#define GAZEBO_PLUGINS_CAMERAPLUGIN_HH_

#include <string>

#include "gazebo/common/Plugin.hh"
#include "gazebo/sensors/CameraSensor.hh"
#include "gazebo/rendering/Camera.hh"
#include "gazebo/util/system.hh"

namespace gazebo {

  class GAZEBO_VISIBLE CameraPlugin : public SensorPlugin {

    public: CameraPlugin();

    /// \brief Destructor
    public: virtual ~CameraPlugin();

    public: virtual void Load(sensors::SensorPtr _sensor, 
        sdf::ElementPtr _sdf);

    public: virtual void OnNewFrame(const unsigned char *_image, 
        unsigned int _width, unsigned int _height, unsigned int _depth, 
        const std::string &_format);

    protected: unsigned int width, height, depth;
    protected: std::string format;
    protected: unsigned int frame_no;
    protected: unsigned int pic_no;

    protected: sensors::CameraSensorPtr parentSensor;
    protected: rendering::CameraPtr camera;

    private: event::ConnectionPtr newFrameConnection;

  };

}
#endif
