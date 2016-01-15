// PICkit 2 Lesson file header

#ifndef PICKIT_H
#define PICKIT_H

/** D E C L A R A T I O N S **************************************************/
typedef enum { LEFT2RIGHT, RIGHT2LEFT} LEDDirections;

/** D E F I N I T I O N S ****************************************************/


/** E X T E R N S ************************************************************/
// declare variables accessible by other files.

/** P R O T O T Y P E S ******************************************************/
void Timer0_Init(void);
void ADC_Init(void);
unsigned char ADC_Convert(void);

#endif