
                           MounRiver Studio_Community
                                Version: v1.50
                                
                           Nedelcu Bogdan Sebastian
                                16/August/2024
                     Modbus RTU stack running on CH32V307VCT6
***********************************************************************************            
  At startup you can open a Termite serial communication tool, configured:
 
     19200 bps, 8N1, no handshake
  
  and see some chip info sent with printf through USART1 configured by debug.c:

     SystemClk: 96000000
     Chip type: 30700518
     ChipID: 00000518
   
  After you see the above info, do not forget to close the port, so the RMMS
  be able to open the port ! :)
***********************************************************************************     
  To test Modbus we need RMMS utility (Radzio! Modbus Master Simulator):
  
  https://en.radzio.dxp.pl/modbus-master-simulator/
  
  Use a USB-TTL adapter connected to pins A9(USART1_Tx) and A10(USART1_Rx)
  
  In MENU > Connections > Settings > Protocol - select - Modbus RTU
  
  In MENU > Connections > Settings > Modbus RTU - select -
      Port = COMx, Bitrate = 19200, Parity = NONE, Stop bits = 1
  
  In MENU > File > New - select - Device ID = 1, Holding registers, Length = 99
  
  In MENU > Connections > Connect

               Below are configurations made in different files  
***********************************************************************************
  Line  59 in mb.c, NULL is redefined. Is first defined in stddef.h (line 34).
***********************************************************************************
  Line 345 in mb.c. Modbus_Request_Flag is set, so we know Modbus was readed.
***********************************************************************************
  Line 395 in mb.c. In a known working example of TCP MODBUS running on STM32F407,
      the lines are commented. Maybe it interfere with TCP.
***********************************************************************************
  Line 30 & 36 in port.c __set_PRIMASK from ARM changed to RISC-V.
***********************************************************************************
  Line 41 in port.c. Holds usefull parameters of Modbus registers configuration.
***********************************************************************************
  Line 140 in portserial.c. Added code to test USART1 interrupt working.
     Send 'A' using Termite over serial (38400,8,N) to blink led.
