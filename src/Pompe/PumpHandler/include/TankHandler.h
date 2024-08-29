#ifndef TANKHANDLER_H
#define TANKHANDLER_H
#include "additional_types.h"
#include "Rte_Type_Def.h"

class TankHandler
{

public:
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: constructor
----------------------------------------------------------------
@parameter: --
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
TankHandler()
{

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: destructor
----------------------------------------------------------------
@parameter: --
@Returnvalue: -
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
~TankHandler()
{

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@Author:    Gerald Emvoutou | Digital Transformation & Technology
                                Alliance
@Creation: 27.03.2023
----------------------------------------------------------------
@Function Description: the function read the water level
----------------------------------------------------------------
@parameter: --
@Returnvalue: the value in percent
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
tUInt8 ui8_GetTankLevel()
{
    tUInt8 ui8_GetTankLevel = 0;
    Rte_Call_Read_Water_Level_PT_ESP8266_Water_Level(ui8_GetTankLevel);
    return ui8_GetTankLevel;
}

};
#endif
