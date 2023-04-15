#include "klaw.h"
#include "frdm_bsp.h"

void Klaw_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		//turning on PortA
	//rows - inputs - 0
	PORTA->PCR[5] |= PORT_PCR_MUX(1);
	PORTA->PCR[6] |= PORT_PCR_MUX(1);
	PORTA->PCR[7] |= PORT_PCR_MUX(1);
	PORTA->PCR[8] |= PORT_PCR_MUX(1);
	PORTA->PCR[5] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[6] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[7] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[8] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	
	//columns - outputs - 1
	PORTA->PCR[9] |= PORT_PCR_MUX(1);
	PORTA->PCR[9] &= (~PORT_PCR_SRE_MASK);
	PORTA->PCR[9] &= (~PORT_PCR_DSE_MASK);
	PORTA->PCR[10] |= PORT_PCR_MUX(1);
	PORTA->PCR[10] &= (~PORT_PCR_SRE_MASK);
	PORTA->PCR[10] &= (~PORT_PCR_DSE_MASK);
	PORTA->PCR[11] |= PORT_PCR_MUX(1);
	PORTA->PCR[11] &= (~PORT_PCR_SRE_MASK);
	PORTA->PCR[11] &= (~PORT_PCR_DSE_MASK);
	PORTA->PCR[12] |= PORT_PCR_MUX(1);
	PORTA->PCR[12] &= (~PORT_PCR_SRE_MASK);
	PORTA->PCR[12] &= (~PORT_PCR_DSE_MASK);
	
	PTA->PDDR |= PTA9_12_MASK_OUT_COLS; //setting pins direction
}