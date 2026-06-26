/*
 * Dio.c
 *
 *  Created on: Jun 17, 2026
 *      Author: BO
 */


#include "Dio.h"
#include <S32K144.h>



const Dio_ChannelGroupType *DioChannelGroup;

FUNC(Dio_LevelType,DIO_CODE)   Dio_ReadChannel (VAR(Dio_ChannelType,AUTOMATIC) ChannelId)
{
    uint16 PinID;
    uint16 PortID;

    Dio_LevelType portLever = STD_LOW;

    #if (DIO_DEV_ERROR_DETECT == STD_ON)
        if(ChannelId > 192 )
        return portLever;
    #endif

    PinID  = (uint16) ChannelId % 32; 
    PortID = (uint16) ChannelId / 32;
    

    switch (PortID)
    {
    case 0u:
        portLever = (Dio_LevelType)((IP_PTA->PDIR >> PinID) & 0x01u); 
        break;
    case 1u:
        portLever = (Dio_LevelType)((IP_PTB->PDIR >> PinID) & 0x01u); 
        break;
    case 2u:
        portLever = (Dio_LevelType)((IP_PTC->PDIR >> PinID) & 0x01u); 
        break;
    case 3u:
        portLever = (Dio_LevelType)((IP_PTD->PDIR >> PinID) & 0x01u); 
        break;
    case 4u:
        portLever = (Dio_LevelType)((IP_PTE->PDIR >> PinID) & 0x01u); 
        break;
    default:
        break;
    }

    return portLever;
}




FUNC(void, DIO_CODE) Dio_WriteChannel(VAR(Dio_ChannelType, AUTOMATIC) ChannelId, VAR(Dio_LevelType, AUTOMATIC) Level)
{
    uint16 PinID;
    uint16 PortID;

    #if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (ChannelId > 159u) { return; }
    #endif

    PinID  = (uint16)(ChannelId % 32u);
    PortID = (uint16)(ChannelId / 32u);

    switch (PortID)
    {
        case 0u:
            if (Level == STD_HIGH) { IP_PTA->PSOR = (1UL << PinID); }
            else                   { IP_PTA->PCOR = (1UL << PinID); }
            break;
        case 1u:
            if (Level == STD_HIGH) { IP_PTB->PSOR = (1UL << PinID); }
            else                   { IP_PTB->PCOR = (1UL << PinID); }
            break;
        case 2u:
            if (Level == STD_HIGH) { IP_PTC->PSOR = (1UL << PinID); }
            else                   { IP_PTC->PCOR = (1UL << PinID); }
            break;
        case 3u:
            if (Level == STD_HIGH) { IP_PTD->PSOR = (1UL << PinID); }
            else                   { IP_PTD->PCOR = (1UL << PinID); }
            break;
        case 4u:
            if (Level == STD_HIGH) { IP_PTE->PSOR = (1UL << PinID); }
            else                   { IP_PTE->PCOR = (1UL << PinID); }
            break;
        default:
            break;
    }
}

FUNC(void, DIO_CODE) Dio_WritePort(VAR(Dio_PortType, AUTOMATIC) PortId, VAR(Dio_PortLevelType, AUTOMATIC) Level)
{
    #if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (PortId > 4u) { return; }
    #endif

    switch (PortId)
    {
        case 0u: IP_PTA->PDOR = Level; break;
        case 1u: IP_PTB->PDOR = Level; break;
        case 2u: IP_PTC->PDOR = Level; break;
        case 3u: IP_PTD->PDOR = Level; break;
        case 4u: IP_PTE->PDOR = Level; break;
        default: break;
    }
}

FUNC(Dio_PortLevelType, DIO_CODE)  Dio_ReadPort (VAR(Dio_PortType,AUTOMATIC) PortId)
{
    Dio_PortLevelType PortL = 0u;
    #if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (PortId > 4u) { return; }
    #endif
    switch (PortId)
    {
        case 0u: PortL = (Dio_PortLevelType)(IP_PTA->PDIR); break;
        case 1u: PortL = (Dio_PortLevelType)(IP_PTB->PDIR); break;
        case 2u: PortL = (Dio_PortLevelType)(IP_PTC->PDIR); break;
        case 3u: PortL = (Dio_PortLevelType)(IP_PTD->PDIR); break;
        case 4u: PortL = (Dio_PortLevelType)(IP_PTE->PDIR); break;
        default: break;
    }
    return PortL;
} 

FUNC(Dio_PortLevelType,DIO_CODE) Dio_ReadChannelGroup (P2CONST(Dio_ChannelGroupType,AUTOMATIC,DIO_APPL_CONST) ChannelGroupIdPtr)
{
    Dio_PortLevelType PortL;

    #if(DIO_DEV_ERROR_DETECT == STD_ON)
        if(ChannelGroupIdPtr == NULL_PTR)
        return;
    #endif

    switch(ChannelGroupIdPtr->port)
    {
        case 0u: PortL = (Dio_PortLevelType)(IP_PTA->PDIR); break;
        case 1u: PortL = (Dio_PortLevelType)(IP_PTB->PDIR); break;
        case 2u: PortL = (Dio_PortLevelType)(IP_PTC->PDIR); break;
        case 3u: PortL = (Dio_PortLevelType)(IP_PTD->PDIR); break;
        case 4u: PortL = (Dio_PortLevelType)(IP_PTE->PDIR); break;
    }

    return ((PortL & ChannelGroupIdPtr->mask) >> (ChannelGroupIdPtr->offset));
}


FUNC(void, DIO_CODE)    Dio_WriteChannelGroup (P2CONST(Dio_ChannelGroupType,AUTOMATIC,DIO_APPL_CONST) ChannelGroupIdPtr, VAR(Dio_PortLevelType, AUTOMATIC) Level)
{
    Dio_PortLevelType Switch = 0u; 
    #if(DIO_DEV_ERROR_DETECT == STD_ON)
        if(ChannelGroupIdPtr == NULL_PTR)
        return;
    #endif
    /*switch level to channel gorup offet */
    Switch = (Level << ChannelGroupIdPtr->offset) & ChannelGroupIdPtr->mask; 

    switch(ChannelGroupIdPtr->port)
    {
        case 0u:
        if(Level == STD_HIGH) IP_PTA->PSOR =  (IP_PTA->PSOR & ~ChannelGroupIdPtr->mask ) | Switch; break;
        case 1u:
        if(Level == STD_HIGH) IP_PTB->PSOR =  (IP_PTB->PSOR & ~ChannelGroupIdPtr->mask ) | Switch; break;
        case 2u:
        if(Level == STD_HIGH) IP_PTC->PSOR =  (IP_PTC->PSOR & ~ChannelGroupIdPtr->mask ) | Switch; break;
        case 3u:
        if(Level == STD_HIGH) IP_PTD->PSOR =  (IP_PTD->PSOR & ~ChannelGroupIdPtr->mask ) | Switch; break;
        case 4u:
        if(Level == STD_HIGH) IP_PTE->PSOR =  (IP_PTE->PSOR & ~ChannelGroupIdPtr->mask ) | Switch; break;
        
    }
}



#if(DIO_VERSION_INFO_API == STD_ON )

FUNC(void, DIO_CODE) Dio_GetVersionInfo (P2VAR(Std_VersionInfoType,AUTOMATIC,DIO_APPL_DATA) VersionInfo)
{
    #if(DIO_DEV_ERROR_DETECT == STD_ON)
        if(VersionInfo == NULL_PTR)
        return;
    #endif

    VersionInfo->module = DIO_VENDOR_ID;
    VersionInfo->vendorID = DIO_VENDOR_ID;
    VersionInfo->sw_major_version = DIO_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = DIO_SW_MINOR_VERSION; 
    VersionInfo->sw_patch_version = DIO_SW_PATCH_VERSION;
}

#endif

FUNC(Dio_LevelType, DIO_CODE) Dio_FlipChannel(VAR(Dio_ChannelType, AUTOMATIC) ChannelId)
{
    uint16        PortID;
    uint16        PinID;
    Dio_LevelType currentLevel = STD_LOW;
    Dio_LevelType newLevel     = STD_LOW;

    #if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (ChannelId > 159u) { return newLevel; }
    #endif

    PortID = (uint16)(ChannelId / 32u);
    PinID  = (uint16)(ChannelId % 32u);

    switch (PortID)
    {
        case 0u:
            currentLevel = (Dio_LevelType)((IP_PTA->PDOR >> PinID) & 0x01u);  
            IP_PTA->PTOR = (1UL << PinID);                                    
        case 1u:
            currentLevel = (Dio_LevelType)((IP_PTB->PDOR >> PinID) & 0x01u);
            IP_PTB->PTOR = (1UL << PinID);
            break;
        case 2u:
            currentLevel = (Dio_LevelType)((IP_PTC->PDOR >> PinID) & 0x01u);
            IP_PTC->PTOR = (1UL << PinID);
            break;
        case 3u:
            currentLevel = (Dio_LevelType)((IP_PTD->PDOR >> PinID) & 0x01u);
            IP_PTD->PTOR = (1UL << PinID);
            break;
        case 4u:
            currentLevel = (Dio_LevelType)((IP_PTE->PDOR >> PinID) & 0x01u);
            IP_PTE->PTOR = (1UL << PinID);
            break;
        default:
            break;
    }

    /* Level mới = đảo ngược của level cũ */
    newLevel = (currentLevel == STD_HIGH) ? STD_LOW : STD_HIGH;

    return newLevel;
}



#if (DIO_MASKED_WRITE_PORT_API == STD_ON)

FUNC(void, DIO_CODE) Dio_MaskedWritePort(VAR(Dio_PortType, AUTOMATIC) PortId,
                                          VAR(Dio_PortLevelType, AUTOMATIC) level,
                                          VAR(Dio_PortLevelType, AUTOMATIC) Mask)
{
    #if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (PortId > 4u) { return; }
    #endif

    switch (PortId)
    {
        case 0u:
            IP_PTA->PDOR = (IP_PTA->PDOR & ~Mask) | (level & Mask);
            break;
        case 1u:
            IP_PTB->PDOR = (IP_PTB->PDOR & ~Mask) | (level & Mask);
            break;
        case 2u:
            IP_PTC->PDOR = (IP_PTC->PDOR & ~Mask) | (level & Mask);
            break;
        case 3u:
            IP_PTD->PDOR = (IP_PTD->PDOR & ~Mask) | (level & Mask);
            break;
        case 4u:
            IP_PTE->PDOR = (IP_PTE->PDOR & ~Mask) | (level & Mask);
            break;
        default:
            break;
    }
}
#endif