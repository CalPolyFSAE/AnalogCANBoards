//******************************************************************************
//! @file $RCSfile: config.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief Configuration file for the following project:
//!             - can_sensor_node_example_gcc
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

#ifndef _CONFIG_H_
#define _CONFIG_H_

//_____ I N C L U D E S ________________________________________________________
#include "compiler.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "at90can_drv.h"

//_____ M A C R O S ____________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________

    // -------------- MCU LIB CONFIGURATION
#define FOSC           16000        // 8 MHz External cristal

    // -------------- CAN LIB CONFIGURATION
#define CAN_BAUDRATE   1000        // in kBit
//#define CAN_BAUDRATE   CAN_AUTOBAUD

    // -------------- MISCELLANEOUS
    // Using TIMER_2 as RTC
//#define USE_TIMER8       TIMER8_2
//#define RTC_TIMER        2      // See "board.h"
//#define RTC_CLOCK        32     // See "board.h"

//_____ D E C L A R A T I O N S ________________________________________________

#endif  // _CONFIG_H_


