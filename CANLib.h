/*
 * CANLib.h
 *
 *  Created on: Feb 10, 2018
 *      Author: root
 */

#ifndef CANLIB_H_
#define CANLIB_H_

#include "stdint.h"


/////////////////////////////////// Begin section from can_lib.h///////////////
//******************************************************************************
//! @file $RCSfile: can_lib.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the prototypes and the macros of the
//!        library of functions of:
//!             - CAN (Controller Array Network)
//!             - for AT90CAN128/64/32.
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

    //_____ D E F I N I T I O N S __________________________________________________

#define FOSC           (F_CPU / 1000)
#define CAN_AUTOBAUD    0

#define CAN_PORT_IN     PIND
#define CAN_PORT_DIR    DDRD
#define CAN_PORT_OUT    PORTD
#define CAN_INPUT_PIN   6
#define CAN_OUTPUT_PIN  5
    // ----------
#define ERR_GEN_MSK ((1<<SERG)|(1<<CERG)|(1<<FERG)|(1<<AERG))            //! MaSK for GENeral ERRors INTerrupts
#define INT_GEN_MSK ((1<<BOFFIT)|(1<<BXOK)|(ERR_GEN_MSK))                //! MaSK for GENeral INTerrupts

#define BRP_MSK     ((1<<BRP5)|(1<<BRP4)|(1<<BRP3)|(1<<BRP2)|(1<<BRP1)|(1<<BRP0))  //! Mask for BRP in CANBT1
#define SJW_MSK     ((1<<SJW1)|(1<<SJW0))                                //! MaSK for SJW  in CANBT2
#define PRS_MSK     ((1<<PRS2)|(1<<PRS1)|(1<<PRS0))                      //! MaSK for PRS  in CANBT2
#define PHS2_MSK    ((1<<PHS22)|(1<<PHS21)|(1<<PHS20))                   //! MaSK for PHS2 in CANBT2
#define PHS1_MSK    ((1<<PHS12)|(1<<PHS11)|(1<<PHS10))                   //! MaSK for PHS1 in CANBT2
#define BRP         (BRP0)                                               //! BRP  in CANBT1
#define SJW         (SJW0)                                               //! SJW  in CANBT2
#define PRS         (PRS0)                                               //! PRS  in CANBT2
#define PHS2        (PHS20)                                              //! PHS2 in CANBT2
#define PHS1        (PHS10)                                              //! PHS1 in CANBT2

#define HPMOB_MSK   ((1<<HPMOB3)|(1<<HPMOB2)|(1<<HPMOB1)|(1<<HPMOB0))    //! MaSK for MOb in HPMOB
#define MOBNB_MSK   ((1<<MOBNB3)|(1<<MOBNB2)|(1<<MOBNB1)|(1<<MOBNB0))    //! MaSK for MOb in CANPAGE

#define ERR_MOB_MSK ((1<<BERR)|(1<<SERR)|(1<<CERR)|(1<<FERR)|(1<<AERR))  //! MaSK for MOb ERRors
#define INT_MOB_MSK ((1<<TXOK)|(1<<RXOK)|(1<<BERR)|(1<<SERR)|(1<<CERR)|(1<<FERR)|(1<<AERR)) //! MaSK for MOb INTerrupts

#define CONMOB_MSK  ((1<<CONMOB1)|(1<<CONMOB0))                          //! MaSK for CONfiguration MOb
#define DLC_MSK     ((1<<DLC3)|(1<<DLC2)|(1<<DLC1)|(1<<DLC0))            //! MaSK for Data Length Coding
#define CONMOB      (CONMOB0)                                            //! CONfiguration MOb
#define DLC         (DLC0)                                               //! Data Length Coding
    // ----------
#define NB_MOB       15
#define NB_DATA_MAX  8
#define LAST_MOB_NB  (NB_MOB-1)
#define NO_MOB       0xFF
    // ----------
#define STATUS_CLEARED            0x00
    // ----------
#define MOB_NOT_COMPLETED         0x00                                              // 0x00
#define MOB_TX_COMPLETED        (1<<TXOK)                                           // 0x40
#define MOB_RX_COMPLETED        (1<<RXOK)                                           // 0x20
#define MOB_RX_COMPLETED_DLCW  ((1<<RXOK)|(1<<DLCW))                                // 0xA0
#define MOB_ACK_ERROR           (1<<AERR)                                           // 0x01
#define MOB_FORM_ERROR          (1<<FERR)                                           // 0x02
#define MOB_CRC_ERROR           (1<<CERR)                                           // 0x04
#define MOB_STUFF_ERROR         (1<<SERR)                                           // 0x08
#define MOB_BIT_ERROR           (1<<BERR)                                           // 0x10
#define MOB_PENDING            ((1<<RXOK)|(1<<TXOK))                                // 0x60
#define MOB_NOT_REACHED        ((1<<AERR)|(1<<FERR)|(1<<CERR)|(1<<SERR)|(1<<BERR))  // 0x1F
#define MOB_DISABLE               0xFF                                              // 0xFF
    // ----------
#define MOB_Tx_ENA  1
#define MOB_Rx_ENA  2
#define MOB_Rx_BENA 3
    // ----------

//_____ M A C R O S ____________________________________________________________

    // ----------
#define Can_reset()       ( CANGCON  =  (1<<SWRES) )
#define Can_enable()      ( CANGCON |=  (1<<ENASTB))
#define Can_disable()     ( CANGCON &= ~(1<<ENASTB))
    // ----------
#define Can_full_abort()  { CANGCON |=  (1<<ABRQ); CANGCON &= ~(1<<ABRQ); }
    // ----------
//#define Can_conf_bt()     { CANBT1=CONF_CANBT1; CANBT2=CONF_CANBT2; CANBT3=CONF_CANBT3; }
    // ----------
#define Can_set_mob(mob)       { CANPAGE = ((mob) << 4);}
#define Can_set_mask_mob()     {  CANIDM4=0xFF; CANIDM3=0xFF; CANIDM2=0xFF; CANIDM1=0xFF; }
#define Can_clear_mask_mob()   {  CANIDM4=0x00; CANIDM3=0x00; CANIDM2=0x00; CANIDM1=0x00; }
#define Can_clear_status_mob() { CANSTMOB=0x00; }
#define Can_clear_mob()        { unsigned char  volatile *__i_; for (__i_=&CANSTMOB; __i_<&CANSTML; __i_++) { *__i_=0x00 ;}}
    // ----------
#define Can_mob_abort()   ( DISABLE_MOB )
    // ----------
#define Can_set_dlc(dlc)  ( CANCDMOB |= (dlc)        )
#define Can_set_ide()     ( CANCDMOB |= (1<<IDE)     )
#define Can_set_rtr()     ( CANIDT4  |= (1<<RTRTAG)  )
#define Can_set_rplv()    ( CANCDMOB |= (1<<RPLV)    )
    // ----------
#define Can_clear_dlc()   ( CANCDMOB &= ~DLC_MSK     )
#define Can_clear_ide()   ( CANCDMOB &= ~(1<<IDE)    )
#define Can_clear_rtr()   ( CANIDT4  &= ~(1<<RTRTAG) )
#define Can_clear_rplv()  ( CANCDMOB &= ~(1<<RPLV)   )
    // ----------
#define DISABLE_MOB       ( CANCDMOB &= (~CONMOB_MSK) )
#define Can_config_tx()        { DISABLE_MOB; CANCDMOB |= (MOB_Tx_ENA  << CONMOB); }
#define Can_config_rx()        { DISABLE_MOB; CANCDMOB |= (MOB_Rx_ENA  << CONMOB); }
#define Can_config_rx_buffer() {              CANCDMOB |= (MOB_Rx_BENA << CONMOB); }
    // ----------
#define Can_get_dlc()      ((CANCDMOB &  DLC_MSK)     >> DLC   )
#define Can_get_ide()      ((CANCDMOB &  (1<<IDE))    >> IDE   )
#define Can_get_rtr()      ((CANIDT4  &  (1<<RTRTAG)) >> RTRTAG)
    // ----------
#define Can_set_rtrmsk()   ( CANIDM4 |= (1<<RTRMSK) )
#define Can_set_idemsk()   ( CANIDM4 |= (1<<IDEMSK) )
    // ----------
#define Can_clear_rtrmsk() ( CANIDM4 &= ~(1<<RTRMSK) )
#define Can_clear_idemsk() ( CANIDM4 &= ~(1<<IDEMSK) )
    // ----------
                //!< STD ID TAG Reading
#define Can_get_std_id(identifier)  { *((unsigned char *)(&(identifier))+1) =  CANIDT1>>5              ; \
                                      *((unsigned char *)(&(identifier))  ) = (CANIDT2>>5)+(CANIDT1<<3); }
    // ----------
                //!< EXT ID TAG Reading
#define Can_get_ext_id(identifier)  { *((unsigned char *)(&(identifier))+3) =  CANIDT1>>3              ; \
                                      *((unsigned char *)(&(identifier))+2) = (CANIDT2>>3)+(CANIDT1<<5); \
                                      *((unsigned char *)(&(identifier))+1) = (CANIDT3>>3)+(CANIDT2<<5); \
                                      *((unsigned char *)(&(identifier))  ) = (CANIDT4>>3)+(CANIDT3<<5); }
    // ----------
                //!< STD ID Construction
#define CAN_SET_STD_ID_10_4(identifier)  (((*((unsigned char *)(&(identifier))+1))<<5)+((* (unsigned char *)(&(identifier)))>>3))
#define CAN_SET_STD_ID_3_0( identifier)  (( * (unsigned char *)(&(identifier))   )<<5)
    // ----------
                //!< STD ID TAG writing
#define Can_set_std_id(identifier)  { CANIDT1   = CAN_SET_STD_ID_10_4(identifier); \
                                      CANIDT2   = CAN_SET_STD_ID_3_0( identifier); \
                                      CANCDMOB &= (~(1<<IDE))                    ; }
    // ----------
                //!< STD ID MASK writing
#define Can_set_std_msk(mask)       { CANIDM1   = CAN_SET_STD_ID_10_4(mask); \
                                      CANIDM2   = CAN_SET_STD_ID_3_0( mask); }
    // ----------
                //!< EXT ID Construction
#define CAN_SET_EXT_ID_28_21(identifier)  (((*((unsigned char *)(&(identifier))+3))<<3)+((*((unsigned char *)(&(identifier))+2))>>5))
#define CAN_SET_EXT_ID_20_13(identifier)  (((*((unsigned char *)(&(identifier))+2))<<3)+((*((unsigned char *)(&(identifier))+1))>>5))
#define CAN_SET_EXT_ID_12_5( identifier)  (((*((unsigned char *)(&(identifier))+1))<<3)+((* (unsigned char *)(&(identifier))   )>>5))
#define CAN_SET_EXT_ID_4_0(  identifier)   ((* (unsigned char *)(&(identifier))   )<<3)
    // ----------
                //!< EXT ID TAG writing
#define Can_set_ext_id(identifier)  { CANIDT1   = CAN_SET_EXT_ID_28_21(identifier); \
                                      CANIDT2   = CAN_SET_EXT_ID_20_13(identifier); \
                                      CANIDT3   = CAN_SET_EXT_ID_12_5( identifier); \
                                      CANIDT4   = CAN_SET_EXT_ID_4_0(  identifier); \
                                      CANCDMOB |= (1<<IDE);                         }
    // ----------
                //!< EXT ID MASK writing
#define Can_set_ext_msk(mask)       { CANIDM1   = CAN_SET_EXT_ID_28_21(mask); \
                                      CANIDM2   = CAN_SET_EXT_ID_20_13(mask); \
                                      CANIDM3   = CAN_SET_EXT_ID_12_5( mask); \
                                      CANIDM4   = CAN_SET_EXT_ID_4_0(  mask); }
    // ----------
/////////////////////////////////// End section from can_lib.h///////////////


//listener interface for CAN rx callback
class CANListener
{
public:
    virtual ~CANListener();
    virtual void INT_Call_GotFrame(const struct CAN_FRAME& frame) {};
    virtual void INT_Call_SentFrame(const struct CAN_FRAME& frame) {};

};

// the initial handler of CAN interrupts
class CANRaw
{
private:
    CANRaw();

    //array of all the CAN rx data handlers
    const CANListener* handlers[NB_MOB];

public:

    enum class CAN_BAUDRATE : uint8_t
    {
        B1M,
        B500K,
        B250K,
        B125K
    };

    union CAN_DATA
    {
        uint64_t value;
        uint8_t byte[8];
    };

    struct CAN_FRAME
    {
        uint32_t id;
        uint8_t rtr;
        uint8_t extendedId;
        uint8_t dataLength;
        CAN_DATA data;
    };

    struct CAN_MOB_CONFIG
    {
        uint32_t id;
        uint32_t idMask;
        uint8_t rtr;
        bool rtrMask;
        uint8_t extendedId;
        bool extendedIdMask;
        uint8_t dataLength;
    };

    enum class CAN_MOB : uint8_t
    {
        MOB_0 = 0,
        MOB_1,
        MOB_2,
        MOB_3,
        MOB_4,
        MOB_5,
        MOB_6,
        MOB_7,
        MOB_8,
        MOB_9,
        MOB_10,
        MOB_11,
        MOB_12,
        MOB_13,
        MOB_14
    };

    static inline CANRaw& StaticClass()
    {
        static CANRaw CAN {};
        return CAN;
    }

    // reinitialize the can hardware
    void Init(CAN_BAUDRATE baud);

    // binds a CANListener to a mob
    bool BindListener(const CANListener* listener, CAN_MOB Mob);

    inline void INT_CANIT() {
        uint8_t origCANPAGE = CANPAGE;
        while (CANSIT2 | CANSIT1)
        {

            CAN_MOB mobn;

            if (CANSIT2 & _BV (0)) mobn = CAN_MOB::MOB_0;
            else if (CANSIT2 & _BV (1)) mobn = CAN_MOB::MOB_1;
            else if (CANSIT2 & _BV (2)) mobn = CAN_MOB::MOB_2;
            else if (CANSIT2 & _BV (3)) mobn = CAN_MOB::MOB_3;
            else if (CANSIT2 & _BV (4)) mobn = CAN_MOB::MOB_4;
            else if (CANSIT2 & _BV (5)) mobn = CAN_MOB::MOB_5;
            else if (CANSIT2 & _BV (6)) mobn = CAN_MOB::MOB_6;
            else if (CANSIT2 & _BV (7)) mobn = CAN_MOB::MOB_7;
            else if (CANSIT1 & _BV (0)) mobn = CAN_MOB::MOB_8;
            else if (CANSIT1 & _BV (1)) mobn = CAN_MOB::MOB_9;
            else if (CANSIT1 & _BV (2)) mobn = CAN_MOB::MOB_10;
            else if (CANSIT1 & _BV (3)) mobn = CAN_MOB::MOB_11;
            else if (CANSIT1 & _BV (4)) mobn = CAN_MOB::MOB_12;
            else if (CANSIT1 & _BV (5)) mobn = CAN_MOB::MOB_13;
            else if (CANSIT1 & _BV (6)) mobn = CAN_MOB::MOB_14;

            Can_set_mob(mobn)

            if (CANSTMOB & (1 << RXOK))
            {
                CANSTMOB = 0x00; // clears interrupt flags
                CPFECANLib::rxInt ();
            }

            /*
             // SLOWER OPTION?
             uint16_t CANSIT = CANSIT2 | (CANSIT1 << 8);
             for(uint8_t mob; mob < NB_MOB; ++mob)
             {
             if(CANSIT & _BV(mob))
             {
             Can_set_mob(mob);
             if(CANSTMOB & MOB_RX_COMPLETED)
             {
             Can_clear_status_mob();
             //handlers[mob]->INT_Call_GotFrame(...);
             } else if (CANSTMOB & MOB_TX_COMPLETED)
             {
             Can_clear_status_mob();
             //handlers
             }

             }
             }*/
        }
        CANPAGE = origCANPAGE;
    }
};


#endif /* CANLIB_H_ */
