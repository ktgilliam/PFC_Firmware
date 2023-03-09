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

#include "adc_controller.h"
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Control Functions  //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace LFAST;

/// @brief Returns a reference to the singleton instantiation of this class
///
/// The first time this is called, the static object is created and calls
/// the device's constructor. Because the static keyword is used the object
/// remains in memory, and every time the function is called after that a 
/// reference to that object is returned.
///
/// @return A reference to the singleton instantiation of this class
ADCController &ADCController::getAdcController()
{
    static ADCController instance;
    return instance;
}

/// @brief Any code which leverages hardware on the Teensy (such as timers, interrupts, etc)
void ADCController::hardware_setup()
{
 pinMode(ADC_MTR_POS_PIN, OUTPUT);
 pinMode(ADC_MTR_NEG_PIN, OUTPUT);
}

/// @brief Stuff that happens outside the interrupt part of the device controller code.
void ADCController::doNonInterruptStuff()
{
    static uint64_t bg_loop_ct = 0;
    // cli->updatePersistentField(DeviceName, BG_LOOP_INFO_ROW, bg_loop_ct++, "%d");
}

/// @brief Creates persistent field labels for the terminal interface.
void ADCController::setupPersistentFields()
{
    if (cli == nullptr)
        return;

    // cli->addPersistentField(DeviceName, "[Hex Info (interrupt)]", HEX_INFO_ROW);
    // cli->addPersistentField(DeviceName, "[Float Info (interrupt)]", FLOAT_INFO_ROW);
    // cli->addPersistentField(DeviceName, "[Dec Info (interrupt)]", DEC_INFO);
    // cli->addPersistentField(DeviceName, "[Callback Counter]", CALLBACK_INFO_ROW);
    // cli->addPersistentField(DeviceName, "[BG Loop Counter]", BG_LOOP_INFO_ROW);
}

/// @brief Function to be called when a callback is received over TCP.
void ADCController::setPosition(double position)
{
    //  double posn = getCurrentPosition();
    targetPosition = position;
    // cli->updatePersistentField(DeviceName, CALLBACK_INFO_ROW, callback_ct++, "%d");
}

double ADCController::getCurrentPosition()
{
    double posn = analogRead(ADC_POSN_WIPER_PIN) * ADC_POSN_WIPER_SCALE;
    return posn;
}