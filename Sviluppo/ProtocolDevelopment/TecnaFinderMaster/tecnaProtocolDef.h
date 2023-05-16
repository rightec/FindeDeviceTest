#ifndef TECNAPROTOCOLDEF_H
#define TECNAPROTOCOLDEF_H

#include "stdint.h"

enum {
    TEC_PROT_DATA_IPADDR_ID      = 0x0,
    TEC_PROT_DATA_NETMASK_ID     = 0x1,
    TEC_PROT_DATA_DEFGWAY_ID     = 0x2,
    TEC_PROT_DATA_PRIDNS_ID      = 0x3,
    TEC_PROT_DATA_SECDNS_ID      = 0x4,
    TEC_PROT_DATA_DHCP_EN_ID     = 0x5,
    TEC_PROT_DATA_HOSTNAME_ID    = 0x6,
    TEC_PROT_DATA_MAX_FIELD_NUM  = 0x7
};

enum {
  TEC_PROT_CMD_SCAN_ID   = 0x0,
  TEC_PROT_CMD_WARN_ID   = 0x1,
  TEC_PROT_CMD_SET_ID    = 0x2,
  TEC_PROT_CMD_MAX_ID    = 0x3
};

enum {
  TEC_PROT_ROLE_MASTER   = 0x0,
  TEC_PROT_ROLE_SLAVE    = 0x1,
};

enum {
  TEC_PROT_SEND_ACK   = 0x0,
  TEC_PROT_SEND_NACK  = 0x1,
};

#define TEC_PROT_MAX_NUM_DEV_SUPP       255 /* Max number of device supported*/
#define TEC_PROT_MAX_UDP_DATA           255
#define TEC_PROT_CHAR_ID_MSB            0xC1
#define TEC_PROT_START_DATA_POS         18
#define TEC_PROT_MAX_FIELD_DATA         (TEC_PROT_MAX_UDP_DATA - TEC_PROT_START_DATA_POS)

#define TEC_PROT_DATA_GEN_IP_LEN        4
#define TEC_PROT_MOD_SCAN_LEN           12
#define TEC_PROT_DEVICE_CODE_LEN        TEC_PROT_DATA_GEN_IP_LEN
#define TEC_PROT_DATA_IPADDR_LEN        TEC_PROT_DATA_GEN_IP_LEN
#define TEC_PROT_DATA_NETMASK_LEN       TEC_PROT_DATA_GEN_IP_LEN
#define TEC_PROT_DATA_DEFGWAY_LEN       TEC_PROT_DATA_GEN_IP_LEN
#define TEC_PROT_DATA_PRIDNS_LEN        TEC_PROT_DATA_GEN_IP_LEN
#define TEC_PROT_DATA_SECDNS_LEN        TEC_PROT_DATA_GEN_IP_LEN
#define TEC_PROT_DATA_DHCP_EN_LEN       1
#define TEC_PROT_DATA_HOSTNAME_LEN      64
#define TEC_PROT_DATA_END_OF_STRING     0x03

#define TEC_PROT_MAX_DATA_LEN  ((TEC_PROT_DATA_GEN_IP_LEN + 2)* TEC_PROT_DATA_MAX_FIELD_NUM) + \
    (TEC_PROT_DATA_DHCP_EN_LEN + 2) + \
    (TEC_PROT_DATA_HOSTNAME_LEN + 2)

#define TEC_PROT_ROLE_POS              14
#define TEC_PROT_CMD_ID_POS            15
#define TEC_PROT_ACK_POS               17
#define TEC_PROT_SRC_MAC_POS            8

typedef enum tecProtErr{
  TEC_PROT_NO_ERR       = 0x0,
  TEC_PROT_UNKWN_MSG_ERR,           /* Unknown Message error*/
  TEC_PROT_GEN_ERR,
  TEC_PROT_MASTER_ERR,
  TEC_PROT_MASTER_RX_ACK,           /* Master received ACK from slave */
  TEC_PROT_MASTER_RX_NACK,          /* Master received NACK from slave */
  TEC_PROT_MASTER_RX_CODE,          /* Master received setting code from slave */
  TEC_PROT_MASTER_RX_INFO,          /* Master received Info from slave */
  TEC_PROT_SLAVE_RX_INFO,           /* Slave received Scan Info request from master */
  TEC_PROT_SLAVE_RX_INFO_NACK,      /* Slave received Scan Info request from another master */
  TEC_PROT_SLAVE_RX_WARN,           /* Slave received WARN message from Master */
  TEC_PROT_SLAVE_RX_SET,            /* Slave received SET message from Master */
  TEC_PROT_SLAVE_HMS,               /* This Slave Message is an HMS type */
  TEC_PROT_ANNOUNCE_MSG,            /* Received the announce message */
  TEC_PROT_MSG_NOT_PARSED,          /* The message is recognized but not parsed */
  TEC_PROT_MSG_END_LIST             /* End of the list */
}tecProtErr_t;

typedef enum tecProtType{
    TEC_PROT_CHAR_ID_LSB_HMS    =    0xAB,
    TEC_PROT_CHAR_ID_LSB_TECNA  =    0xAC
}tecProtType_t;

typedef struct tecProtocolHeader{
    uint8_t s_FirstCtrlMsb;
    uint8_t s_FirstCtrlLsb;
    uint8_t s_DestMacAddr[6];
    uint8_t s_SrcMacAddr[6];
    uint8_t s_Role;
    uint8_t s_CmdId;
    uint8_t s_DataLen;
    uint8_t s_Ack;
}tecProtocolHeader_t;

typedef struct tecProtocolData{
    uint8_t s_Data[TEC_PROT_MAX_DATA_LEN];
}tecProtocolData_t;

typedef struct tecProtocolFrame{
    tecProtocolHeader_t sHeader;
    tecProtocolData_t sData;
}tecProtocolFrame_t;

/* Scan message from Master*/
typedef struct tecProtocolMScanFrame{
    uint8_t s_Data[TEC_PROT_MOD_SCAN_LEN];
}tecProtocolMScanFrame_t;

/* Command ID = 0x02 message from Master*/
typedef struct tecProtocolMSetFrame{
    uint8_t s_DeviceCode[TEC_PROT_DEVICE_CODE_LEN];
    uint8_t s_IpAddrID; /*  = TEC_PROT_DATA_IPADDR_ID */
    uint8_t s_IpAddrLEN; /* = TEC_PROT_DATA_IPADDR_LEN */
    uint8_t s_IpAddrData[TEC_PROT_DATA_IPADDR_LEN];

    uint8_t s_IpNetMaskID; /*  = TEC_PROT_DATA_NETMASK_ID */
    uint8_t s_IpNetMaskLEN; /* = TEC_PROT_DATA_NETMASK_LEN */
    uint8_t s_IpNetMaskData[TEC_PROT_DATA_NETMASK_LEN];

    uint8_t s_IpDefGWID; /*  = TEC_PROT_DATA_DEFGWAY_ID */
    uint8_t s_IpDefGWLEN; /* = TEC_PROT_DATA_DEFGWAY_LEN */
    uint8_t s_IpDefGWData[TEC_PROT_DATA_DEFGWAY_LEN];

    uint8_t s_IpPriDNSID; /*  = TEC_PROT_DATA_PRIDNS_ID */
    uint8_t s_IpPriDNSLEN; /* = TEC_PROT_DATA_PRIDNS_LEN */
    uint8_t s_IpPriDNSData[TEC_PROT_DATA_PRIDNS_LEN];

    uint8_t s_IpSecDNSID; /*  = TEC_PROT_DATA_SECDNS_ID */
    uint8_t s_IpSecDNSLEN; /* = TEC_PROT_DATA_SECDNS_LEN */
    uint8_t s_IpSecDNSData[TEC_PROT_DATA_SECDNS_LEN];

    uint8_t s_IpDHCPID; /*  = TEC_PROT_DATA_DHCP_EN_ID */
    uint8_t s_IpDHCPLEN; /* = TEC_PROT_DATA_DHCP_EN_LEN */
    uint8_t s_IpDHCPData;

    uint8_t s_IpHostNameID; /*  = TEC_PROT_DATA_HOSTNAME_ID */
    uint8_t s_IpHostNameLEN; /* = TEC_PROT_DATA_HOSTNAME_LEN */
    uint8_t s_IpHostNameData[TEC_PROT_DATA_HOSTNAME_LEN];


}tecProtocolMSetFrame_t;

/* Command ID = 0x02 message from Master - Only Raw data*/
typedef struct tecProtocolMSetDataFrame{
    uint8_t s_DeviceCode[TEC_PROT_DEVICE_CODE_LEN];
    uint8_t s_IpAddrData[TEC_PROT_DATA_IPADDR_LEN];
    uint8_t s_IpNetMaskData[TEC_PROT_DATA_NETMASK_LEN];
    uint8_t s_IpDefGWData[TEC_PROT_DATA_DEFGWAY_LEN];
    uint8_t s_IpPriDNSData[TEC_PROT_DATA_PRIDNS_LEN];
    uint8_t s_IpSecDNSData[TEC_PROT_DATA_SECDNS_LEN];
    uint8_t s_IpDHCPData;
    uint8_t s_IpHostNameData[TEC_PROT_DATA_HOSTNAME_LEN];
}tecProtocolMSetDataFrame_t;


  /* Command ID = 0x00 message from Slave*/
  typedef struct tecProtocolSDiscoveryFrame {
  uint8_t s_IpAddrID;  /*  = TEC_PROT_DATA_IPADDR_ID */
  uint8_t s_IpAddrLEN; /* = TEC_PROT_DATA_IPADDR_LEN */
  uint8_t s_IpAddrData[TEC_PROT_DATA_IPADDR_LEN];

  uint8_t s_IpNetMaskID;  /*  = TEC_PROT_DATA_NETMASK_ID */
  uint8_t s_IpNetMaskLEN; /* = TEC_PROT_DATA_NETMASK_LEN */
  uint8_t s_IpNetMaskData[TEC_PROT_DATA_NETMASK_LEN];

  uint8_t s_IpDefGWID;  /*  = TEC_PROT_DATA_DEFGWAY_ID */
  uint8_t s_IpDefGWLEN; /* = TEC_PROT_DATA_DEFGWAY_LEN */
  uint8_t s_IpDefGWData[TEC_PROT_DATA_DEFGWAY_LEN];

  uint8_t s_IpPriDNSID;  /*  = TEC_PROT_DATA_PRIDNS_ID */
  uint8_t s_IpPriDNSLEN; /* = TEC_PROT_DATA_PRIDNS_LEN */
  uint8_t s_IpPriDNSData[TEC_PROT_DATA_PRIDNS_LEN];

  uint8_t s_IpSecDNSID;  /*  = TEC_PROT_DATA_SECDNS_ID */
  uint8_t s_IpSecDNSLEN; /* = TEC_PROT_DATA_SECDNS_LEN */
  uint8_t s_IpSecDNSData[TEC_PROT_DATA_SECDNS_LEN];

  uint8_t s_IpDHCPID;  /*  = TEC_PROT_DATA_DHCP_EN_ID */
  uint8_t s_IpDHCPLEN; /* = TEC_PROT_DATA_DHCP_EN_LEN */
  uint8_t s_IpDHCPData;

  uint8_t s_IpHostNameID;  /*  = TEC_PROT_DATA_HOSTNAME_ID */
  uint8_t s_IpHostNameLEN; /* = TEC_PROT_DATA_HOSTNAME_LEN */
  uint8_t s_IpHostNameData[TEC_PROT_DATA_HOSTNAME_LEN];

} tecProtocolSDiscoveryFrame_t;


/* Command ID = 0x01 message from Slave*/
typedef struct tecProtocolSPresetFrame{
    uint8_t s_DeviceCode[TEC_PROT_DEVICE_CODE_LEN];
    uint8_t s_MaxDataField; /* = TEC_PROT_DATA_MAX_FIELD_NUM */
}tecProtocolSPresetFrame_t;

typedef struct tecProtocolDeviceInfo{
   tecProtocolHeader_t sHeader;
   tecProtocolSDiscoveryFrame_t sDataDevice;
   tecProtocolSPresetFrame_t sDeviceSetInfo;
} tecProtocolDeviceInfo_t;



#endif /* TECNAPROTOCOLDEF_H */
