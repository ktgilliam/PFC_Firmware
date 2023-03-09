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
/// @file device_controller.h
///


#ifndef LASER_ARRAY_CONTROLLER_H
#define LASER_ARRAY_CONTROLLER_H

#include <Arduino.h>
#include <LFAST_Device.h>
#include <TerminalInterface.h>
#include <cmath>
#include <algorithm>

#include <math_util.h>

#include "lfast_device.h"
#include "teensy41_device.h"

/// @brief  Use an enum to make it easy to switch the order that persistent fields are printed out.
enum LASER_CTRL_CLI_ROWS
{
    DIODE_STATE_ROW,
};

namespace LFAST
{
   
};

/// @brief Rename the LaserArrayController class when creating a new controller from this template.
class LaserArrayController : public LFAST_Device
{
public:
    static LaserArrayController &getLaserController();
    virtual ~LaserArrayController() {}
    void setupPersistentFields() override;

    void hardware_setup();
    void doNonInterruptStuff();

    void setLasterState(unsigned int onOff);

private:
    LaserArrayController() : LFAST_Device() {};

};

#endif
