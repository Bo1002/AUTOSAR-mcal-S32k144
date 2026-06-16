/*
 * Port.c
 *
 *  Created on: Jun 11, 2026
 *      Author: BO
 */

#include "Port.h"
#include <S32K144.h>



static uint8                    Port_InitState = STD_OFF;
static const Port_ConfigType*   Port_pConfig   = NULL_PTR;



FUNC(void, PORT_CODE) Port_Init(P2CONST(Port_ConfigType,AUTOMATIC,PORT_APPL_CONST)  ConfigPt)
{
    /**
     * @brief 
     * PinPort = 0 , 1,2,3,4 <=> A,B,C,D,E
     * 
     */
    const Port_PinConfigType* pPin;
    uint8 i;
    uint16 PinID  ; 
    uint16 PortID ;
    uint32 pcrValue;

    #if(PORT_DEV_ERROR_DETECT == STD_ON)
    if(ConfigPt == NULL_PTR)
    {
        return;
    }
    #endif
    /*luu congifptr vao bien static Portconfig */
    Port_pConfig = ConfigPt;
    /*turn of Bit select by loop i */
    for(i = 0; i < ConfigPt->numPins ; i++)
    {
        pPin = &ConfigPt->pinConfigPtr[i];
        PinID = (uint16)(pPin->pinId % 32);
        PortID = (uint16)(pPin->pinId / 32); 

        pcrValue = PORT_PCR_MUX(pPin->pinMode);

        if(pPin->pullEnable == TRUE)
        {
            pcrValue |= PORT_PCR_PE_MASK;
        }

        if(pPin->pullUp == TRUE)
        {
            pcrValue |= PORT_PCR_PS_MASK;
        }

        if(pPin->driveStrength == TRUE)
        {
            pcrValue |= PORT_PCR_DSE_MASK;
        }
        /*turnning Port*/
        switch (PortID)
        {
        case 0u:
            IP_PORTA->PCR[PinID] = pcrValue;
            break;
        case 1u:
            IP_PORTB->PCR[PinID] = pcrValue;
            break;
        case 2u:
            IP_PORTC->PCR[PinID] = pcrValue;
            break;
        case 3u:
            IP_PORTD->PCR[PinID] = pcrValue;
            break;
        case 4u:
            IP_PORTE->PCR[PinID] = pcrValue;
            break;
        default:
            break;
        }


        switch (PortID)
        {
        case 0u:
            if(pPin->direction == PORT_PIN_OUT)
            {
                IP_PTA->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTA->PDDR &= ~(1u << PinID);
            }
            break;

        case 1u:
            if(pPin->direction == PORT_PIN_OUT)
            {
                IP_PTB->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTB->PDDR &= ~(1u << PinID);
            }
            break;

        case 2u:
            if(pPin->direction == PORT_PIN_OUT)
            {
                IP_PTC->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTC->PDDR &= ~(1u << PinID);
            }
            break;

        case 3u:
            if(pPin->direction == PORT_PIN_OUT)
            {
                IP_PTD->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTD->PDDR &= ~(1u << PinID);
            }
            break;

        case 4u:
            if(pPin->direction == PORT_PIN_OUT)
            {
                IP_PTE->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTE->PDDR &= ~(1u << PinID);
            }
            break;
        default:
            break;
        }

    }

    Port_InitState = STD_ON;
}



/**
 * @brief Write 
 * 
 */
FUNC(void,PORT_CODE) Port_SetPinDirection(VAR(Port_PinType,PORT_APPL_DATA) Pin, VAR(Port_PinDirectionType, PORT_APPL_DATA) Direction)
{
    
    #if(PORT_DEV_ERROR_DETECT == STD_ON)
        if(Port_InitState == STD_OFF)
        {return;}
    #endif

    uint8 i;
    uint16 PinID;
    uint16 PortID;
    const Port_PinConfigType* pPin = NULL_PTR ;
    

    for ( i = 0; i < Port_pConfig->numPins; i++)
    {
        if(Port_pConfig->pinConfigPtr[i].pinId == Pin)
        {
            pPin = &Port_pConfig->pinConfigPtr[i];
            break;
        }
    }


    #if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (pPin == NULL_PTR)              { return; }  /* PORT_E_PARAM_PIN */
    if (pPin->dirChangeable == FALSE)  { return; }  /* PORT_E_DIRECTION_UNCHANGEABLE */
    #endif

    PinID  = (uint16)(pPin->pinId % 32);
    PortID = (uint16)(pPin->pinId / 32);

     switch (PortID)
        {
        case 0u:
            if(Direction == PORT_PIN_OUT)
            {
                IP_PTA->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTA->PDDR &= ~(1u << PinID);
            }
            break;

        case 1u:
            if(Direction == PORT_PIN_OUT)
            {
                IP_PTB->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTB->PDDR &= ~(1u << PinID);
            }
            break;

        case 2u:
            if(Direction == PORT_PIN_OUT)
            {
                IP_PTC->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTC->PDDR &= ~(1u << PinID);
            }
            break;

        case 3u:
            if(Direction == PORT_PIN_OUT)
            {
                IP_PTD->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTD->PDDR &= ~(1u << PinID);
            }
            break;

        case 4u:
            if(Direction == PORT_PIN_OUT)
            {
                IP_PTE->PDDR |=  1u << PinID;
            }
            else
            {
                IP_PTE->PDDR &= ~(1u << PinID);
            }
            break;
        default:
            break;
        }


}


/**
 * @brief reset all the config port to the config direction 
 * 
 */
FUNC(void, PORT_CODE) Port_RefreshPortDirection(void)
{
    uint8  i;
    uint16 portIndex;
    uint16 pinIndex;
    const Port_PinConfigType* pPin;

    #if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_InitState == STD_OFF) { return; }
    #endif

    for (i = 0u; i < Port_pConfig->numPins; i++)
    {
        pPin = &Port_pConfig->pinConfigPtr[i];

        /* Chỉ refresh pin KHÔNG cho phép đổi direction */
        if (pPin->dirChangeable == FALSE)
        {
            portIndex = (uint16)(pPin->pinId / 32u);
            pinIndex  = (uint16)(pPin->pinId % 32u);

            switch (portIndex)
            {
                case 0u:
                    if (pPin->direction == PORT_PIN_OUT) { IP_PTA->PDDR |=  (1UL << pinIndex); }
                    else                                 { IP_PTA->PDDR &= ~(1UL << pinIndex); }
                    break;
                case 1u:
                    if (pPin->direction == PORT_PIN_OUT) { IP_PTB->PDDR |=  (1UL << pinIndex); }
                    else                                 { IP_PTB->PDDR &= ~(1UL << pinIndex); }
                    break;
                case 2u:
                    if (pPin->direction == PORT_PIN_OUT) { IP_PTC->PDDR |=  (1UL << pinIndex); }
                    else                                 { IP_PTC->PDDR &= ~(1UL << pinIndex); }
                    break;
                case 3u:
                    if (pPin->direction == PORT_PIN_OUT) { IP_PTD->PDDR |=  (1UL << pinIndex); }
                    else                                 { IP_PTD->PDDR &= ~(1UL << pinIndex); }
                    break;
                case 4u:
                    if (pPin->direction == PORT_PIN_OUT) { IP_PTE->PDDR |=  (1UL << pinIndex); }
                    else                                 { IP_PTE->PDDR &= ~(1UL << pinIndex); }
                    break;
                default:
                    break;
            }
        }
    }
}

/*Get version */
#if(PORT_VERSION_INFO_API == STD_ON )

FUNC(void, PORT_CODE) Port_GetVersionInfo (P2VAR(Std_VersionInfoType,AUTOMATIC,PORT_APPL_DATA) versioninfo)
{
    #if(PORT_DEV_ERROR_DETECT == STD_ON)
        if(versioninfo == NULL_PTR)
        return;
    #endif

    versioninfo->module = PORT_MODULE_ID;
    versioninfo->vendorID = PORT_VENDOR_ID;
    versioninfo->sw_major_version = PORT_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = PORT_SW_MINOR_VERSION; 
    versioninfo->sw_patch_version = PORT_SW_PATCH_VERSION;
}

#endif



FUNC(void, PORT_CODE) Port_SetPinMode(VAR(Port_PinType,AUTOMATIC) Pin, VAR(Port_PinModeType, AUTOMATIC) Mode)
{
    #if(PORT_DEV_ERROR_DETECT == STD_ON)
        if(Port_InitState == STD_OFF)
        return;
    #endif

    uint8 i; 
    uint16 PinID;
    uint16 PortID;
    uint8 max_mode; 
    const Port_PinConfigType* pPin;

    /*check each Pin config and copy to pPin*/
    for(i = 0; i < Port_pConfig->numPins; i++)
    { 
        if (Port_pConfig->pinConfigPtr[i].pinId == Pin)
            {
                pPin = &Port_pConfig->pinConfigPtr[i];
                break;
            }

    }   
    max_mode = 7;
    #if(PORT_DEV_ERROR_DETECT == STD_ON)
        if(pPin == NULL_PTR == FALSE) return;
        if (pPin->modeChangeable == FALSE){ return; }  /* PORT_E_MODE_UNCHANGEABLE */
        if (Mode > max_mode)              { return; }  /* PORT_E_PARAM_INVALID_MODE */
    #endif
    

    PinID = (uint16)(Pin % 32);
    PortID = (uint16)(Pin / 32); 

    
    switch (PortID)
    {
    case 0u:
        IP_PORTA->PCR[PinID] = (IP_PORTA->PCR[PinID] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Mode);
        break;

    case 1u:
        IP_PORTB->PCR[PinID] = (IP_PORTA->PCR[PinID] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Mode);
        break;

    case 2u: 
        IP_PORTC->PCR[PinID] = (IP_PORTA->PCR[PinID] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Mode);
        break;
    
    case 3u: 
        IP_PORTD->PCR[PinID] = (IP_PORTA->PCR[PinID] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Mode);
        break;

    case 4u: 
        IP_PORTE->PCR[PinID] = (IP_PORTA->PCR[PinID] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(Mode);
        break;

    default:
        break;
    }

}
