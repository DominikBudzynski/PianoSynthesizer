#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "tsi.h"
#include "DAC.h"
#include "klaw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define M10BIT	0x03FF
#define FREQ 4096

//variables used with keyboard addressing
volatile const uint32_t columns[4] = {0x00000200, 0x00000400, 0x00000800, 0x00001000};
volatile const uint16_t keyboardKeys[2][4] = {{0x1FC0, 0x1FA0, 0x1F60, 0x1EE0},
											 {0x1BE0, 0x17E0, 0xFE0, 0x1DE0}};

uint16_t col = 0;
uint16_t out = 0;
uint16_t pin = 0;
uint16_t row_numb = 0;
uint16_t col_numb = 0;
volatile static uint8_t col_counter = 0;	//counter used to iterate through columns
																	
//variables used to control tones
volatile const uint16_t tones[13] = {33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, 65};
volatile uint8_t octaves[3] = {2, 4, 8};
int8_t octave;

//DAC
uint16_t slider;
uint16_t dac;
int16_t Sinus[1024];
uint16_t phase, mod;
uint8_t sustain = 255;
uint8_t sustain_on_off = 255;
uint8_t set_octaves = 255;

//func that checks if it's possible to change tone
uint16_t changeTone(uint16_t tone, int8_t octave){
	if ((tone * octave) > 511)
		return mod = tone * (octave - 1);
	else if (tone * octave <= 0)
		return mod = tone * (octave + 1);
	else
		return mod = tone * octave;
}

//ISR
void SysTick_Handler(void)	
{ 
	if (col_counter > 3) 
		col_counter = 0;
		
	if (pin < 8160){	//checking if any key is pressed
		row_numb = pin;
		col_numb = out;
	}
	else{
		row_numb = 0;
		col_numb = 0;
	}
	col = ~kolumny[col_counter];	//setting 0 on each output(column) according to "col_counter"
	out = ((PTA->PDOR & PTA5_8_MASK_IN_ROWS) | (col & PTA9_12_MASK_OUT_COLS));	//changing columns bits
	pin = ((PTA->PDIR | PTA9_12_MASK_OUT_COLS)& SET_PORTA);	//reading pins state (only columns and rows, not whole register)

	PTA->PDOR = out;	//sending "out" to the pins
		
	++col_counter;
	if (sustain == 255 && set_octaves == 255){
		dac=(Sinus[phase] / 100) * slider + 0x0800;
		DAC_Load_Trig(dac);
		phase += mod;		
		phase &= M10BIT;
	}
}


int main (void) 
{
	uint8_t w;
	slider  = 50;
	octave = 0;
	
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	
	//init functions
	Klaw_Init();
	LCD1602_Init();
	LCD1602_Backlight(TRUE);
	TSI_Init();
	DAC_Init();
	
	for(phase = 0 ; phase < 1024; phase++)
		Sinus[phase]=(sin((double)phase * 0.0061359231515) * 2047.0);
	
	//setting to high column and row pins
	PTA->PDOR |= SET_PORTA;
	
	SysTick_Config(SystemCoreClock/FREQ);
	
  while(1)
	{
		w = TSI_ReadSlider();
	
		if (w!=0){
			slider = w;
		}
		
		//S1-S4
		if (row_numb == keyboardKeys[0][0]){
			sustain = 255;
			LCD1602_ClearAll();
			if (col_numb == keyboardKeys[1][0]){
				mod = changeTone(tones[0], octaves[octave]);
				sprintf(display, "C: %.1f [Hz]", 130.8 * (octave + 1));
				LCD1602_Print(display);
			}
			else if(col_numb == keyboardKeys[1][1]){
				mod = changeTone(tones[1], octaves[octave]);
				sprintf(display, "C#: %.1f [Hz]", 138.6 * (octave + 1));
				LCD1602_Print(display);
			}
			else if (col_numb == keyboardKeys[1][2]){
				mod = changeTone(tones[2], octaves[octave]);
				sprintf(display, "D: %.1f [Hz]", 146.8 * (octave + 1));
				LCD1602_Print(display);
			}
			else if (col_numb == keyboardKeys[1][3]){
				mod = changeTone(tones[3], octaves[octave]);
				sprintf(display, "D#: %.1f [Hz]", 155.6 * (octave + 1));
				LCD1602_Print(display);
			}
			
		}
		
		//S5-S8
		else if (row_numb ==  keyboardKeys[0][1]){
			sustain = 255;
			LCD1602_ClearAll();
			if (col_numb == keyboardKeys[1][0]){
				mod = changeTone(tones[4], octaves[octave]);
				sprintf(display, "E: %.1f [Hz]", 164.8 * (octave + 1));
				LCD1602_Print(display);
			}
			else if(col_numb == keyboardKeys[1][1]){
				mod = changeTone(tones[5], octaves[octave]);
				sprintf(display, "F: %.1f [Hz]", 174.6 * (octave + 1));
				LCD1602_Print(display);
			}
			else if (col_numb == keyboardKeys[1][2]){
				mod = changeTone(tones[6], octaves[octave]);
				sprintf(display, "F#: %.1f [Hz]", 185.0 * (octave + 1));
				LCD1602_Print(display);
			}
			else if (col_numb == keyboardKeys[1][3]){
				mod = changeTone(tones[7], octaves[octave]);
				sprintf(display, "G: %.1f [Hz]", 196.0 * (octave + 1));
				LCD1602_Print(display);
			}			
		}
		//S9-S12
		else if (row_numb ==  keyboardKeys[0][2]){
			sustain = 255;
			LCD1602_ClearAll();
			if (col_numb == keyboardKeys[1][0]){
				mod = changeTone(tones[8], octaves[octave]);
				sprintf(display, "G#: %.1f [Hz]", 207.7 * (octave + 1));
				LCD1602_Print(display);
			}
			else if(col_numb == keyboardKeys[1][1]){
				mod = changeTone(tones[9], octaves[octave]);
				sprintf(display, "A: %.1f [Hz]", 220.0 * (octave + 1));
				LCD1602_Print(display);
			}
			else if (col_numb == keyboardKeys[1][2]){
				mod = changeTone(tones[10], octaves[octave]);
				sprintf(display, "B(Bb):%.1f [Hz]", 233.1 * (octave + 1));
				LCD1602_Print(display);
			}
			else if (col_numb == keyboardKeys[1][3]){
				mod = changeTone(tones[11], octaves[octave]);
				sprintf(display, "H(B): %.1f [Hz]", 246.9 * (octave + 1));
				LCD1602_Print(display);
			}			
		}
		//S13-S16
		else if (row_numb ==  keyboardKeys[0][3]){
			sustain = 255;
			LCD1602_ClearAll();
			if (col_numb == keyboardKeys[1][0]){
				mod = changeTone(tones[12], octaves[octave]);
				sprintf(display, "C: %.1f [Hz]", 261.6 * (octave + 1));
				//sprintf(display, "mod %d   oct %d", mod, octave);
				LCD1602_Print(display);
			}
			else if(col_numb == keyboardKeys[1][1]){
				set_octaves = 0;
				octave += 1;
				
				if (octave >= 2)
					octave = 2;
				
				//sprintf(display, "mod %d   oct %d", mod, octave);
				sprintf(display, "Oktawa w gore.");
				LCD1602_Print(display);
				DELAY(100);
				set_octaves = 255;
			}
			else if (col_numb == keyboardKeys[1][2]){
				set_octaves = 0;
				octave -=1;
				
				if (octave <= 0)
					octave = 0;
				
				//sprintf(display, "mod %d   oct %d", mod, octave);
				sprintf(display, "Oktawa w dol.");
				LCD1602_Print(display);
				DELAY(100);
				set_octaves = 255;
			}
			else if (col_numb == keyboardKeys[1][3]){
				sprintf(display, "Wcisnieto: S16");
				sustain = ~(sustain);
				LCD1602_Print(display);
			}
		}
		else{
			sustain = 0;
		}	
	}	
}
