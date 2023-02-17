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
/// @brief Template main file for an LFAST Controller device
///
/// Template program which uses all the important functionalities
/// contained in the LFAST_Device repository.
/// Intended to be run on a Teensy4.1, untested elsewhere.
///
/// @author Kevin Gilliam
/// @date February 16th, 2023
/// @file main.cpp
///


#include <Arduino.h>
// #include <TeensyThreads.h>

#include <cmath>
#include <math.h>
#include <string>

#include <TcpCommsService.h>
#include <TerminalInterface.h>
#include <teensy41_device.h>

#include "device_config.h"
#include "device_controller.h"

/// @brief Pointers to the two LFAST_Device objects being used here
LFAST::TcpCommsService *commsService;
TerminalInterface *cli;

/// @brief Pointer to the controller which is specific to this application.
DeviceController *pDC;


///////////////////////////////////////////////////////////////////////////
/// The LFAST Comms library (of which TcpCommsService is a component)
/// works by associating JSON key-value pairs to function pointers.
/// When a key is received, the library checks to see if a function 
/// pointers has been registered for it. If it has, it calls that function
/// and passes the value from the key-value pair as an argument.
/// This template defines and registers two such callbacks in this file.
///////////////////////////////////////////////////////////////////////////
void handshake(unsigned int val);
void otherCallback(double some_value);

/// @brief variables for the TCP configuration
byte myIP[] IP_ADDR;
unsigned int myPort = PORT;

/// @brief Function is called by the Arduino framework before the main loop starts
void setup()
{
  // The terminal interface uses a teensy serial port to display data and report
  // messages in an organized way. 
  cli = new TerminalInterface(DEVICE_CLI_LABEL, &(TEST_SERIAL), TEST_SERIAL_BAUD);
  commsService = new LFAST::TcpCommsService(myIP);
  // Any classes derived from LFAST_Device (such as TcpCommsService can print
  // data out to the same terminal interface, they don't all need their own.
  // They just have to be given a pointer to the terminal object:
  commsService->connectTerminalInterface(cli, "Comms");
  // It is helpful to connect the terminal before calling the TcpCommsService's
  // Initialization function so that any error messages can be printed out.
  commsService->initializeEnetIface(PORT);

  // The DeviceController class is a singleton, (meaning only one can exist), so 
  // instead of creating one with the new keyword, we use its getDeviceController
  // function.
  DeviceController &dc = DeviceController::getDeviceController();
  pDC = &dc;
  pDC->connectTerminalInterface(cli, "Device");

  // The terminal's persistent fields are set up to print out values which update 
  // frequently to the same position in the console window, rather than printing out
  // an endlessly scrolling list of values.
  cli->printPersistentFieldLabels();

  // The CommsService library will tell you if there was a problem starting up.
  if (!commsService->Status())
  {
    cli->printDebugMessage("Device Setup Failed.");
    while (true)
    {
      ;
    }
  }

  // Registering the message handlers as described above.
  commsService->registerMessageHandler<unsigned int>("Handshake", handshake);
  commsService->registerMessageHandler<double>("Other_Callback", otherCallback);

  delay(500);

  // The watchdog timer resets the teensy if it gets stuck in a state which
  // prevents the loop from running
  configureWatchdog();
  // The terminal interface prints out debug messages in a typical console
  // scroll beneath the persistent fields.
  cli->printDebugMessage("Initialization complete");
  // cli->printDebugMessage(DEBUG_CODE_ID_STR);

  // The ARM controller on the Teensy will tell you if something in the code caused
  // The application to crash if you ask it to. The while loop is there to keep
  // the message up and give the user a chance to fix the problem before it power
  // cycles due to another crash.
  bool crashOccurred = checkForCrashReport();
  while (crashOccurred)
  {
    ;
  }
}

/// @brief Function is called in an infinite loop by the Arduino framework after setup() has completed.
void loop()
{
#if WATCHDOG_ENABLED
  feedWatchDog();
#endif

  commsService->checkForNewClients();
  if (commsService->checkForNewClientData())
  {
    commsService->processClientData("DeviceFilterStr");
  }
  commsService->stopDisconnectedClients();

  // Loop code for updating the controller device
  pDC->doNonInterruptStuff();
}

/// @brief Handshake function to confirm connection
/// @param val The value that was sent with the handshake key
void handshake(unsigned int val)
{
  if (val == 0xDEAD)
  {
    LFAST::CommsMessage newMsg;
    newMsg.addKeyValuePair<unsigned int>("Handshake", 0xBEEF);
    commsService->sendMessage(newMsg, LFAST::CommsService::ACTIVE_CONNECTION);
    cli->printDebugMessage("Connected to client, starting control ISR.");
    pDC->enableControlInterrupt();
  }
  return;
}

/// @brief  An example of another callback function with a different value type
/// @param some_value 
void otherCallback(double some_value)
{
  // Disable interrupts if this callback uses shared data
  noInterrupts();
  pDC->doSomethingForACallback();
  // Re-enable Interrupts
  interrupts();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////