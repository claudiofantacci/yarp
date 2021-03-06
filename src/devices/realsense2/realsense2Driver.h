﻿/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */


#ifndef REALSENSE2_DRIVER_H
#define REALSENSE2_DRIVER_H

#include <iostream>
#include <cstring>
#include <map>
#include <mutex>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/FrameGrabberControl2.h>
#include <yarp/os/PeriodicThread.h>
#include <yarp/sig/all.h>
#include <yarp/sig/Matrix.h>
#include <yarp/os/all.h>
#include <yarp/os/Stamp.h>
#include <yarp/dev/IRGBDSensor.h>
#include <yarp/dev/RGBDSensorParamParser.h>
#include <librealsense2/rs.hpp>

namespace yarp
{
    namespace dev
    {
        class realsense2Driver;
    }
}

/**
 *  @ingroup dev_impl_media
 *
 *
 * This device is a YARP plugin for realsense2 compatible devices, and exposes the IRGBDSensor and IFrameGrabberControls
 * interfaces to read the images and operate on the available settings.
 *
 * See the documentation for more details about each interface.
 *
 * This device is paired with its server called RGBDSensorWrapper to stream the images and perform remote operations.
 *
 * The config file is subdivided into 2 major sections called "SETTINGS" and "HW_DESCRIPTION".
 *
 * The "SETTINGS" section is meant for read/write parameters, meaning parameters which can be get and set by the device.
 * A common case of setting is the image resolution in pixel. This setting will be read by the device and it'll be applied
 * in the startup phase. If the setting fails, the device will terminate the execution with a error message.
 *
 * The "HW_DESCRIPTION" section is meant for read only parameters which describe the hardware property of the device and
 * cannot be provided by the device through software API.
 * A common case is the 'Field Of View' property, which may or may not be supported by the physical device.
 * When a property is present in the HW_DESCRIPTION group, the YARP RGBDSensorClient will report this value when asked for
 * and setting will be disabled.
 * This group can also be used to by-pass realsense2 API in case some functionality is not correctly working with the current
 * device. For example the 'clipPlanes' property may return incorrect values or values using non-standard measurement unit.
 * In this case using the HW_DESCRIPTION, a user can override the value got from OpenNI2 API with a custom value.
 *
 * \note Parameters inside the HW_DESCRIPTION are read only, so the relative set functionality will be disabled.
 * \note For parameters which are neither in SETTINGS nor in HW_DESCRIPTION groups, read / write functionality is assumed
 *  but not initial setting will be performed. Device will start with manufacturer default values.
 * \warning A single parameter cannot be present into both SETTINGS and HW_DESCRIPTION groups.
 * \warning whenever more then one value is required by the setting, the values must be in parenthesys!
 *
 *   Parameters used by this device are:
 * | Parameter name               | SubParameter        | Type                |  Read / write   | Units          | Default Value | Required                         | Description                                                                            | Notes                                                                 |
 * |:----------------------------:|:-------------------:|:-------------------:|:---------------:|:--------------:|:-------------:|:--------------------------------:|:--------------------------------------------------------------------------------------:|:---------------------------------------------------------------------:|
 * |  SETTINGS                    |      -              | group               |  Read / write   | -              |   -           |  Yes                             | Initial setting of the device.                                                         |  Properties must be read/writable in order for setting to work        |
 * |                              |  rgbResolution      | int, int            |  Read / write   | pixels         |   -           |  Yes                             | Size of rgb image in pixels                                                            |  2 values expected as height, width                                   |
 * |                              |  depthResolution    | int, int            |  Read / write   | pixels         |   -           |  Yes                             | Size of depth image in pixels                                                          |  Values are height, width                                             |
 * |                              |  accuracy           | double              |  Read / write   | meters         |   -           |  No                              | Accuracy of the device, as the depth measurement error at 1 meter distance             |  Note that only few realsense devices allows to set it                |
 * |                              |  framerate          | int                 |  Read / Write   | fps            |   30          |  No                              | Framerate of the sensor                                                                |                                                                       |
 * |                              |  enableEmitter      | bool                |  Read / Write   | -              |   true        |  No                              | Flag for enabling the IR emitter(if supported by the sensor)                           |                                                                       |
 * |                              |  needAlignment      | bool                |  Read / Write   | -              |   true        |  No                              | Flag for enabling the alignment of the depth frame over the rgb frame                  |  This operation could be heavy, set it to false to increase the fps   |
 * |  HW_DESCRIPTION              |      -              |  group              |                 | -              |   -           |  Yes                             | Hardware description of device property.                                               |  Read only property. Setting will be disabled                         |
 * |                              |  clipPlanes         | double, double      |  Read / write   | meters         |   -           |  No                              | Minumum and maximum distance at which an object is seen by the depth sensor            |  parameter introduced mainly for simulated sensors, it can be used to set the clip planes if Openni gives wrong values |
 *
 * Configuration file using .ini format, using subdevice keyword.
 *
 * \code{.unparsed}

device       RGBDSensorWrapper
subdevice    realsense2
name         /depthCamera

[SETTINGS]
depthResolution (640 480)    #Note the parentesys
rgbResolution   (640 480)
framerate       30
enableEmitter   true
needAlignment   true

[HW_DESCRIPTION]
clipPlanes (0.2 10.0)
 *
 * \endcode
 */



class yarp::dev::realsense2Driver :  public yarp::dev::DeviceDriver,
                                     public yarp::dev::IFrameGrabberControls,
                                     public yarp::dev::IFrameGrabberImageRaw,
                                     public yarp::dev::IRGBDSensor
{
private:
    typedef yarp::sig::ImageOf<yarp::sig::PixelFloat> depthImage;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    typedef yarp::os::Stamp                           Stamp;
    typedef yarp::os::Property                        Property;
    typedef yarp::sig::FlexImage                      FlexImage;

#endif
public:
    realsense2Driver();
    ~realsense2Driver();

    // DeviceDriver
    virtual bool open(yarp::os::Searchable& config) override;
    virtual bool close() override;

    // IRGBDSensor
    virtual int    getRgbHeight() override;
    virtual int    getRgbWidth() override;
    virtual bool   getRgbSupportedConfigurations(yarp::sig::VectorOf<CameraConfig> &configurations) override;
    virtual bool   getRgbResolution(int &width, int &height) override;
    virtual bool   setRgbResolution(int width, int height) override;
    virtual bool   getRgbFOV(double& horizontalFov, double& verticalFov) override;
    virtual bool   setRgbFOV(double horizontalFov, double verticalFov) override;
    virtual bool   getRgbMirroring(bool& mirror) override;
    virtual bool   setRgbMirroring(bool mirror) override;

    virtual bool   getRgbIntrinsicParam(Property& intrinsic) override;
    virtual int    getDepthHeight() override;
    virtual int    getDepthWidth() override;
    virtual bool   setDepthResolution(int width, int height) override;
    virtual bool   getDepthFOV(double& horizontalFov, double& verticalFov) override;
    virtual bool   setDepthFOV(double horizontalFov, double verticalFov) override;
    virtual bool   getDepthIntrinsicParam(Property& intrinsic) override;
    virtual double getDepthAccuracy() override;
    virtual bool   setDepthAccuracy(double accuracy) override;
    virtual bool   getDepthClipPlanes(double& nearPlane, double& farPlane) override;
    virtual bool   setDepthClipPlanes(double nearPlane, double farPlane) override;
    virtual bool   getDepthMirroring(bool& mirror) override;
    virtual bool   setDepthMirroring(bool mirror) override;


    virtual bool   getExtrinsicParam(sig::Matrix &extrinsic) override;
    virtual bool   getRgbImage(FlexImage& rgbImage, Stamp* timeStamp = nullptr) override;
    virtual bool   getDepthImage(depthImage& depthImage, Stamp* timeStamp = nullptr) override;
    virtual bool   getImages(FlexImage& colorFrame, depthImage& depthFrame, Stamp* colorStamp=NULL, Stamp* depthStamp=NULL) override;

    virtual RGBDSensor_status     getSensorStatus() override;
    virtual std::string getLastErrorMsg(Stamp* timeStamp = NULL) override;

    //IFrameGrabberControls
    virtual bool   getCameraDescription(CameraDescriptor *camera) override;
    virtual bool   hasFeature(int feature, bool*   hasFeature) override;
    virtual bool   setFeature(int feature, double  value) override;
    virtual bool   getFeature(int feature, double* value) override;
    virtual bool   setFeature(int feature, double  value1,  double  value2) override;
    virtual bool   getFeature(int feature, double* value1,  double* value2) override;
    virtual bool   hasOnOff(  int feature, bool*   HasOnOff) override;
    virtual bool   setActive( int feature, bool    onoff) override;
    virtual bool   getActive( int feature, bool*   isActive) override;
    virtual bool   hasAuto(   int feature, bool*   hasAuto) override;
    virtual bool   hasManual( int feature, bool*   hasManual) override;
    virtual bool   hasOnePush(int feature, bool*   hasOnePush) override;
    virtual bool   setMode(   int feature, FeatureMode mode) override;
    virtual bool   getMode(   int feature, FeatureMode *mode) override;
    virtual bool   setOnePush(int feature) override;

    //IFrameGrabberImageRaw
    virtual bool getImage(yarp::sig::ImageOf<yarp::sig::PixelMono>& image) override;
    virtual int height() const override;
    virtual int width() const override;

private:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    //method
    inline bool initializeRealsenseDevice();
    inline bool setParams();

    bool        getImage(FlexImage& Frame, Stamp* timeStamp, rs2::frameset& sourceFrame);
    bool        getImage(depthImage& Frame, Stamp* timeStamp, const rs2::frameset& sourceFrame);
    bool        setIntrinsic(yarp::os::Property& intrinsic, const rs2_intrinsics& values);
    bool        setExtrinsicParam(yarp::sig::Matrix& extrinsic, const rs2_extrinsics& values);
    void        settingErrorMsg(const std::string& error, bool& ret);
    void        updateTransformations();
    bool        pipelineStartup();
    bool        pipelineShutdown();
    bool        pipelineRestart();
    bool        setFramerate(const int _fps);
    void        fallback();


    // realsense classes
    std::mutex   m_mutex;
    rs2::context m_ctx;
    rs2::config m_cfg;
    rs2::pipeline m_pipeline;
    rs2::device  m_device;
    std::vector<rs2::sensor> m_sensors;
    rs2::sensor* m_depth_sensor;
    rs2::sensor* m_color_sensor;
    rs2_intrinsics m_depth_intrin, m_color_intrin, m_infrared_intrin;
    rs2_extrinsics m_depth_to_color, m_color_to_depth;


    yarp::os::Stamp m_rgb_stamp;
    yarp::os::Stamp m_depth_stamp;
    std::string m_lastError;
    yarp::dev::RGBDSensorParamParser* m_paramParser;
    bool m_verbose;
    bool m_initialized;
    bool m_stereoMode;
    bool m_needAlignment;
    int m_fps;
    float m_scale;
    std::vector<cameraFeature_id_t> m_supportedFeatures;
#endif
};
#endif
