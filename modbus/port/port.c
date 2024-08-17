/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: port.h,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "main.h"
#include "port.h"
#include "mbutils.h"
#include "mb.h"


void ENTER_CRITICAL_SECTION(void)// Off into the supercritical total interruption
{
	//__set_PRIMASK(1);   // for ARM
	__disable_irq();      // for RISCV
}

void EXIT_CRITICAL_SECTION(void)// Exit supercritical total break open
{
	//__set_PRIMASK(0);   // for ARM
	__enable_irq();       // for RISCV
}


u8 REG_HOLDING_START=0, REG_COIL_START=0;
u8 REG_HOLDING_NREGS=100, REG_COIL_NREGS=65;
u8 usRegHoldingStart=0, usRegCoilsStart=0;


// Register read and write commands to function supports read and write 0x06 0x03
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
	// u16 *PRT=(u16*)pucRegBuffer;

    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            case MB_REG_READ:
                while( usNRegs > 0 )
                {
                    // *PRT++ = __REV16(usRegHoldingBuf[iRegIndex++]); // Sequence data transfer REV16.W
                    *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
                    *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
                    iRegIndex++;
                    usNRegs--;
                }
                break;

            case MB_REG_WRITE:
                while( usNRegs > 0 )
                {
                    // usRegHoldingBuf[iRegIndex++] = __REV16(*PRT++); // Sequence data transfer REV16.W
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    iRegIndex++;
                    usNRegs--;
                }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


// Read / write register switch 0x01  x05
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
    short           iNCoils = ( short )usNCoils;
    unsigned short  usBitOffset;
    
    // Check if we have registers mapped at this block. 
    if( ( usAddress >= REG_COIL_START ) && ( usAddress + usNCoils <= REG_COIL_START + REG_COIL_NREGS ) )
    {
        usBitOffset = ( unsigned short )( usAddress - REG_COIL_START );
        switch ( eMode )
        {
            // Read current values and pass to protocol stack. 
            case MB_REG_READ:
                while( iNCoils > 0 )
                {
                    *pucRegBuffer++ =
                        xMBUtilGetBits( usRegCoilBuf, usBitOffset,
                                        ( unsigned char )( iNCoils > 8 ? 8 : iNCoils ) );
                    iNCoils -= 8;
                    usBitOffset += 8;
                }
                break;

            // Update current register values. 
            case MB_REG_WRITE:
                while( iNCoils > 0 )
                {
                    xMBUtilSetBits( usRegCoilBuf, usBitOffset,
                                    ( unsigned char )( iNCoils > 8 ? 8 : iNCoils ),
                                    *pucRegBuffer++ );
                    iNCoils -= 8;
                }
                break;
        }

    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


// Reading function code word register 0x04
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNRegs;
    return MB_ENOREG;
}


// Read switch register 0x02
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}

