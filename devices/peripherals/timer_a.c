/*
  MSP430 Emulator
  Copyright (C) 2016 Rudolf Geosits (rgeosits@live.esu.edu)  
                                                                      
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
                                                                   
  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.          
                                                       
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses
*/

#include "timer_a.h"

void handle_timer_a (Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Timer_a *timer = cpu->timer_a;

  uint8_t TA0CTL = *timer->TA0CTL;

  // Handle Timer_A0 Control Register
  uint8_t TASSEL1 = (TA0CTL >> 8) & 0x03;
  uint8_t ID1     = (TA0CTL >> 6) & 0x03;
  uint8_t MC1     = (TA0CTL >> 4) & 0x03;
  uint8_t TA0CLR   = (TA0CTL >> 2) & 0x01;
  uint8_t TA0IE    = (TA0CTL >> 1) & 0x01;
  uint8_t TA0IFG   = TA0CTL & 0x01;

  switch (TASSEL1) {
  case 0b00: {timer->source_1 = TACLK; break;}
  case 0b01: {timer->source_1 = ACLK; break;}
  case 0b10: {timer->source_1 = SMCLK; break;}
  case 0b11: {timer->source_1 = INCLK; break;}
  default: break;
  }

  switch (ID1) {
  case 0b00: {timer->idiv_1 = 1; break;}
  case 0b01: {timer->idiv_1 = 2; break;}
  case 0b10: {timer->idiv_1 = 4; break;}
  case 0b11: {timer->idiv_1 = 8; break;}
  default: break;
  }

  timer->mode_0 = MC1;
  
  if (!timer->timer_0_started && MC1 != 0) {
    print_console(emu, "START TIMER\n");
  }

  /* Timer_A clear; setting this bit resets TAR, the clock divider,
     and the count direction. The TACLR bit is automatically 
     reset and is always read as zero. */
  if (TA0CLR) {    
    *timer->TA0R = 0;
    *timer->TA0CTL &= 0xFF0B; // 0b00001011
  }
}

void setup_timer_a (Emulator *emu)
{
  Cpu *cpu = emu->cpu;
  Timer_a *timer = cpu->timer_a;

  // Configure Timer_A0 Registers
  const uint16_t TA0CTL_VLOC  = 0x160;
  const uint16_t TA0R_VLOC  = 0x170;
  const uint16_t TA0CCTL0_VLOC = 0x162;
  const uint16_t TA0CCR0_VLOC  = 0x172;
  const uint16_t TA0CCTL1_VLOC  = 0x164;
  const uint16_t TA0CCR1_VLOC  = 0x174;
  const uint16_t TA0CCTL2_VLOC  = 0x166;
  const uint16_t TA0CCR2_VLOC  = 0x176;
  const uint16_t TA0IV_VLOC  = 0x12E;
  
  *(timer->TA0CTL  = (uint16_t *) get_addr_ptr(TA0CTL_VLOC)) = 0;
  *(timer->TA0R  = (uint16_t *) get_addr_ptr(TA0R_VLOC)) = 0;
  *(timer->TA0CCTL0  = (uint16_t *) get_addr_ptr(TA0CCTL0_VLOC)) = 0;
  *(timer->TA0CCR0  = (uint16_t *) get_addr_ptr(TA0CCR0_VLOC)) = 0;
  *(timer->TA0CCTL1  = (uint16_t *) get_addr_ptr(TA0CCTL1_VLOC)) = 0;
  *(timer->TA0CCR1  = (uint16_t *) get_addr_ptr(TA0CCR1_VLOC)) = 0;
  *(timer->TA0CCTL2  = (uint16_t *) get_addr_ptr(TA0CCTL2_VLOC)) = 0;
  *(timer->TA0CCR2  = (uint16_t *) get_addr_ptr(TA0CCR2_VLOC)) = 0;
  *(timer->TA0IV  = (uint16_t *) get_addr_ptr(TA0IV_VLOC)) = 0;

  // Configure Timer_A1 Registers
  const uint16_t TA1CTL_VLOC  = 0x180;
  const uint16_t TA1R_VLOC  = 0x190;
  const uint16_t TA1CCTL0_VLOC = 0x182;
  const uint16_t TA1CCR0_VLOC  = 0x192;
  const uint16_t TA1CCTL1_VLOC  = 0x184;
  const uint16_t TA1CCR1_VLOC  = 0x194;
  const uint16_t TA1CCTL2_VLOC  = 0x186;
  const uint16_t TA1CCR2_VLOC  = 0x196;
  const uint16_t TA1IV_VLOC  = 0x11E;
  
  *(timer->TA1CTL  = (uint16_t *) get_addr_ptr(TA1CTL_VLOC)) = 0;
  *(timer->TA1R  = (uint16_t *) get_addr_ptr(TA1R_VLOC)) = 0;
  *(timer->TA1CCTL0  = (uint16_t *) get_addr_ptr(TA1CCTL0_VLOC)) = 0;
  *(timer->TA1CCR0  = (uint16_t *) get_addr_ptr(TA1CCR0_VLOC)) = 0;
  *(timer->TA1CCTL1  = (uint16_t *) get_addr_ptr(TA1CCTL1_VLOC)) = 0;
  *(timer->TA1CCR1  = (uint16_t *) get_addr_ptr(TA1CCR1_VLOC)) = 0;
  *(timer->TA1CCTL2  = (uint16_t *) get_addr_ptr(TA1CCTL2_VLOC)) = 0;
  *(timer->TA1CCR2  = (uint16_t *) get_addr_ptr(TA1CCR2_VLOC)) = 0;
  *(timer->TA1IV  = (uint16_t *) get_addr_ptr(TA1IV_VLOC)) = 0;

  // Configure other
  timer->source_0 = 0b10;
  timer->timer_0_started = false;

  timer->source_1 = 0b10;
  timer->timer_1_started = false;
}

/* POWER UP CLEAR (PUC)      
 *
 * A PUC is always generated when a POR is generated, but a POR is not
 * generated by a PUC. The following events trigger a PUC:  
 *                                                
 * A POR signal                             
 * Watchdog timer expiration when in watchdog mode only
 * Watchdog timer security key violation          
 * A Flash memory security key violation        
 * A CPU instruct fetch from the peripheral address range 0h to 01FFh

void power_up_clear () {
  *P1OUT = *P1DIR = *P1IFG = *P1IE = *P1SEL = *P1SEL2 = *P1REN = 0;
}
 */
