#ifndef BAR_VIEW_CFG_H
#define BAR_VIEW_CFG_H
/*****************************************************************************
*                                 Constant Declarations                      *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the constant.                                                        *
* purpose, unit, and resolution                                              *
******************************************************************************/

/*****************************************************************************
*                                 Global Macro Definitions                   *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/

/*****************************************************************************
*                                 Type Decleration                           *
*----------------------------------------------------------------------------*
* Decleration of type shall be accompanied by a comment that explains the    *
* purpose and usage of the type.                                             *
******************************************************************************/

void f_fuel_get_target_counts(uint8* targetCounts);


#define 	GET_FUEL_LEVEL_BAR_COUNTS		f_fuel_get_target_counts
#define 	SET_BAR_PIN						CIoHwAb_Impl_SetPin

typedef struct
{
    int LevelId;
    int WaterLightNo;
} BarLightTable;

#define BAR_TYPE_COOLANT	0
#define BAR_TYPE_FUEL_LEVEL	1


#define BAR_LED_MIN 1
#define BAR_LED_MAX	8
#define BAR_DIVISOR 

BarLightTable coolantTbl[BAR_LED_MAX] = 
{
	{0 ,  20},
	{1 ,  19},
	{2 ,  18},
	{3 ,  17},
	{4 ,  16},
	{5 ,  15},
	{6 ,  14},
	{7 ,  13},
#if 0
	{8 ,  eIOPinOutId_TT_WATER_TEMP_BAR_3},
	{9 ,  eIOPinOutId_TT_WATER_TEMP_BAR_2},
	{10,  eIOPinOutId_TT_WATER_TEMP_BAR_1},
	{11,  eIOPinOutId_TT_WATER_TEMP_BAR_0}
#endif
};

BarLightTable fuelLevelTbl[BAR_LED_MAX] = 
{
    {0 ,  eIOPinOutId_TT_FUEL_BAR_7},
    {1 ,  eIOPinOutId_TT_FUEL_BAR_6},
    {2 ,  eIOPinOutId_TT_FUEL_BAR_5},
    {3 ,  eIOPinOutId_TT_FUEL_BAR_4},
    {4 ,  eIOPinOutId_TT_FUEL_BAR_3},
    {5 ,  eIOPinOutId_TT_FUEL_BAR_2},
    {6 ,  eIOPinOutId_TT_FUEL_BAR_1},
    {7 ,  eIOPinOutId_TT_FUEL_BAR_0},
#if 0
    {8 ,  eIOPinOutId_TT_FUEL_BAR_3},
    {9 ,  eIOPinOutId_TT_FUEL_BAR_2},
    {10,  eIOPinOutId_TT_FUEL_BAR_1},
    {11,  eIOPinOutId_TT_FUEL_BAR_0}
#endif
};




#endif


