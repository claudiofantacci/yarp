/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include <yarp/dev/ImplementControlBoardInterfaces.h>
#include <yarp/dev/ControlBoardHelper.h>

#include <cmath>

// Be careful: this file contains template implementations and is included by translation
// units that use the template (e.g. .cpp files). Avoid putting here non-template functions to
// avoid multiple definitions.

using namespace yarp::dev;

////////////////////////
// Encoder Interface Implementation
template <class DERIVED, class IMPLEMENT>
ImplementEncoders<DERIVED, IMPLEMENT>::ImplementEncoders(DERIVED *y)
{
    iEncoders= dynamic_cast<IEncodersRaw *> (y);
    helper = 0;
    temp=0;
}

template <class DERIVED, class IMPLEMENT>
ImplementEncoders<DERIVED, IMPLEMENT>::~ImplementEncoders()
{
    uninitialize();
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>:: initialize (int size, const int *amap, const double *enc, const double *zos)
{
    if (helper!=0)
        return false;

    helper=(void *)(new ControlBoardHelper(size, amap, enc, zos));
    yAssert (helper != 0);
    temp=new double [size];
    yAssert (temp != 0);
    return true;
}

/**
* Clean up internal data and memory.
* @return true if uninitialization is executed, false otherwise.
*/
template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::uninitialize ()
{
    if (helper!=0)
    {
        delete castToMapper(helper);
        helper=0;
    }

    checkAndDestroy(temp);

    return true;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::getAxes(int *ax)
{
    (*ax)=castToMapper(helper)->axes();
    return true;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::resetEncoder(int j)
{
    int k;
    k=castToMapper(helper)->toHw(j);

    return iEncoders->resetEncoderRaw(k);
}


template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::resetEncoders()
{
    return iEncoders->resetEncodersRaw();
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::setEncoder(int j, double val)
{
    int k;
    double enc;

    castToMapper(helper)->posA2E(val, j, enc, k);

    return iEncoders->setEncoderRaw(k, enc);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::setEncoders(const double *val)
{
    castToMapper(helper)->posA2E(val, temp);

    return iEncoders->setEncodersRaw(temp);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::getEncoder(int j, double *v)
{
    int k;
    double enc;
    bool ret;

    k=castToMapper(helper)->toHw(j);

    ret=iEncoders->getEncoderRaw(k, &enc);

    *v=castToMapper(helper)->posE2A(enc, k);

    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::getEncoders(double *v)
{
    bool ret;
    castToMapper(helper)->axes();

    ret=iEncoders->getEncodersRaw(temp);

    castToMapper(helper)->posE2A(temp, v);

    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::getEncoderSpeed(int j, double *v)
{
    int k;
    double enc;
    bool ret;

    k=castToMapper(helper)->toHw(j);

    ret=iEncoders->getEncoderSpeedRaw(k, &enc);

    *v=castToMapper(helper)->velE2A(enc, k);

    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::getEncoderSpeeds(double *v)
{
    bool ret;
    ret=iEncoders->getEncoderSpeedsRaw(temp);

    castToMapper(helper)->velE2A(temp, v);

    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::getEncoderAcceleration(int j, double *v)
{
    int k;
    double enc;
    bool ret;

    k=castToMapper(helper)->toHw(j);

    ret=iEncoders->getEncoderAccelerationRaw(k, &enc);

    *v=castToMapper(helper)->accE2A(enc, k);

    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementEncoders<DERIVED, IMPLEMENT>::getEncoderAccelerations(double *v)
{
    bool ret;
    ret=iEncoders->getEncoderAccelerationsRaw(temp);

    castToMapper(helper)->accE2A(temp, v);

    return ret;
}

////////////////////////
// ControlCalibration Interface Implementation
template <class DERIVED, class IMPLEMENT>
ImplementControlCalibration<DERIVED, IMPLEMENT>::ImplementControlCalibration(DERIVED *y)
{
    iCalibrate= dynamic_cast<IControlCalibrationRaw *> (y);
    helper = 0;
    temp=0;
}

template <class DERIVED, class IMPLEMENT>
ImplementControlCalibration<DERIVED, IMPLEMENT>::~ImplementControlCalibration()
{
    uninitialize();
}

template <class DERIVED, class IMPLEMENT>
bool ImplementControlCalibration<DERIVED, IMPLEMENT>:: initialize (int size, const int *amap, const double *enc, const double *zos)
{
    if (helper!=0)
        return false;

    helper=(void *)(new ControlBoardHelper(size, amap, enc, zos));
    yAssert (helper != 0);
    temp=new double [size];
    yAssert (temp != 0);
    return true;
}

/**
* Clean up internal data and memory.
* @return true if uninitialization is executed, false otherwise.
*/
template <class DERIVED, class IMPLEMENT>
bool ImplementControlCalibration<DERIVED, IMPLEMENT>::uninitialize ()
{
    if (helper!=0)
    {
        delete castToMapper(helper);
        helper=0;
    }

    checkAndDestroy(temp);

    return true;
}

#ifndef YARP_NO_DEPRECATED // Since YARP 3.0.0
template <class DERIVED, class IMPLEMENT>
bool ImplementControlCalibration<DERIVED, IMPLEMENT>::calibrate(int j, double p)
{
    int k=castToMapper(helper)->toHw(j);

    return iCalibrate->calibrateRaw(k, p);
}
#endif

template <class DERIVED, class IMPLEMENT>
bool ImplementControlCalibration<DERIVED, IMPLEMENT>::done(int j)
{
    int k=castToMapper(helper)->toHw(j);

    return iCalibrate->doneRaw(k);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementControlCalibration<DERIVED, IMPLEMENT>::calibrate(int axis, unsigned int type, double p1, double p2, double p3)
{
    int k=castToMapper(helper)->toHw(axis);

    return iCalibrate->calibrateRaw(k, type, p1, p2, p3);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementControlCalibration<DERIVED, IMPLEMENT>::setCalibrationParameters(int axis, const CalibrationParameters& params)
{
    int k = castToMapper(helper)->toHw(axis);

    return iCalibrate->setCalibrationParametersRaw(k, params);
}

//////////////////////////////
///////////////// Implement
template <class DERIVED, class IMPLEMENT>
ImplementAmplifierControl<DERIVED, IMPLEMENT>::ImplementAmplifierControl(DERIVED *y)
{
    iAmplifier= dynamic_cast<IAmplifierControlRaw *> (y);
    helper = 0;
    dTemp=0;
    iTemp=0;
}

template <class DERIVED, class IMPLEMENT>
ImplementAmplifierControl<DERIVED, IMPLEMENT>::~ImplementAmplifierControl()
{
    uninitialize();
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>:: initialize (int size, const int *amap, const double *enc, const double *zos, const double *ampereFactor, const double *voltFactor)
{
    if (helper!=0)
        return false;

    helper=(void *)(new ControlBoardHelper(size, amap, enc, zos,nullptr, ampereFactor, voltFactor));
    yAssert (helper != 0);
    dTemp=new double[size];
    yAssert (dTemp != 0);
    iTemp=new int[size];
    yAssert (iTemp != 0);

    return true;
}

/**
* Clean up internal data and memory.
* @return true if uninitialization is executed, false otherwise.
*/
template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::uninitialize ()
{
    if (helper!=0)
        delete castToMapper(helper);

    delete [] dTemp;
    delete [] iTemp;

    helper=0;
    dTemp=0;
    iTemp=0;
    return true;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::enableAmp(int j)
{
    int k=castToMapper(helper)->toHw(j);

    return iAmplifier->enableAmpRaw(k);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::disableAmp(int j)
{
    int k=castToMapper(helper)->toHw(j);

    return iAmplifier->disableAmpRaw(k);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getCurrents(double *currs)
{
    bool ret=iAmplifier->getCurrentsRaw(dTemp);
    castToMapper(helper)->ampereS2A(dTemp, currs);
    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getCurrent(int j, double *c)
{
    double temp = 0;
    int k = castToMapper(helper)->toHw(j);
    bool ret = iAmplifier->getCurrentRaw(k, &temp);
    castToMapper(helper)->ampereS2A(temp, k, *c, j);
    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getAmpStatus(int *st)
{
    bool ret=iAmplifier->getAmpStatusRaw(iTemp);
    castToMapper(helper)->toUser(iTemp, st);

    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getAmpStatus(int k, int *st)
{
    int j=castToMapper(helper)->toHw(k);
    bool ret=iAmplifier->getAmpStatusRaw(j, st);

    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::setMaxCurrent(int m, double v)
{
    int k;
    double curr;
    castToMapper(helper)->ampereA2S(v, m, curr, k);
    return iAmplifier->setMaxCurrentRaw(k, curr);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getMaxCurrent(int j, double* v)
{
    double val;
    int k=castToMapper(helper)->toHw(j);
    bool ret = iAmplifier->getMaxCurrentRaw(k, &val);
    *v = castToMapper(helper)->ampereS2A(val, k);
    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getNominalCurrent(int m, double *curr)
{
    int k;
    bool ret;
    double tmp;

    k=castToMapper(helper)->toHw(m);
    ret=iAmplifier->getNominalCurrentRaw(k, &tmp);
    *curr=castToMapper(helper)->ampereS2A(tmp, k);
    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getPeakCurrent(int m, double *curr)
{
    int k;
    bool ret;
    double tmp;

    k=castToMapper(helper)->toHw(m);
    ret=iAmplifier->getPeakCurrentRaw(k, &tmp);
    *curr=castToMapper(helper)->ampereS2A(tmp, k);
    return ret;
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::setPeakCurrent(int m, const double curr)
{
    int k;
    double val;
    castToMapper(helper)->ampereA2S(curr, m, val, k);
    return iAmplifier->setPeakCurrentRaw(k, val);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::setNominalCurrent(int m, const double curr)
{
    int k;
    double val;
    castToMapper(helper)->ampereA2S(curr, m, val, k);
    return iAmplifier->setNominalCurrentRaw(k, val);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getPWM(int m, double* pwm)
{
    int k;
    k=castToMapper(helper)->toHw(m);
    return iAmplifier->getPWMRaw(k, pwm);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getPWMLimit(int m, double* limit)
{
    int k;
    k=castToMapper(helper)->toHw(m);
    return iAmplifier->getPWMLimitRaw(k, limit);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::setPWMLimit(int m, const double limit)
{
    int k;
    k=castToMapper(helper)->toHw(m);
    return iAmplifier->setPWMLimitRaw(k, limit);
}

template <class DERIVED, class IMPLEMENT>
bool ImplementAmplifierControl<DERIVED, IMPLEMENT>::getPowerSupplyVoltage(int m, double *voltage)
{
    int k;
    k=castToMapper(helper)->toHw(m);
    return iAmplifier->getPowerSupplyVoltageRaw(k, voltage);
}
