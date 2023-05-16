#include "tecnaProtocol.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*** START VARIABLE SECTION ***/

const uint8_t cModuleScan[TEC_PROT_MOD_SCAN_LEN] =
                                {0x4d, 0x4F, 0x44, 0x55,
                                 0x4c,0x45,0x20,0x53,
                                 0x43,0x41,0x4e,0x00};

static uint8_t sDeviceRole;
/* Mac Address of the device wheter Master or Slave */
static uint8_t sSourceMacAddress[6];
static tecProtType_t sTypeOfProt;
static tecProtocolMSetFrame_t sMSetFrame;
static tecProtocolHeader_t sHeader;
static tecProtocolDeviceInfo_t sSlaveDiscovery[TEC_PROT_MAX_NUM_DEV_SUPP];
static uint8_t sSlaveDiscIndexList;
static uint8_t sReceivedUdp[TEC_PROT_MAX_UDP_DATA];

/*** Used only from the slave device
 *   sMasterIpAddress
*/

/* Mac Address of the destination. It is the master announcing itsself otherwise is the defualt value */
static uint8_t sMasterIpAddress[4] = {0xFF,0xFF,0xFF,0xFF};
static uint8_t sMasterMacAddress[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static tecProtocolSDiscoveryFrame_t sSlaveConfiguration;
static tecProtocolSDiscoveryFrame_t sSlaveConfToSet;
static tecProtocolSPresetFrame_t sSlaveWarned;
static uint8_t sMaxFieldNumThisDev = TEC_PROT_DATA_MAX_FIELD_NUM;
/*** END VARIABLE SECTION
*************************
***/



/*** START STATIC FUNCTION DECLARATION SECTION ***/

int  TP_copyToSetMaster(void*_src);
void TP_initMSetFramePayload();
void TP_initHeader();
int  TP_RetrieveIndexFromMac(uint8_t *_SrcMacAddr);
int  TP_isTheAnnMsg(uint8_t *_udpMessage);


/** Used in Slave device
 * */
void TP_initSSetFramePayload();
uint8_t TP_GetRandomCode();

/*** END STATIC FUNCTION DECLARATION SECTION
********************************************
***/



/*** START FUNCTION SECTION ***/

uint8_t TP_prepareCmd(uint8_t _cmdId,
                   uint8_t *_cmdDestMac,
                   uint8_t *_cmdStruct,
                   uint8_t *_cmdPayload )
{
    uint8_t l_retVal = 0;
    int i_len = -1;
    tecProtocolHeader_t *l_header = &sHeader;
    tecProtocolSPresetFrame_t *l_SlaveWarned = &sSlaveWarned;
    tecProtocolMSetDataFrame_t l_MasterDataSet;
    uint8_t l_u8i = 0;

    memset((void*)&l_MasterDataSet,0,sizeof(tecProtocolMSetDataFrame_t));
    memset((void*)l_SlaveWarned,0,sizeof(tecProtocolSPresetFrame_t));

    if ((_cmdDestMac == NULL) && (sDeviceRole == TEC_PROT_ROLE_SLAVE)){
        _cmdDestMac = &sMasterMacAddress[0];
    } //

    if (l_header != NULL){
        l_header->s_CmdId = _cmdId;
        l_header->s_Ack = TEC_PROT_SEND_ACK;


        if (_cmdId < TEC_PROT_CMD_MAX_ID){
            /* Request for a standard command */
            if (_cmdPayload != NULL){
                if (_cmdDestMac != NULL){
                    for (l_u8i = 0; l_u8i < 6; l_u8i++){
                       l_header->s_DestMacAddr[l_u8i] = *(_cmdDestMac+l_u8i);
                    }
                    switch (_cmdId){
                    case TEC_PROT_CMD_SCAN_ID:
                        if (sDeviceRole == TEC_PROT_ROLE_MASTER){
                            /** If Master the payload is null
                                So this is OK*/
                            /** FILL */
                            memcpy((void*)_cmdPayload,(void*)l_header,sizeof(tecProtocolHeader_t));
                            l_retVal = sizeof(tecProtocolHeader_t);
                        } else {
                            /** if Slave the payload is tecProtocolSDiscoveryFrame_t
                             *  */
                            l_header->s_DataLen = sizeof(tecProtocolSDiscoveryFrame_t) - TEC_PROT_DATA_HOSTNAME_LEN + sSlaveConfiguration.s_IpHostNameLEN;
                            if (_cmdDestMac == NULL){
                                for (l_u8i = 0; l_u8i < 6; l_u8i++){
                                   l_header->s_DestMacAddr[l_u8i] = sMasterMacAddress[l_u8i];
                                }
                            } // else ??

                            /** FILL */
                            memcpy((void*)_cmdPayload,(void*)l_header,sizeof(tecProtocolHeader_t));
                            memcpy((void*)&_cmdPayload[sizeof(tecProtocolHeader_t)],(void*)&sSlaveConfiguration,l_header->s_DataLen);
                            l_retVal = sizeof(tecProtocolHeader_t) + l_header->s_DataLen;
                        }
                        break;
                    case TEC_PROT_CMD_WARN_ID:
                        if (sDeviceRole == TEC_PROT_ROLE_MASTER){
                            /** If Master the payload is null
                                So this is OK*/
                            /** FILL */
                            memcpy((void*)_cmdPayload,(void*)l_header,sizeof(tecProtocolHeader_t));
                            l_retVal = sizeof(tecProtocolHeader_t);
                        } else {
                            /** if Slave the payload is tecProtocolSPresetFrame_t
                             *  */
                            for (l_u8i = 0; l_u8i < TEC_PROT_DEVICE_CODE_LEN; l_u8i++){
                                l_SlaveWarned->s_DeviceCode[l_u8i] = TP_GetRandomCode();
                            }
                            l_SlaveWarned->s_MaxDataField = sMaxFieldNumThisDev;
                            l_header->s_DataLen = sizeof(tecProtocolSPresetFrame_t);
                            /** FILL */
                            memcpy((void*)_cmdPayload,(void*)l_header,sizeof(tecProtocolHeader_t));
                            memcpy((void*)&_cmdPayload[sizeof(tecProtocolHeader_t)],(void*)l_SlaveWarned,sizeof(tecProtocolSPresetFrame_t));
                            l_retVal = sizeof(tecProtocolHeader_t) + sizeof(tecProtocolSPresetFrame_t);
                        }
                        break;
                    case TEC_PROT_CMD_SET_ID:
                        if (sDeviceRole == TEC_PROT_ROLE_MASTER){
                            /** If Master the payload is NOT null*/
                            if (_cmdStruct != NULL){
                                memcpy((void*)&l_MasterDataSet,(tecProtocolMSetDataFrame_t*)_cmdStruct,sizeof(tecProtocolMSetDataFrame_t));
                                i_len = TP_copyToSetMaster((void*)&l_MasterDataSet);
                                if (i_len != -1){
                                    l_header->s_DataLen = (sizeof(tecProtocolMSetFrame_t) - TEC_PROT_DATA_HOSTNAME_LEN + i_len);
                                    /** FILL */
                                    memcpy((void*)_cmdPayload,(void*)l_header,sizeof(tecProtocolHeader_t));
                                    memcpy((void*)&_cmdPayload[sizeof(tecProtocolHeader_t)],(void*)&sMSetFrame,sizeof(tecProtocolMSetFrame_t));
                                    l_retVal = sizeof(tecProtocolHeader_t) + (sizeof(tecProtocolMSetFrame_t) - TEC_PROT_DATA_HOSTNAME_LEN + i_len);
                                } else {
                                    /**  So this is error*/
                                    l_retVal = 0;
                                }
                            } else {
                                /**  So this is error*/
                                l_retVal = 0;
                            }

                        } else {
                            /** if Slave the payload is NULL
                                 **/
                            /** FILL */
                            memcpy((void*)_cmdPayload,(void*)l_header,sizeof(tecProtocolHeader_t));
                            l_retVal = sizeof(tecProtocolHeader_t);
                        }
                        break;
                    default:
                        l_retVal = 0;
                        break;
                    }
                } else {
                    /* Memory  error */
                    l_retVal = 0;
                }
            } else {
                /* Memory  error */
                l_retVal = 0;
            }
        } else {
            if (_cmdId == TEC_PROT_CMD_MAX_ID){
                /* Could be the announce message */
                if ((_cmdDestMac == NULL) &&
                    (_cmdStruct == NULL) &&
                     (_cmdPayload != NULL)){
                    /* It is the announce message */
                    memcpy((void*)_cmdPayload,(void*)&cModuleScan[0],TEC_PROT_MOD_SCAN_LEN);
                    l_retVal = TEC_PROT_MOD_SCAN_LEN;
                } else {
                    /* Cmd ID error*/
                    l_retVal = 0;
                }
            } else {
                /* Cmd ID error*/
                l_retVal = 0;
            }
        }
    } else {
        /* Something wrong with init*/
        l_retVal = 0;
    }


    return l_retVal;
}



int TP_prepareNack(uint8_t _cmdId,
                   uint8_t *_cmdDestMac,
                   uint8_t *_cmdPayload )
{
    int l_retVal = 0;
    tecProtocolHeader_t *l_header = &sHeader;
    uint8_t l_u8i = 0;

     if (_cmdId < TEC_PROT_CMD_MAX_ID){
        if (l_header != NULL){
            l_header->s_CmdId = _cmdId;
            l_header->s_Ack = TEC_PROT_SEND_NACK;
            l_header->s_Role = sDeviceRole;
            if (sDeviceRole == TEC_PROT_ROLE_SLAVE){
                for (l_u8i = 0; l_u8i < 6; l_u8i++){
                   l_header->s_DestMacAddr[l_u8i] = sMasterMacAddress[l_u8i];
                }
            } else {
                /** If master want to send NACK needs che Destination MAC
                    If iti is not the case it sends a broadcast message**/
                if (_cmdDestMac != NULL){
                    for (l_u8i = 0; l_u8i < 6; l_u8i++){
                       l_header->s_DestMacAddr[l_u8i] = _cmdDestMac[l_u8i];
                    } // end for
               } else {
                    for (l_u8i = 0; l_u8i < 6; l_u8i++){
                       l_header->s_DestMacAddr[l_u8i] = 0xFF;
                    } // end for
                }
            }
            l_header->s_DataLen = 0;

            /** FILL */
            memcpy((void*)_cmdPayload,(void*)l_header,sizeof(tecProtocolHeader_t));
            l_retVal = sizeof(tecProtocolHeader_t);
        } else {
            l_retVal = -1;
        }
     } else {
         l_retVal = -1;
     }

    return l_retVal;

}


uint8_t TP_initProtocol(uint8_t _cmdRole,
                     uint8_t *_cmdSrcMac,
                     tecProtType_t _type)
{
    uint8_t l_retVal = 0;
    uint8_t l_u8i = 0;

    if (_cmdSrcMac != NULL){
        sDeviceRole = _cmdRole;
        for (l_u8i = 0; l_u8i < 6; l_u8i++){
           sSourceMacAddress[l_u8i] = *(_cmdSrcMac+l_u8i);
        }

        sTypeOfProt = _type;

        TP_initHeader();
        TP_initMSetFramePayload();

        l_retVal = 1;

    } else {
        l_retVal = 0;
    }


    return l_retVal;
}

uint8_t TP_SetType(tecProtType_t _type)
{

    sTypeOfProt = _type;

    TP_initHeader();
    return 0;

}

int TP_ParseMessage(uint8_t *_udpMessage, uint8_t _size, uint8_t *_sender)
{
    int l_ret = -1;
    uint8_t l_bProcessEnable = 0;
    int l_pos = 0;
    int l_nameLen = 0;
    int l_isMyMaster = -1;

    memset ((void*)&sReceivedUdp[0],0, TEC_PROT_MAX_UDP_DATA);
    memcpy ((void*)&sReceivedUdp[0],(void*)_udpMessage, _size);

    if ( (sReceivedUdp[0] == TEC_PROT_CHAR_ID_MSB ) &&
         ( (sReceivedUdp[1] == TEC_PROT_CHAR_ID_LSB_HMS) ||
           (sReceivedUdp[1] == TEC_PROT_CHAR_ID_LSB_TECNA)) )  {
        // This is our protocol
        if (sReceivedUdp[TEC_PROT_ROLE_POS] == sDeviceRole){
            // It is a message we dont want to process
            l_bProcessEnable = 0;
            l_ret = TEC_PROT_MSG_NOT_PARSED;
        } else {
            if ((sReceivedUdp[TEC_PROT_ROLE_POS] == TEC_PROT_ROLE_SLAVE) ||
                (sReceivedUdp[TEC_PROT_ROLE_POS] == TEC_PROT_ROLE_MASTER)){
                // It is a message we want to process
                l_bProcessEnable = 1;
                l_ret = TEC_PROT_NO_ERR;
            } else {
                // It is an error
                l_ret = -1;
            }
        }

        if (l_ret != -1){
            if (l_bProcessEnable == 1){
                if (sReceivedUdp[TEC_PROT_ACK_POS] == 0){
                    switch (sReceivedUdp[TEC_PROT_CMD_ID_POS]){
                    case TEC_PROT_CMD_SCAN_ID:
                        if (sDeviceRole == TEC_PROT_ROLE_MASTER){
                            l_ret = TEC_PROT_MASTER_RX_INFO;
                            // Get the MAC address from UDP packet
                            tecProtocolDeviceInfo_t l_SlaveInfo;
                            memset((void*)&l_SlaveInfo,0,sizeof(tecProtocolDeviceInfo_t));
                            memcpy((void*)&l_SlaveInfo.sHeader,(void*)&sReceivedUdp[0],sizeof(tecProtocolHeader_t));
                            if (sReceivedUdp[1] == TEC_PROT_CHAR_ID_LSB_TECNA){
                                memcpy((void*)&l_SlaveInfo.sDataDevice, (void*)&sReceivedUdp[TEC_PROT_START_DATA_POS],sizeof(tecProtocolSDiscoveryFrame_t));
                            } else {
                                /**it is a device from HMS
                                3 is the 1st unused field
                                2 is Field 0x1 id and len*/
                                l_pos = TEC_PROT_ACK_POS + 3 + 2  +
                                        sReceivedUdp[TEC_PROT_ACK_POS + 3 + 2] +
                                        2;
                                l_nameLen = sReceivedUdp[l_pos];
                                l_SlaveInfo.sDataDevice.s_IpHostNameLEN = l_nameLen;
                                memcpy((void*)&l_SlaveInfo.sDataDevice.s_IpHostNameData, (void*)&sReceivedUdp[l_pos+1],l_nameLen);

                            }
                            TP_AddSlaveToList(l_SlaveInfo);
                            /** sDeviceSetInfo is reset to 0 - OK*/
                        } else {
                            // It is the slave device
                            /** Verify if the Master is
                             *  really my Master
                             */
                            l_isMyMaster = TP_SlaveIsMyMaster(_sender);
                            if (l_isMyMaster > 0){
                                /** Prepare the correct answer*/
                                TP_SlaveStoreMac(&sReceivedUdp[TEC_PROT_SRC_MAC_POS]);
                                l_ret = TEC_PROT_SLAVE_RX_INFO;
                            } else {
                                /** Prepare the NACK answer */
                                l_ret = TEC_PROT_SLAVE_RX_INFO_NACK;
                            }
                        }
                        break;
                    case TEC_PROT_CMD_WARN_ID:
                        if (sDeviceRole == TEC_PROT_ROLE_MASTER){
                            l_ret = TEC_PROT_MASTER_RX_CODE;
                            // Get the device code from the slave
                            tecProtocolHeader_t l_slaveHeader;
                            tecProtocolSPresetFrame_t l_slaveDeviceSetInfo;
                            memset((void*)&l_slaveHeader,0,sizeof(tecProtocolHeader_t));
                            memset((void*)&l_slaveDeviceSetInfo,0,sizeof(tecProtocolSPresetFrame_t));
                            memcpy((void*)&l_slaveHeader,(void*)&sReceivedUdp[0],sizeof(tecProtocolHeader_t));
                            memcpy((void*)&l_slaveDeviceSetInfo, (void*)&sReceivedUdp[TEC_PROT_START_DATA_POS],sizeof(tecProtocolSPresetFrame_t));
                        TP_AddSlaveDeviceInfo(l_slaveDeviceSetInfo,(uint8_t *)&l_slaveHeader.s_SrcMacAddr);
                        } else {
                            // It is the slave device
                            /** Verify if the Master is
                             *  really my Master
                             */
                            l_isMyMaster = TP_SlaveIsMyMaster(_sender);
                            if (l_isMyMaster > 0){
                                /** Prepare the correct answer*/
                                TP_SlaveStoreMac(&sReceivedUdp[TEC_PROT_SRC_MAC_POS]);
                                l_ret = TEC_PROT_SLAVE_RX_WARN;
                            } else {
                                /** Prepare the NACK answer */
                                l_ret = TEC_PROT_SLAVE_RX_INFO_NACK;
                            }
                        }
                        break;
                    case TEC_PROT_CMD_SET_ID:
                        if (sDeviceRole == TEC_PROT_ROLE_MASTER){
                            l_ret = TEC_PROT_MASTER_RX_ACK;
                        } else {
                            memset((void*)&sSlaveConfToSet,0,sizeof(tecProtocolSDiscoveryFrame_t));
                            if (sReceivedUdp[1] == TEC_PROT_CHAR_ID_LSB_TECNA){
                                tecProtocolSPresetFrame_t l_SlaveCodeRet;
                                memset((void*)&l_SlaveCodeRet,0,sizeof(tecProtocolSPresetFrame_t));
                                memcpy((void*)&l_SlaveCodeRet.s_DeviceCode[0], (void*)&sReceivedUdp[TEC_PROT_START_DATA_POS],TEC_PROT_DEVICE_CODE_LEN);
                                int iCount = 0;
                                for (int lindex = 0; lindex < 4; lindex++){
                                    if (l_SlaveCodeRet.s_DeviceCode[lindex] == sSlaveWarned.s_DeviceCode[lindex]){
                                        iCount++;
                                    } else {
                                        break;
                                    }
                                } // end for
                                if (iCount == 4){
                                    l_ret = TEC_PROT_SLAVE_RX_SET;
                                    memcpy((void*)&sSlaveConfToSet, (void*)&sReceivedUdp[TEC_PROT_START_DATA_POS+TEC_PROT_DEVICE_CODE_LEN],sizeof(tecProtocolSDiscoveryFrame_t));
                                } else {
                                    /** Prepare the NACK answer */
                                    l_ret = TEC_PROT_SLAVE_RX_INFO_NACK;
                                }
                            } else {
                                l_ret = TEC_PROT_SLAVE_HMS;
                            }

                        }
                        break;
                    default:
                        l_ret = TEC_PROT_GEN_ERR;
                        break;
                    }
                } else {
                    l_ret = TEC_PROT_MASTER_RX_NACK;
                }
            } else {
                // i dont want to process it
                l_ret = TEC_PROT_MSG_NOT_PARSED;
            }
        } //
    } else {
        /** It could be an unknown message or the Announcement message
            if my role is slave */
        if ((sDeviceRole == TEC_PROT_ROLE_SLAVE ) && (_size == TEC_PROT_MOD_SCAN_LEN) &&
            (TP_isTheAnnMsg(_udpMessage) == 0) ){
            l_ret = TEC_PROT_ANNOUNCE_MSG;
        } else {
         l_ret = TEC_PROT_UNKWN_MSG_ERR; /* Unknown message */
        }
    }

    return l_ret;

}

int TP_AddSlaveToList(tecProtocolDeviceInfo_t _slaveDevice)
{
    int iRetVal = -1;

    if (sSlaveDiscIndexList < TEC_PROT_MAX_NUM_DEV_SUPP){
        if (_slaveDevice.sHeader.s_FirstCtrlMsb == TEC_PROT_CHAR_ID_MSB){
            if (_slaveDevice.sHeader.s_Role == TEC_PROT_ROLE_SLAVE ){
                if (_slaveDevice.sHeader.s_FirstCtrlLsb == TEC_PROT_CHAR_ID_LSB_TECNA){
                    /** We do not analyze the ACK field for the moment
                        We can save the item in the list
                    */
                    memcpy((void*)&sSlaveDiscovery[sSlaveDiscIndexList],
                           (void*)&_slaveDevice,
                           sizeof(tecProtocolDeviceInfo_t));
                    sSlaveDiscIndexList++;
                } else {
                    if (_slaveDevice.sHeader.s_FirstCtrlLsb == TEC_PROT_CHAR_ID_LSB_HMS){
                        memcpy((void*)&sSlaveDiscovery[sSlaveDiscIndexList],
                               (void*)&_slaveDevice,
                               sizeof(tecProtocolDeviceInfo_t));
                        // sizeof(tecProtocolFrame_t));
                        sSlaveDiscIndexList++;
                    } /* else - It is not a protcol TECNA or HMS*/
                } /*end else */
            }  /* else it is not a message from a slave*/
        } /* else - It is not a protcol TECNA or HMS */
    } /* else - No space in the list */


    return iRetVal;
}

int TP_AddSlaveDeviceInfo(tecProtocolSPresetFrame_t _slaveDeviceSetInfo, uint8_t *_SrcMacAddr)
{
    int iRetVal = -1;

    iRetVal = TP_RetrieveIndexFromMac(_SrcMacAddr);

    if (iRetVal != -1){
        /** Source address matches the one in the table
            We can copy it to the list */
        memcpy((void*)&sSlaveDiscovery[iRetVal].sDeviceSetInfo, (void*)&_slaveDeviceSetInfo, sizeof(_slaveDeviceSetInfo));
    } /*else */

    return iRetVal;
}

int TP_GetDevCodeFromPosition(int _pos, uint8_t *_devCode)
{
    int iRetVal = -1;
    uint8_t *l_devCode = NULL;


    if (_devCode != NULL){
        if ((sSlaveDiscIndexList > 0) && (_pos < sSlaveDiscIndexList)){
            l_devCode = &sSlaveDiscovery[_pos].sDeviceSetInfo.s_DeviceCode[0];
             memcpy((void*)&_devCode[0],(void*)l_devCode, 4);
             iRetVal = 0;
        } /*else list is empty and position is plausible */
    } /* else no _macAddr address**/

    return iRetVal;
}

int TP_GetIpAddrFromPosition(int _pos, uint8_t *_ipAddr)
{
    int iRetVal = -1;
    uint8_t *l_ipAddr = NULL;


    if (_ipAddr != NULL){
        if ((sSlaveDiscIndexList > 0) && (_pos < sSlaveDiscIndexList)){
            l_ipAddr = &sSlaveDiscovery[_pos].sDataDevice.s_IpAddrData[0];
             memcpy((void*)&_ipAddr[0],(void*)&l_ipAddr[0], 4);
             iRetVal = 0;
        } /*else list is empty and position is plausible */
    } /* else no _ipAddr address**/

    return iRetVal;


}

int TP_GetNetMaskFromPosition(int _pos, uint8_t *_netMask)
{
    int iRetVal = -1;
    uint8_t *l_netMask = NULL;


    if (_netMask != NULL){
        if ((sSlaveDiscIndexList > 0) && (_pos < sSlaveDiscIndexList)){
            l_netMask = &sSlaveDiscovery[_pos].sDataDevice.s_IpNetMaskData[0];
             memcpy((void*)&_netMask[0],(void*)&l_netMask[0], 4);
             iRetVal = 0;
        } /*else list is empty and position is plausible */
    } /* else no _ipAddr address**/

    return iRetVal;


}

int TP_GetGWAddrFromPosition(int _pos, uint8_t *_gateway)
{
    int iRetVal = -1;
    uint8_t *l_gateway = NULL;


    if (_gateway != NULL){
        if ((sSlaveDiscIndexList > 0) && (_pos < sSlaveDiscIndexList)){
            l_gateway = &sSlaveDiscovery[_pos].sDataDevice.s_IpDefGWData[0];
             memcpy((void*)&_gateway[0],(void*)&l_gateway[0], 4);
             iRetVal = 0;
        } /*else list is empty and position is plausible */
    } /* else no _ipAddr address**/

    return iRetVal;
}

int TP_GetPriDNSFromPosition(int _pos, uint8_t *_priDNS)
{
    int iRetVal = -1;
    uint8_t *l_priDNS = NULL;


    if (_priDNS != NULL){
        if ((sSlaveDiscIndexList > 0) && (_pos < sSlaveDiscIndexList)){
            l_priDNS = &sSlaveDiscovery[_pos].sDataDevice.s_IpPriDNSData[0];
             memcpy((void*)&_priDNS[0],(void*)&l_priDNS[0], 4);
             iRetVal = 0;
        } /*else list is empty and position is plausible */
    } /* else no _ipAddr address**/

    return iRetVal;
}

int TP_GetSecDNSFromPosition(int _pos, uint8_t *_secDNS)
{
    int iRetVal = -1;
    uint8_t *l_secDNS = NULL;


    if (_secDNS != NULL){
        if ((sSlaveDiscIndexList > 0) && (_pos < sSlaveDiscIndexList)){
            l_secDNS = &sSlaveDiscovery[_pos].sDataDevice.s_IpSecDNSData[0];
             memcpy((void*)&_secDNS[0],(void*)&l_secDNS[0], 4);
             iRetVal = 0;
        } /*else list is empty and position is plausible */
    } /* else no _ipAddr address**/

    return iRetVal;
}

int TP_GetDHCPFromPosition(int _pos, uint8_t *_dhcp)
{
    int iRetVal = -1;


    if (_dhcp != NULL){
        if ((sSlaveDiscIndexList > 0) && (_pos < sSlaveDiscIndexList)){
            *_dhcp = sSlaveDiscovery[_pos].sDataDevice.s_IpDHCPData;
             iRetVal = *_dhcp;
        } /*else list is empty and position is plausible */
    } /* else no _ipAddr address**/

    return iRetVal;

}

int TP_GetMacFromPosition(int _pos, uint8_t *_macAddr)
{
    int iRetVal = -1;
    uint8_t *l_srcAddr = NULL;


    if (_macAddr != NULL){
        if ((sSlaveDiscIndexList > 0) && (_pos < sSlaveDiscIndexList)){
            l_srcAddr = &sSlaveDiscovery[_pos].sHeader.s_SrcMacAddr[0];
             memcpy((void*)&_macAddr[0],(void*)&l_srcAddr[0], 6);
             iRetVal = 0;
        } /*else list is empty and position is plausible */
    } /* else no _macAddr address**/

    return iRetVal;
}

int  TP_RetrieveIndexFromMac(uint8_t *_SrcMacAddr)
{
    int iRetVal = -1;
    int i = 0;
    uint8_t l_srcAddr[6] = {0,0,0,0,0,0};

    memcpy((void*)&l_srcAddr[0],(void*)_SrcMacAddr, 6);

    if (_SrcMacAddr != NULL){
        for (i =0; i < sSlaveDiscIndexList; i++){
            if ((sSlaveDiscovery[i].sHeader.s_SrcMacAddr[0] == l_srcAddr[0]) &&
                (sSlaveDiscovery[i].sHeader.s_SrcMacAddr[1] == l_srcAddr[1]) &&
                (sSlaveDiscovery[i].sHeader.s_SrcMacAddr[2] == l_srcAddr[2]) &&
                (sSlaveDiscovery[i].sHeader.s_SrcMacAddr[3] == l_srcAddr[3]) &&
                (sSlaveDiscovery[i].sHeader.s_SrcMacAddr[4] == l_srcAddr[4]) &&
                (sSlaveDiscovery[i].sHeader.s_SrcMacAddr[5] == l_srcAddr[5])){
                    iRetVal = i;
                    i = sSlaveDiscIndexList; /*break*/
            } /* end if*/
        } /* end for scanning the list */
    } /* else no src address**/

    return iRetVal;

}

void TP_initMSetFramePayload()
{
    memset((void*)&sMSetFrame,0,sizeof(tecProtocolMSetFrame_t));
    sMSetFrame.s_IpAddrID = TEC_PROT_DATA_IPADDR_ID;
    sMSetFrame.s_IpAddrLEN =  TEC_PROT_DATA_IPADDR_LEN;
    sMSetFrame.s_IpNetMaskID = TEC_PROT_DATA_NETMASK_ID;
    sMSetFrame.s_IpNetMaskLEN = TEC_PROT_DATA_NETMASK_LEN;
    sMSetFrame.s_IpDefGWID = TEC_PROT_DATA_DEFGWAY_ID;
    sMSetFrame.s_IpDefGWLEN = TEC_PROT_DATA_DEFGWAY_LEN;
    sMSetFrame.s_IpPriDNSID = TEC_PROT_DATA_PRIDNS_ID;
    sMSetFrame.s_IpPriDNSLEN = TEC_PROT_DATA_PRIDNS_LEN;
    sMSetFrame.s_IpSecDNSID  = TEC_PROT_DATA_SECDNS_ID;
    sMSetFrame.s_IpSecDNSLEN = TEC_PROT_DATA_SECDNS_LEN;
    sMSetFrame.s_IpDHCPID = TEC_PROT_DATA_DHCP_EN_ID;
    sMSetFrame.s_IpDHCPLEN = TEC_PROT_DATA_DHCP_EN_LEN;
    sMSetFrame.s_IpHostNameID  = TEC_PROT_DATA_HOSTNAME_ID;
    sMSetFrame.s_IpHostNameLEN = 0;
}

void TP_initSSetFramePayload()
{
    sSlaveConfiguration.s_IpAddrID = TEC_PROT_DATA_IPADDR_ID;
    sSlaveConfiguration.s_IpAddrLEN =  TEC_PROT_DATA_IPADDR_LEN;
    sSlaveConfiguration.s_IpNetMaskID = TEC_PROT_DATA_NETMASK_ID;
    sSlaveConfiguration.s_IpNetMaskLEN = TEC_PROT_DATA_NETMASK_LEN;
    sSlaveConfiguration.s_IpDefGWID = TEC_PROT_DATA_DEFGWAY_ID;
    sSlaveConfiguration.s_IpDefGWLEN = TEC_PROT_DATA_DEFGWAY_LEN;
    sSlaveConfiguration.s_IpPriDNSID = TEC_PROT_DATA_PRIDNS_ID;
    sSlaveConfiguration.s_IpPriDNSLEN = TEC_PROT_DATA_PRIDNS_LEN;
    sSlaveConfiguration.s_IpSecDNSID  = TEC_PROT_DATA_SECDNS_ID;
    sSlaveConfiguration.s_IpSecDNSLEN = TEC_PROT_DATA_SECDNS_LEN;
    sSlaveConfiguration.s_IpDHCPID = TEC_PROT_DATA_DHCP_EN_ID;
    sSlaveConfiguration.s_IpDHCPLEN = TEC_PROT_DATA_DHCP_EN_LEN;
    sSlaveConfiguration.s_IpHostNameID  = TEC_PROT_DATA_HOSTNAME_ID;
}

void TP_initHeader()
{
    uint8_t l_u8i;
    memset((void*)&sHeader,0,sizeof(tecProtocolHeader_t));
    sHeader.s_FirstCtrlMsb = TEC_PROT_CHAR_ID_MSB;
    sHeader.s_FirstCtrlLsb = sTypeOfProt;
    sHeader.s_Role = sDeviceRole;
    for (l_u8i = 0; l_u8i < 6; l_u8i++){
       sHeader.s_SrcMacAddr[l_u8i] = sSourceMacAddress[l_u8i];
    }

}

int TP_copyToSetMaster(void*_src)
{
    int i_retVal = -1;
    int i = 0;
    tecProtocolMSetFrame_t *l_dest = (tecProtocolMSetFrame_t*)(&sMSetFrame);
    tecProtocolMSetDataFrame_t *l_src = (tecProtocolMSetDataFrame_t*)(_src);

    if (l_src != NULL){
        /** Destination is the tecProtocolMSetFrame_t struct
         *  Source is the tecProtocolMSetDataFrame_t struct
        */
        for (i = 0; i< TEC_PROT_DATA_GEN_IP_LEN; i++){
            l_dest->s_DeviceCode[i] = l_src->s_DeviceCode[i];
            l_dest->s_IpAddrData[i] = l_src->s_IpAddrData[i];
            l_dest->s_IpNetMaskData[i] = l_src->s_IpNetMaskData[i];
            l_dest->s_IpDefGWData[i] = l_src->s_IpDefGWData[i];
            l_dest->s_IpPriDNSData[i] = l_src->s_IpPriDNSData[i];
            l_dest->s_IpSecDNSData[i] = l_src->s_IpSecDNSData[i];
        } /* end for copy */

        l_dest->s_IpDHCPData = l_src->s_IpDHCPData;

        for (i = 0; i< TEC_PROT_DATA_HOSTNAME_LEN; i++){
            l_dest->s_IpHostNameData[i] = l_src->s_IpHostNameData[i];

            if (l_src->s_IpHostNameData[i] == TEC_PROT_DATA_END_OF_STRING){
                i_retVal = i;
                i = TEC_PROT_DATA_HOSTNAME_LEN;
                l_dest->s_IpHostNameLEN = i_retVal;
                /* break the for cycle */
            } /*else*/
        } /* end for*/

    } /* else error */

    return i_retVal;
}

int TP_isTheAnnMsg(uint8_t *_udpMessage)
{
    int iRetVal = 0;
    int i = 0;
    uint8_t l_udpMessage[TEC_PROT_MOD_SCAN_LEN];

    /* Reset the array*/
    for (i=0; i<TEC_PROT_MOD_SCAN_LEN; i++){
        l_udpMessage[i] = 0;
    }

    if (_udpMessage != NULL){
        memcpy((void*)&l_udpMessage[0],(void*)_udpMessage,TEC_PROT_MOD_SCAN_LEN);
        /* Compare the message */
        for (i=0; i<TEC_PROT_MOD_SCAN_LEN; i++){
            if (l_udpMessage[i] != cModuleScan[i]){
                /* Find a difference - break*/
                iRetVal = -1;
                i = TEC_PROT_MOD_SCAN_LEN;
            } /* else */
        } /* end for */
    } /* else */

    return iRetVal;
}

int TP_SetAnnouncedMaster(uint8_t *_ipAddress)
{
    int iRetVal = -1;
    int i = 0;
    uint8_t l_ipAddr[4];
    if (_ipAddress != NULL){
        /** Copy the IP address -
         *  We have now our reference master
         */
        memcpy((void*)&l_ipAddr[0], (void*)_ipAddress, 4);
        for (i=0; i<4; i++){
           sMasterIpAddress[i] =l_ipAddr[i];
        }
        iRetVal = 0;
    } /*else*/

    return iRetVal;
}

int TP_GetNumberOfItemInList()
{
    return sSlaveDiscIndexList;
}

int TP_GetHostName(uint8_t * _hostName, int _pos)
{
    int iRet = -1;
    if (_hostName != NULL){
        memcpy((void*)_hostName, (void*) &sSlaveDiscovery[_pos].sDataDevice.s_IpHostNameData, sSlaveDiscovery[_pos].sDataDevice.s_IpHostNameLEN);
        iRet =  sSlaveDiscovery[_pos].sDataDevice.s_IpHostNameLEN;
    } /*else*/

    return iRet;
}

int TP_ResetSlaveDevice()
{
    int iRetVal = 0;
    int i= 0;

    for (i=0; i<TEC_PROT_MAX_NUM_DEV_SUPP; i++){
        memset((void*)&sSlaveDiscovery[i],0,sizeof(tecProtocolDeviceInfo_t));
    }

    sSlaveDiscIndexList = 0;

    return iRetVal;
}

int TP_SlaveConfig(tecProtocolSDiscoveryFrame_t *_config)
{
    int iRetVal = -1;
    if (_config != NULL){
        memcpy((void*)&sSlaveConfiguration, _config,sizeof(tecProtocolSDiscoveryFrame_t));
        TP_initSSetFramePayload();
    } else {
        iRetVal = -1;
    }

    return iRetVal;
}

int TP_SlaveIsMyMaster(uint8_t *_sender)
{
    int iRetVal = -1;
    int i =0;

    if (_sender != NULL){
        for (i=0; i<4; i++){
            if (sMasterIpAddress[i] == _sender[i]){
                iRetVal = i;
            } //
        }
        if (iRetVal == 3){
            /*Compare is OK*/
            iRetVal = 1;
        } else {
            /*Compare is failed*/
            iRetVal = 0;
        }
    }

    return iRetVal;
}

int TP_SlaveStoreMac(uint8_t *_masterMAC)
{
    int iRetVal = -1;
    int i =0;

    if (_masterMAC != NULL){
        for (i=0; i<6; i++){
            sMasterMacAddress[i] = _masterMAC[i];
        }
        iRetVal = 0;
    } //

    return iRetVal;

}

int TP_GetSlaveToSet(uint8_t *_settings)
{
    int iRetVal = -1;

    if (_settings != NULL){
        memcpy((void*)_settings, (void*)&sSlaveConfToSet,sizeof(tecProtocolSDiscoveryFrame_t));
    } // else


    return iRetVal;
}

uint8_t TP_GetRandomCode()
{
    int l_i_rand_num = 0;
    uint8_t l_u8Rand = 0;

    l_i_rand_num = rand();

    l_u8Rand = (uint8_t)l_i_rand_num;

    return l_u8Rand;
}

/*** END FUNCTION SECTION ***/

#ifdef __cplusplus
}
#endif
