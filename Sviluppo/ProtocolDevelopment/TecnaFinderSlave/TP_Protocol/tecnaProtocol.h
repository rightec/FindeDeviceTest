#ifndef TECNAPROTOCOL_H
#define TECNAPROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include <stdio.h>
#include "tecnaProtocolDef.h"


/**
 * @brief prepareCmd: Prepare the command send
 * @param _cmdId: Id of the message to send
 * @param _cmdDestMac: Mac address of the destination device
 * @param _cmdStruct: Message data to be incapsulated
 * @param _cmdPayload: Return data to send
 * @return 0 if it fails - Len of the frame if it is OK
 */
uint8_t TP_prepareCmd(uint8_t _cmdId,
                   uint8_t *_cmdDestMac,
                   uint8_t *_cmdStruct,
                   uint8_t *_cmdPayload );


/**
 * @brief TP_prepareNack:Prepare the Nack message to send.
 * @param _cmdId: Nack to Command received
 * @param _cmdPayload
 * @return
 */
int TP_prepareNack(uint8_t _cmdId,
                   uint8_t *_cmdDestMac,
                   uint8_t *_cmdPayload );


/**
 * @brief TP_initProtocol: Initialize protocol
 * @param _cmdRole: If it is master or slave
 * @param _cmdSrcMac: Set the Source MAC address
 * @param _type: type of protocol: HMS or TECNA
 * @return
 */
uint8_t TP_initProtocol(uint8_t _cmdRole,
                     uint8_t *_cmdSrcMac,
                     tecProtType_t _type);

/**
 * @brief TP_SetType
 * @param _type
 * @return
 */
uint8_t TP_SetType(tecProtType_t _type);

/**
 * @brief TP_AddSlaveToList
 * @param _slaveDevice
 * @return: -1 if adding to slave list fails otherwise the update index in the list
 */
int TP_AddSlaveToList(tecProtocolDeviceInfo_t _slaveDevice);

/**
 * @brief TP_AddSlaveDeviceInfo
 * @param _slaveDeviceSetInfo
 * @param _SrcMacAddr
 * @return -1 if updating the slave list fails
 */
int TP_AddSlaveDeviceInfo(tecProtocolSPresetFrame_t _slaveDeviceSetInfo, uint8_t *_SrcMacAddr);


/**
 * @brief TP_ParseMessage: Parse the message receivied form udp stack
 * @param _udpMessage
 * @param _size
 * @param _sender: IP sender
 * @return -1 in case of input param error or one of tecProtErr_t
 */
int  TP_ParseMessage(uint8_t *_udpMessage, uint8_t _size, uint8_t *_sender);

/**
 * @brief TP_SetAnnouncedMaster: Function used from the slave
 * when receiving the announce message
 * @param _ipAddress: Store the Master ip Adddress
 * @return -1 if the input parameter is null
 */
int TP_SetAnnouncedMaster(uint8_t *_ipAddress);

/**
 * @brief TP_GetNumberOfItemInList
 * @return the number of item in the list
 */
int TP_GetNumberOfItemInList();


/**
 * @brief TP_GetHostName: Return the host name from the list at the given position
 * @param _hostName
 * @param _pos
 * @return -1 if fails or the len of the host name
 */
int TP_GetHostName(uint8_t * _hostName, int _pos);

/**
 * @brief TP_GetMacFromPosition
 * @param _pos: Position in the list
 * @param _macAddr: Mac address of the device
 * @return -1 if it fails.
 */
int TP_GetMacFromPosition(int _pos, uint8_t *_macAddr);

/**
 * @brief TP_GetIpAddrFromPosition
 * @param _pos: Position in the list
 * @param _ipAddr: ip address of the device
 * @return -1 if it fails.
 */
int TP_GetIpAddrFromPosition(int _pos, uint8_t *_ipAddr);

/**
 * @brief TP_GetGwAddrFromPosition
 * @param _pos: Position in the list
 * @param _gateway: gateway address of the device
 * @return -1 if it fails.
 */
int TP_GetGWAddrFromPosition(int _pos, uint8_t *_gateway);

/**
 * @brief TP_GetPriDNSFromPosition
 * @param _pos: Position in the list
 * @param _priDNS: Primary DNS address of the device
 * @return -1 if it fails.
 */
int TP_GetPriDNSFromPosition(int _pos, uint8_t *_priDNS);

/**
 * @brief TP_GetSecDNSFromPosition
 * @param _pos: Position in the list
 * @param _secDNS: Secondary DNS address of the device
 * @return -1 if it fails.
 */
int TP_GetSecDNSFromPosition(int _pos, uint8_t *_secDNS);


/**
 * @brief TP_GetNetMaskFromPosition
 * @param _pos: Position in the list
 * @param _netMask: netMask of the device
 * @return -1 if it fails.
 */
int TP_GetNetMaskFromPosition(int _pos, uint8_t *_netMask);


/**
 * @brief TP_GetDHCPFromPosition
 * @param _pos: Position in the list
 * @param _dhcp: dhcp state
 * @return -1 if it fails.
 */
int TP_GetDHCPFromPosition(int _pos, uint8_t *_dhcp);



/**
 * @brief TP_GetDevCodeFromPosition
 * @param _pos: Position in the list
 * @param _devCode: device code to operate set
 * @return -1 if it fails.
 */
int TP_GetDevCodeFromPosition(int _pos, uint8_t *_devCode);

/**
 * @brief TP_GetSlaveToSet: Slave receives parameter to set
 * @param *_settings: parameter received from master
 * @return -1 if it fails.
 */
int TP_GetSlaveToSet(uint8_t *_settings);

/**
 * @brief TP_ResetSlaveDevice: Delete any slave item from the slave list
 * @return: -1 if it fails.
 */
int TP_ResetSlaveDevice();

/**
 * @brief TP_SlaveConfig
 * @param _config
 * @return -1 if it fails
 */
int TP_SlaveConfig(tecProtocolSDiscoveryFrame_t *_config);


/**
 * @brief TP_SlaveIsMyMaster: Verify if the UDP sender is my master when we are slave
 * @param _sender
 * @return -1 if it fails. 0 if it is not my master. 1 If it is my master
 */
int TP_SlaveIsMyMaster(uint8_t *_sender);

/**
 * @brief TP_SlaveStoreMac: Copy the MAC address of the master in the relevant struct
 * @param _masterMAC
 * @return -1 if it fails
 */
int TP_SlaveStoreMac(uint8_t *_masterMAC);

#ifdef __cplusplus
}
#endif

#endif /* TECNAPROTOCOL_H */
