/*******************************************************************************
Copyright 2022
Steward Observatory Engineering & Technical Services, University of Arizona
This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/

///
/// @brief Template controller class inheriting from LFAST_Device
/// @author Kevin Gilliam
/// @date February 16th, 2023
/// @file device_controller.cpp
///

#include "laser_array_controller.h"
#include <Arduino.h>
#include <cstring>
#include <cmath>
#include <cinttypes>
#include <iostream>
#include <TerminalInterface.h>
#include <math_util.h>
#include "PFC_config.h"
#include "teensy41_device.h"
#include "TimerOne.h"

const unsigned int laserDiodePins[] = LASER_DIODE_PINS;
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Control Functions  //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace LFAST;

/// @brief Any code which leverages hardware on the Teensy (such as timers, interrupts, etc)
void LaserArrayController::hardware_setup()
{
    for(int ii = 0; ii < sizeof(laserDiodePins)/sizeof(unsigned int); ii++)
    {
        pinMode(laserDiodePins[ii], OUTPUT);
        digitalWrite(laserDiodePins[ii], LOW);
    }
}

/// @brief Returns a reference to the singleton instantiation of this class
///
/// The first time this is called, the static object is created and calls
/// the device's constructor. Because the static keyword is used the object
/// remains in memory, and every time the function is called after that a 
/// reference to that object is returned.
///
/// @return A reference to the singleton instantiation of this class
LaserArrayController &LaserArrayController::getLaserController()
{
    static LaserArrayController instance;
    return instance;
}

/// @brief Creates persistent field labels for the terminal interface.
void LaserArrayController::setupPersistentFields()
{
    if (cli == nullptr)
        return;

    cli->addPersistentField(DeviceName, "[Hex Info (interrupt)]", DIODE_STATE_ROW);
}

/// @brief Function to be called when a callback is received over TCP.
void LaserArrayController::setLasterState(unsigned int onOff)
{
    for(int ii = 0; ii < sizeof(laserDiodePins)/sizeof(unsigned int); ii++)
    {
        digitalWrite(laserDiodePins[ii], onOff);
    }
    cli->updatePersistentField(DeviceName, DIODE_STATE_ROW, onOff, "%d");
}