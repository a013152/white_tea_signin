//*************************************************************************************
// Version 0.25
//*************************************************************************************

#ifndef __HFRDAPI_H__
#define __HFRDAPI_H__

#if !defined(API_EXPORTS)
#define DECLSPEC __declspec(dllimport)
#else
#define DECLSPEC
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* HID_DEVICE;

#define SUCCESS                         0

#define HID_DEVICE_FAILED               0xC0
#define HID_DEVICE_NOT_FOUND            0xC1
#define HID_DEVICE_NOT_OPENED           0xC2
#define HID_DEVICE_ALREADY_OPENED       0xC3
#define HID_DEVICE_TRANSFER_TIMEOUT     0xC4
#define HID_DEVICE_TRANSFER_FAILED      0xC5
#define HID_DEVICE_CANNOT_GET_HID_INFO  0xC6
#define HID_DEVICE_HANDLE_ERROR         0xC7
#define HID_DEVICE_INVALID_BUFFER_SIZE  0xC8
#define HID_DEVICE_SYSTEM_CODE          0xC9
#define HID_DEVICE_UNKNOW_ERROR         0xCA

#define LIB_FAILED                      0xE0
#define LIB_CHECKDATA_FAILED            0xE1
#define LIB_LENGTH_FAILED               0xE2
#define LIB_PARAMETER_FAILED            0xE3

#define MAX_RF_BUFFER                   1024
 

//=====================================================================================
//============================== System function ======================================
//=====================================================================================
DECLSPEC int WINAPI Sys_GetLibVersion(DWORD *pVer);

DECLSPEC int WINAPI Sys_GetDeviceNum(WORD vid, WORD pid, DWORD *pNum);

DECLSPEC int WINAPI Sys_GetHidSerialNumberStr(DWORD deviceIndex,
                                              WORD vid,
                                              WORD pid,
                                              char *deviceString,
                                              DWORD deviceStringLength);

DECLSPEC int WINAPI Sys_Open(HID_DEVICE *device,
                             DWORD index,
                             WORD vid,
                             WORD pid);

DECLSPEC BOOL WINAPI Sys_IsOpen(HID_DEVICE device);

DECLSPEC int WINAPI Sys_Close(HID_DEVICE *device);

DECLSPEC int WINAPI Sys_GetTimeouts(HID_DEVICE device,
                                    DWORD *getRepotTimeout,
                                    DWORD *setReportTimeout);

DECLSPEC int WINAPI Sys_SetTimeouts(HID_DEVICE device,
                                    DWORD getRepotTimeout,
                                    DWORD setReportTimeout);

DECLSPEC int WINAPI Sys_SetDllDeviceAddr(WORD dllDevAddr);

DECLSPEC int WINAPI Sys_GetDllDeviceAddr(WORD *pDllDevID);

DECLSPEC int WINAPI Sys_SetDeviceAddr(HID_DEVICE device, WORD devAddr);

DECLSPEC int WINAPI Sys_GetDeviceAddr(HID_DEVICE device, WORD *pDevAddr);

DECLSPEC int WINAPI Sys_GetModel(HID_DEVICE device, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI Sys_GetSnr(HID_DEVICE device, BYTE *pSnr);

DECLSPEC int WINAPI Sys_SetLight(HID_DEVICE device, BYTE color);

DECLSPEC int WINAPI Sys_SetBuzzer(HID_DEVICE device, BYTE msec);

DECLSPEC int WINAPI Sys_SetAntenna(HID_DEVICE device, BYTE mode);

DECLSPEC int WINAPI Sys_InitType(HID_DEVICE device, BYTE type);

DECLSPEC int WINAPI Sys_Reset(HID_DEVICE device, BYTE type);


//=====================================================================================
//================================ Flash function =====================================
//=====================================================================================
DECLSPEC int WINAPI Sys_UserFlash_Read(HID_DEVICE device,
                                       WORD block,
                                       WORD startAddress,
                                       WORD length,
                                       BYTE *pData);

DECLSPEC int WINAPI Sys_UserFlash_Write(HID_DEVICE device,
                                        WORD block,
                                        WORD startAddress,
                                        WORD length,
                                        BYTE *pData);


//=====================================================================================
//============================== Auxiliary function ===================================
//=====================================================================================
DECLSPEC int WINAPI Aux_SingleDES(BYTE desType,
                                  BYTE *key,
                                  BYTE *srcData,
                                  DWORD srcDataLen,
                                  BYTE *destData,
                                  DWORD *destDataLen);

DECLSPEC int WINAPI Aux_TripleDES(BYTE desType,
                                  BYTE *key,
                                  BYTE *srcData,
                                  DWORD srcDataLen,
                                  BYTE *destData,
                                  DWORD *destDataLen);
                        
DECLSPEC int WINAPI Aux_SingleMAC(BYTE *key,
                                  BYTE *initData,
                                  BYTE *srcData, 
                                  DWORD srcDataLen, 
                                  BYTE *macData);

DECLSPEC int WINAPI Aux_TripleMAC(BYTE *key,
                                  BYTE *initData,
                                  BYTE *srcData, 
                                  DWORD srcDataLen, 
                                  BYTE *macData);

DECLSPEC int WINAPI Des_Encrypt(BYTE *szOut,
                                BYTE *szIn,
                                DWORD inlen,
                                BYTE *key,
                                DWORD keylen);

DECLSPEC int WINAPI Des_Decrypt(BYTE *szOut,
                                BYTE *szIn,
                                DWORD inlen,
                                BYTE *key,
                                DWORD keylen);


//=====================================================================================
//============================== ISO14443A function ===================================
//=====================================================================================
DECLSPEC int WINAPI TyA_Request(HID_DEVICE device, BYTE mode , WORD *pTagType);

DECLSPEC int WINAPI TyA_Halt(HID_DEVICE device);

//============================== M1 function
DECLSPEC int WINAPI TyA_Anticollision(HID_DEVICE device,
                                      BYTE bcnt,
                                      BYTE *pSnr,
                                      BYTE *pLen);

DECLSPEC int WINAPI TyA_Select(HID_DEVICE device,
                               BYTE *pSnr,
                               BYTE snrLen,
                               BYTE *pSak);

DECLSPEC int WINAPI TyA_CascAnticoll(HID_DEVICE device, 
                     BYTE cln, 
                     BYTE bcnt, 
                     BYTE *pSnr);

DECLSPEC int WINAPI TyA_CascSelect(HID_DEVICE device, 
                   BYTE cln, 
                   BYTE *pSnr, 
                   BYTE *pSak);

DECLSPEC int WINAPI TyA_CS_DownloadKey(HID_DEVICE device, BYTE keyIndex, BYTE *pKey);

DECLSPEC int WINAPI TyA_CS_AuthE2(HID_DEVICE device, 
                                  BYTE mode, 
                                  BYTE block, 
                                  BYTE keyIndex);

DECLSPEC int WINAPI TyA_CS_Authentication2(HID_DEVICE device,
                                           BYTE mode,
                                           BYTE block,
                                           BYTE *pKey);

DECLSPEC int WINAPI TyA_CS_Read(HID_DEVICE device,
                                BYTE block,
                                BYTE *pData,
                                BYTE *pLen);

DECLSPEC int WINAPI TyA_CS_Write(HID_DEVICE device, BYTE block, BYTE *pData);

DECLSPEC int WINAPI TyA_CS_InitValue(HID_DEVICE device, BYTE block, long value);

DECLSPEC int WINAPI TyA_CS_ReadValue(HID_DEVICE device, BYTE block, long *pValue);

DECLSPEC int WINAPI TyA_CS_Decrement(HID_DEVICE device, BYTE block, long value);

DECLSPEC int WINAPI TyA_CS_Increment(HID_DEVICE device, BYTE block, long value);

DECLSPEC int WINAPI TyA_CS_Restore(HID_DEVICE device, BYTE block);

DECLSPEC int WINAPI TyA_CS_Transfer(HID_DEVICE device, BYTE block);

//============================== Ultralight function
DECLSPEC int WINAPI TyA_UL_Select(HID_DEVICE device, BYTE *pSnr, BYTE *pLen);

DECLSPEC int WINAPI TyA_UL_Authentication1(HID_DEVICE device, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI TyA_UL_Authentication2(HID_DEVICE device, 
                                           BYTE *pInData, 
                                           BYTE *pOutData, 
                                           BYTE *pLen);

DECLSPEC int WINAPI TyA_UL_Authentication(HID_DEVICE device, BYTE *pKey);

DECLSPEC int WINAPI TyA_UL_ChangeKey(HID_DEVICE device, BYTE *pKey);

DECLSPEC int WINAPI TyA_UL_GetVersion(HID_DEVICE device, BYTE *pData, BYTE* pLen);

DECLSPEC int WINAPI TyA_UL_Read(HID_DEVICE device, BYTE addr, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI TyA_UL_FastRead(HID_DEVICE device, BYTE startAddr, BYTE endAddr, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI TyA_UL_Write(HID_DEVICE device, BYTE page, BYTE *pdata);

DECLSPEC int WINAPI TyA_UL_CompWrite(HID_DEVICE device, BYTE addr, BYTE *pData);

DECLSPEC int WINAPI TyA_UL_ReadCnt(HID_DEVICE device, BYTE addr, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI TyA_UL_IncrCnt(HID_DEVICE device, BYTE addr, BYTE *pIncrValue);

DECLSPEC int WINAPI TyA_UL_PwdAuth(HID_DEVICE device, BYTE *pPwd, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI TyA_UL_ReadSig(HID_DEVICE device, BYTE addr, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI TyA_UL_WriteSig(HID_DEVICE device, BYTE addr, BYTE *pdata);

DECLSPEC int WINAPI TyA_UL_LockSig(HID_DEVICE device, BYTE arg);

DECLSPEC int WINAPI TyA_UL_CheckTearingEvent(HID_DEVICE device, BYTE addr, BYTE *pValidFlag);

DECLSPEC int WINAPI TyA_UL_VCSL(HID_DEVICE device, BYTE *pIID, BYTE *pPcdCaps, BYTE *pVCTID);

//============================== NTAG function
DECLSPEC int WINAPI TyA_NTAG_AnticollSelect(HID_DEVICE device, BYTE *pSnr, BYTE *pLen);

DECLSPEC int WINAPI TyA_NTAG_GetVersion(HID_DEVICE device, BYTE *pData, BYTE* pLen);

DECLSPEC int WINAPI TyA_NTAG_Read(HID_DEVICE device, 
                                  BYTE addr, 
                                  BYTE *pData, 
                                  BYTE *pLen);

DECLSPEC int WINAPI TyA_NTAG_FastRead(HID_DEVICE device, 
                                      BYTE startAddr, 
                                      BYTE endAddr, 
                                      BYTE *pData, 
                                      BYTE *pLen);

DECLSPEC int WINAPI TyA_NTAG_Write(HID_DEVICE device, BYTE addr, BYTE *pdata);

DECLSPEC int WINAPI TyA_NTAG_CompWrite(HID_DEVICE device, BYTE addr, BYTE *pData);

DECLSPEC int WINAPI TyA_NTAG_ReadCnt(HID_DEVICE device, 
                                     BYTE addr, 
                                     BYTE *pData, 
                                     BYTE *pLen);

DECLSPEC int WINAPI TyA_NTAG_PwdAuth(HID_DEVICE device, 
                                     BYTE *pPwd, 
                                     BYTE *pData, 
                                     BYTE *pLen);

DECLSPEC int WINAPI TyA_NTAG_ReadSig(HID_DEVICE device, 
                                     BYTE addr, 
                                     BYTE *pData, 
                                     BYTE *pLen);

//============================== CPU function
DECLSPEC int WINAPI TyA_DF_Reset(HID_DEVICE device,
                                 BYTE mode,
                                 BYTE *pData,
                                 BYTE *pMsgLg);

DECLSPEC int WINAPI TyA_Reset(HID_DEVICE device,
                              BYTE mode,
                              BYTE *pData,
                              BYTE *pMsgLg);

DECLSPEC int WINAPI TyA_CosCommand(HID_DEVICE device,
                                   BYTE *pCommand,
                                   BYTE cmdLen,
                                   BYTE *pData,
                                   BYTE *pMsgLg);

DECLSPEC int WINAPI TyA_Deselect(HID_DEVICE device);


//=====================================================================================
//============================== ISO14443B function ===================================
//=====================================================================================
DECLSPEC int WINAPI TyB_RequestAttrib(HID_DEVICE device,
                                      BYTE mode,
                                      BYTE *pData,
                                      BYTE *pMsgLg);

DECLSPEC int WINAPI TyB_Hltb(HID_DEVICE device, DWORD PUPI);

DECLSPEC int WINAPI TyB_CosCommand(HID_DEVICE device, 
                                   BYTE *pCommand,
                                   BYTE cmdLen,
                                   BYTE *pData,
                                   BYTE *pMsgLg);

DECLSPEC int WINAPI TyB_Deselect(HID_DEVICE device);

DECLSPEC int WINAPI TyB_AT020_Check(HID_DEVICE device, BYTE *pKey);

DECLSPEC int WINAPI TyB_AT020_Read(HID_DEVICE device,
                                   BYTE page,
                                   BYTE *pData,
                                   BYTE *pMsgLen);

DECLSPEC int WINAPI TyB_AT020_Write(HID_DEVICE device, BYTE page, BYTE *pData);

DECLSPEC int WINAPI TyB_AT020_Lock(HID_DEVICE device, BYTE *pData);

DECLSPEC int WINAPI TyB_AT020_Count(HID_DEVICE device, BYTE *pData);

DECLSPEC int WINAPI TyB_AT020_Deselect(HID_DEVICE device);

DECLSPEC int WINAPI TyB_SR_Initiate(HID_DEVICE device, BYTE *pChipID);
    
DECLSPEC int WINAPI TyB_SR_Select(HID_DEVICE device, BYTE ChipID);

DECLSPEC int WINAPI TyB_SR_Completion(HID_DEVICE device);

DECLSPEC int WINAPI TyB_SR176_ReadBlock(HID_DEVICE device,
                                        BYTE block,
                                        BYTE *pData,
                                        BYTE *pLen);

DECLSPEC int WINAPI TyB_SR176_WriteBlock(HID_DEVICE device, BYTE block, BYTE *pData);

DECLSPEC int WINAPI TyB_SR176_ProtectBlock(HID_DEVICE device, BYTE lockReg);

DECLSPEC int WINAPI TyB_SRI4K_ResetToInventory(HID_DEVICE device);

DECLSPEC int WINAPI TyB_SRI4K_GetUid(HID_DEVICE device, BYTE *pUid, BYTE *pLen);

DECLSPEC int WINAPI TyB_SRI4K_ReadBlock(HID_DEVICE device,
                                        BYTE block,
                                        BYTE *pData,
                                        BYTE *pLen);

DECLSPEC int WINAPI TyB_SRI4K_WriteBlock(HID_DEVICE device, BYTE block, BYTE *pData);

DECLSPEC int WINAPI TyB_SRI4K_ProtectBlock(HID_DEVICE device, BYTE lockReg);

DECLSPEC int WINAPI TyB_SRI4K_Authenticate(HID_DEVICE device,
                                           BYTE *pRND,
                                           BYTE *pSIG,
                                           BYTE *pLen);

DECLSPEC int WINAPI TyB_SGIDC_RequestAttrib(HID_DEVICE device, 
                                            BYTE mode, 
                                            BYTE *pData, 
                                            BYTE *pMsgLen);

DECLSPEC int WINAPI TyB_SGIDC_GetUid(HID_DEVICE device, 
                                     BYTE *pUid, 
                                     BYTE *pLen);


//=====================================================================================
//============================== ISO15693 function ====================================
//=====================================================================================
DECLSPEC int WINAPI I15693_Inventory(HID_DEVICE device, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI I15693_Inventorys(HID_DEVICE device, BYTE *pData, BYTE *pLen);

DECLSPEC int WINAPI I15693_StayQuiet(HID_DEVICE device, BYTE *pUID);

DECLSPEC int WINAPI I15693_Select(HID_DEVICE device, BYTE *pUID);

DECLSPEC int WINAPI I15693_ResetToReady(HID_DEVICE device, BYTE mode, BYTE *pUID);

DECLSPEC int WINAPI I15693_Read(HID_DEVICE device,
                                BYTE mode,
                                BYTE *pUID,
                                BYTE block,
                                BYTE number,
                                BYTE *pData,
                                BYTE *pLen);

DECLSPEC int WINAPI I15693_Write(HID_DEVICE device,
                                 BYTE mode,
                                 BYTE *pUID,
                                 BYTE block,
                                 BYTE *pData);
                  
DECLSPEC int WINAPI I15693_LockBlock(HID_DEVICE device,
                                     BYTE mode,
                                     BYTE *pUID,
                                     BYTE block);

DECLSPEC int WINAPI I15693_WriteAFI(HID_DEVICE device, 
                                    BYTE mode,
                                    BYTE *pUID,
                                    BYTE AFI);

DECLSPEC int WINAPI I15693_LockAFI(HID_DEVICE device, BYTE mode, BYTE *pUID);

DECLSPEC int WINAPI I15693_WriteDSFID(HID_DEVICE device, 
                                      BYTE mode,
                                      BYTE *pUID,
                                      BYTE DSFID);

DECLSPEC int WINAPI I15693_LockDSFID(HID_DEVICE device, BYTE mode, BYTE *pUID);

DECLSPEC int WINAPI I15693_GetSystemInformation(HID_DEVICE device,
                                                BYTE  mode,
                                                BYTE  *pUID,
                                                BYTE  *pData,
                                                BYTE  *pLen);
                           
DECLSPEC int WINAPI I15693_GetBlockSecurity(HID_DEVICE device,
                                            BYTE  mode,
                                            BYTE  *pUID,
                                            BYTE  block,
                                            BYTE  number,
                                            BYTE  *pData,
                                            BYTE  *pLen);

DECLSPEC int WINAPI I15693_DirectTransmit(HID_DEVICE device, 
                                          BYTE *pCommand,
                                          BYTE cmdLen,
                                          BYTE *pData,
                                          BYTE *pMsgLg);

//============================== ICODE function
DECLSPEC int WINAPI ICODE_SetEAS(HID_DEVICE device,
                                 BYTE mode,
                                 BYTE mfgCode,
                                 BYTE *pUID);

DECLSPEC int WINAPI ICODE_ResetEAS(HID_DEVICE device,
                                   BYTE mode,
                                   BYTE mfgCode,
                                   BYTE *pUID);

DECLSPEC int WINAPI ICODE_LockEAS(HID_DEVICE device,
                                  BYTE mode,
                                  BYTE mfgCode,
                                  BYTE *pUID);

DECLSPEC int WINAPI ICODE_EasAlarm(HID_DEVICE device,
                                   BYTE mode,
                                   BYTE mfgCode,
                                   BYTE *pUID,
                                   BYTE maskLen,
                                   BYTE *pEasId,
                                   BYTE *pData,
                                   BYTE *pLen);

DECLSPEC int WINAPI ICODE_PasswordProtectEasAfi(HID_DEVICE device,
                                                BYTE mode,
                                                BYTE mfgCode,
                                                BYTE *pUID);

DECLSPEC int WINAPI ICODE_WriteEasId(HID_DEVICE device,
                                     BYTE mode,
                                     BYTE mfgCode,
                                     BYTE *pUID,
                                     BYTE *pEasId);

DECLSPEC int WINAPI ICODE_ReadEPC(HID_DEVICE device,
                                  BYTE mode,
                                  BYTE mfgCode,
                                  BYTE *pUID,
                                  BYTE *pData,
                                  BYTE *pLen);

DECLSPEC int WINAPI ICODE_GetNxpSystemInfomation(HID_DEVICE device,
                                                 BYTE mode,
                                                 BYTE mfgCode,
                                                 BYTE *pUID,
                                                 BYTE *pData,
                                                 BYTE *pLen);

DECLSPEC int WINAPI ICODE_GetRandomNumber(HID_DEVICE device,
                                          BYTE mode,
                                          BYTE mfgCode,
                                          BYTE *pUID,
                                          BYTE *pData,
                                          BYTE *pLen);

DECLSPEC int WINAPI ICODE_SetPassword(HID_DEVICE device,
                                      BYTE mode,
                                      BYTE mfgCode,
                                      BYTE *pUID,
                                      BYTE pwdIdentifier,
                                      BYTE *pXorPassword);

DECLSPEC int WINAPI ICODE_WritePassword(HID_DEVICE device,
                                        BYTE mode,
                                        BYTE mfgCode,
                                        BYTE *pUID,
                                        BYTE pwdIdentifier,
                                        BYTE *pPassword);

DECLSPEC int WINAPI ICODE_LockPassword(HID_DEVICE device,
                                       BYTE mode,
                                       BYTE mfgCode,
                                       BYTE *pUID,
                                       BYTE pwdIdentifier);

DECLSPEC int WINAPI ICODE_ProtectPage(HID_DEVICE device,
                                      BYTE mode,
                                      BYTE mfgCode,
                                      BYTE *pUID,
                                      BYTE address,
                                      BYTE status);

DECLSPEC int WINAPI ICODE_LockPageProtectionCondition(HID_DEVICE device,
                                                      BYTE mode,
                                                      BYTE mfgCode,
                                                      BYTE *pUID,
                                                      BYTE address);

DECLSPEC int WINAPI ICODE_GetBlockProtectionStatus(HID_DEVICE device,
                                                   BYTE mode,
                                                   BYTE mfgCode,
                                                   BYTE *pUID,
                                                   BYTE block,
                                                   BYTE number,
                                                   BYTE *pData,
                                                   BYTE *pLen);

DECLSPEC int WINAPI ICODE_SLI_Destroy(HID_DEVICE device,
                                      BYTE mode,
                                      BYTE mfgCode,
                                      BYTE *pUID);

DECLSPEC int WINAPI ICODE_SLIX_Destroy(HID_DEVICE device,
                                       BYTE mode,
                                       BYTE mfgCode,
                                       BYTE *pUID,
                                       BYTE *pXorPassword);

DECLSPEC int WINAPI ICODE_SLI_EnablePrivacy(HID_DEVICE device,
                                            BYTE mode,
                                            BYTE mfgCode,
                                            BYTE *pUID);

DECLSPEC int WINAPI ICODE_SLIX_EnablePrivacy(HID_DEVICE device,
                                             BYTE mode,
                                             BYTE mfgCode,
                                             BYTE *pUID,
                                             BYTE *pXorPassword);

DECLSPEC int WINAPI ICODE_64BitPasswordProtection(HID_DEVICE device,
                                                  BYTE mode,
                                                  BYTE mfgCode,
                                                  BYTE *pUID);

DECLSPEC int WINAPI ICODE_StayQuietPersistent(HID_DEVICE device,
                                              BYTE mode,
                                              BYTE mfgCode,
                                              BYTE *pUID);

int WINAPI ICODE_ReadSignature(HID_DEVICE device,
                               BYTE mode,
                               BYTE mfgCode,
                               BYTE *pUID,
                               BYTE *pData,
                               BYTE *pLen);


#ifdef __cplusplus
}
#endif

#endif