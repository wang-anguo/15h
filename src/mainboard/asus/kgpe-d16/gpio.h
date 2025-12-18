/*** SP5100 SMBUS GPIO ***/

#define SMBUS_GPIO_PCIE5_ABSENT		19

#define SMBUS_GPIO_SPD_MUX_ENABLE	52
/* SPD Mux Settings
BIT1    BIT0    State
0       0       Disabled
0       1       Normal
1       0       Socket 0 SPD
1       1       Socket 1 SPD */
#define SMBUS_GPIO_SPD_MUX_BIT0		59
#define SMBUS_GPIO_SPD_MUX_BIT1		60
