// PICkit 2 Lesson file header

#ifndef PICKIT_H
#define PICKIT_H

/** D E C L A R A T I O N S **************************************************/
typedef enum { FREQ_250_KHZ = 0,
               FREQ_500_KHZ = 1,
               FREQ_1_MHZ = 2,
               FREQ_2_MHZ = 3,
               FREQ_4MHZ = 4,
               FREQ_8_MHZ = 5,
               FREQ_16_MHZ = 6,
               FREQ_32_MHZ = 7,
               FREQ_64_MHZ = 8} IntOSCFreq;

/** D E F I N I T I O N S ****************************************************/


/** E X T E R N S ************************************************************/
// declare variables accessible by other files.

/** P R O T O T Y P E S ******************************************************/
void SetupINT0Switch(void);
void EnableInterrupts(void);
void SetIntOSC(IntOSCFreq *ClockSet);

#endif