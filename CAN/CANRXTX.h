/*
 * CANRXTX.h
 *
 *  Created on: Dec 9, 2017
 *      Author: root
 */

#ifndef CAN_CANRXTX_H_
#define CAN_CANRXTX_H_

#include <stdint.h>

class CANRXTX
{
public:

    // ----------
    // @brief This constant is used as return value for "can_cmd" function.
    enum class CAN_CMD_STATUS : uint8_t
    {
        REFUSED = 0xFF,
        ACCEPTED = 0x00
    };

    // ----------
    // @brief This constant is used as return value for "can_get_status" function.
    enum class CAN_MOB_STATUS : uint8_t
    {
        COMPLETED = 0x00,
        NOT_COMPLETED = 0x01,
        ERROR = 0x02
    };

    // ----------
    // @brief This enumeration is used to select an action for a specific message
    // declared in st_cmd_t structure.
    enum class CAN_CMD : uint8_t
    {
        CMD_NONE,
        CMD_TX,
        CMD_TX_DATA,
        CMD_TX_REMOTE,
        CMD_RX,
        CMD_RX_DATA,
        CMD_RX_REMOTE,
        CMD_RX_MASKED,
        CMD_RX_DATA_MASKED,
        CMD_RX_REMOTE_MASKED,
        CMD_REPLY,
        CMD_REPLY_MASKED,
        CMD_ABORT
    };

    // ----------
    // @brief This union defines a CAN identifier and allows to access it in mode
    // Standard, extended or thought a table.
    union CAN_ID
    {
        uint32_t ext;
        uint16_t std;
        uint8_t tab[4];
    };

    // ----------
    // @brief This structure defines some specific information as RTR bit and
    // IDE bit
    struct CAN_CTRL
    {
        bool rtr;
        bool ide;
    };

    // ----------
    // @brief This structure allows to define a specific action on CAN network.
    // 1) handle:  managed by the library.
    // 2) cmd   :  initialized by the application to select the operation.
    // 3) id    :  initialized by the application. Identifier for the CAN message
    // 4) dlc   :  initialized by the application to give the size of CAN data
    // 5) pt_data: pointer to memory with send or receive data
    // 6) status:  managed by the library. Status of MOB
    // 7) ctrl  :  CAN ide field for extended frame
    struct CMD
    {
        uint8_t handle;
        CAN_CMD cmd;
        CAN_ID id;
        uint8_t dlc;
        uint8_t* pt_data;
        uint8_t status;
        CAN_CTRL ctrl;
    };


    // ----------
    // @brief CAN Bus baud rate setting used for initialization
    enum class BAUD_RATE
        : uint8_t
        {
            B1M, B500K, B250K, B125K
    };


    static uint8_t Init(uint8_t mode);


private:
    CANRXTX();


};

#endif /* CAN_CANRXTX_H_ */
