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
/// @brief Macros for easy device configuration changes
/// @author Kevin Gilliam
/// @date February 16th, 2023
/// @file device_config.h
///

#ifndef DEVICE_CONFIG_H_H
#define DEVICE_CONFIG_H_H

#define LED_PIN 13

// Laser diode controller pins
#define LASER_DIODE_PINS {10, 11, 12, 13}

// ADC Motor Pins
#define ADC_MTR_POS_PIN 24
#define ADC_MTR_NEG_PIN 22
#define ADC_POSN_WIPER_PIN A0
#define ADC_POSN_WIPER_SCALE 1.0 // need to work this out and update

// Voicecoil controller interface pins (Serial 4)
#define VC_CTRL_IFACE_TX_PIN 17
#define VC_CTRL_IFACE_RX_PIN 16

//Determine Network values
#define MAC { 0x00, 0x50, 0xB6, 0xEA, 0x8F, 0x44 }
// #define IP_ADDR   { 169,254,232,24 }
//#define MAC     { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }
#define IP_ADDR   {192, 168, 121, 177}
#define GATEWAY 0,0,0,0
#define SUBNET  0,0,0,0
#define PORT    4500

#define UPDATE_PRD_US 100 
#define TERM_UPDATE_PRD_SEC 0.2


#define ENABLE_TERMINAL_UPDATES 1

#endif