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

#include "device_controller.h"
#include <Arduino.h>
#include <cstring>
#include <cmath>
#include <cinttypes>
#include <iostream>
#include <TerminalInterface.h>
#include <math_util.h>
#include "device_config.h"
#include "teensy41_device.h"
#include "TimerOne.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Control Functions  //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace LFAST;

/// @brief Interrupt for an interrupt-driven controller
///
/// If the controller is interrupt-driven, the ISR should not be part of the controller's
/// class. It should use getDeviceController() which will return a reference to the same
/// object as is used in the main.cpp code, since the device controller is a singleton.
///
void primaryMirrorControl_ISR()
{
    noInterrupts();

    DeviceController &dc = DeviceController::getDeviceController();
    dc.doInterruptStuff();
    interrupts();
}

/// @brief Returns a reference to the singleton instantiation of this class
///
/// The first time this is called, the static object is created and calls
/// the device's constructor. Because the static keyword is used the object
/// remains in memory, and every time the function is called after that a 
/// reference to that object is returned.
///
/// @return A reference to the singleton instantiation of this class
DeviceController &DeviceController::getDeviceController()
{
    static DeviceController instance;
    return instance;
}

/// @brief Any code which leverages hardware on the Teensy (such as timers, interrupts, etc)
void DeviceController::hardware_setup()
{
    // Initialize Timer
    Timer1.initialize(UPDATE_PRD_US);
    Timer1.stop();
    Timer1.attachInterrupt(primaryMirrorControl_ISR);
}

void DeviceController::enableControlInterrupt()
{
    Timer1.start();
}

/// @brief Stuff that happens inside the interrupt part of the device controller code.
void DeviceController::doInterruptStuff()
{
    static uint32_t info_1_demo_val = 0;
    static double info_2_demo_val = 0;
    static int32_t info_3_demo_val = 0;

    info_1_demo_val++;
    info_2_demo_val += 0.5;
    info_3_demo_val -= 3;

#if ENABLE_TERMINAL_UPDATES
    cli->updatePersistentField(DeviceName, HEX_INFO_ROW, info_1_demo_val, "%#08x");
    cli->updatePersistentField(DeviceName, HEX_INFO_ROW, info_2_demo_val, "%0.4f");
    cli->updatePersistentField(DeviceName, HEX_INFO_ROW, info_3_demo_val, "%d");
#endif
}

/// @brief Stuff that happens outside the interrupt part of the device controller code.
void DeviceController::doNonInterruptStuff()
{
    static uint64_t bg_loop_ct = 0;
    cli->updatePersistentField(DeviceName, BG_LOOP_INFO_ROW, bg_loop_ct++, "%d");
}

/// @brief Creates persistent field labels for the terminal interface.
void DeviceController::setupPersistentFields()
{
    if (cli == nullptr)
        return;

    cli->addPersistentField(DeviceName, "[Hex Info (interrupt)]", HEX_INFO_ROW);
    cli->addPersistentField(DeviceName, "[Float Info (interrupt)]", FLOAT_INFO_ROW);
    cli->addPersistentField(DeviceName, "[Dec Info (interrupt)]", DEC_INFO);
    cli->addPersistentField(DeviceName, "[Callback Counter]", CALLBACK_INFO_ROW);
    cli->addPersistentField(DeviceName, "[BG Loop Counter]", BG_LOOP_INFO_ROW);
}

/// @brief Function to be called when a callback is received over TCP.
void DeviceController::doSomethingForACallback()
{
    static uint64_t callback_ct = 0;
    cli->updatePersistentField(DeviceName, CALLBACK_INFO_ROW, callback_ct++, "%d");
}