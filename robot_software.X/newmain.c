/* 
 * File:   newmain.c
 * Author: allis
 *
 * Created on May 8, 2022, 6:37 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <time.h>

#define MOTOR1_FWD 0b00000010
#define MOTOR1_BWD 0b00000001
#define MOTOR2_FWD 0b00010000
#define MOTOR2_BWD 0b00100000
#define MOTOR1_BRK 0b00000011
#define MOTOR2_BRK 0b00110000

#define SW_REG_1    PIND
#define SW_REG_2    PINC

void my_delay_ms_10ms_steps(int ms)
{
  while (0 < ms)
  {  
    _delay_ms(10);
    ms -= 10;
  }
}

int motor1_drive(int dir) {
    if (dir) {
        PORTB = MOTOR1_BWD;
    }
    else {
        PORTB = MOTOR1_FWD;
    }
    return (EXIT_SUCCESS);
}

int motor2_drive(int dir) {
    if (dir) {
        PORTC = MOTOR2_BWD;
    }
    else {
        PORTC = MOTOR2_FWD;
    }
    return (EXIT_SUCCESS);
}


int drive_straight(int dir) {
    if (dir) {
        PORTB = MOTOR1_BWD;
        PORTC = MOTOR2_BWD; 
    }
    else {
        PORTB = MOTOR1_FWD;
        PORTC = MOTOR2_FWD;
    } 
    return (EXIT_SUCCESS);
}

int turn(int dir, float ang) {
    const int delay_time = ang / 360 * 4000;
    if (dir) {
        PORTB = MOTOR1_FWD;
        PORTC = MOTOR2_BWD;
        my_delay_ms_10ms_steps(delay_time);
        PORTB = MOTOR1_BRK;
        PORTC = MOTOR2_BRK;
    }
    else {
        PORTB = MOTOR1_BWD;
        PORTC = MOTOR2_FWD;
        my_delay_ms_10ms_steps(delay_time);
        PORTB = MOTOR1_BRK;
        PORTC = MOTOR2_BRK;
    }
    return (EXIT_SUCCESS);
}

int spin(int dir) {
    if (dir) {
        PORTB = MOTOR1_FWD;
        PORTC = MOTOR2_BWD; 
    }
    else {
        PORTB = MOTOR1_BWD;
        PORTC = MOTOR2_FWD;
    } 
    return (EXIT_SUCCESS);
}

int get_bit(uint8_t reg, uint8_t bit_num) {
    /*
     * Isolate a specific bit from a register.
     * Registers are little-endian. 
     */
    
    return ((reg >> bit_num) & 0b00000001);
}

int write_bit(uint8_t *reg, uint8_t bit_num, uint8_t status) {
    /*
     * Write a bit to an LED.
     *
     * LED_reg      :   pointer to register that LED is on (e.g PORTD)
     * LED_num      :   bit number of LED within the register (1-8)
     * status       :   1 for ON and 0 for OFF
     */
    if (status){
        *reg |= (1 << bit_num);
    }
    else {
        *reg &= ~(1 << bit_num);
    }
        
    return (EXIT_SUCCESS);
}

//int mux_select(uint8_t select, uint8_t *mux_select_reg, uint8_t mux_select_bit1_loc) {
//    /*
//     * write the select bits of a multiplexer
//     * 
//     * select           :   value of select bits
//     * mux_select_reg   :   pointer to the register used to output select bits
//     * select_bit1_loc  :   location of 1st select bit within register (1-8)
//     */
//    
//    //write the select bits of the multiplexer to sample the switch
//    for (int ind = 0; ind++; ind < 3) {
//        uint8_t select_bit_i = get_bit(select, ind);
//        write_bit(mux_select_reg, mux_select_bit1_loc + ind, select_bit_i);
//    }
//    
//    return (EXIT_SUCCESS);
//}

int main(int argc, char** argv) {
    // motor 1 on pins B0 and B1
    DDRB = 0b00000011;
    // motor 2 on pins C4 and C5
    DDRC = 0b00110000;
    
    DDRD = 0b00000000;
    // spin(0);
    
    // turn(0, 360);
    // PORTD = (1 << 3);
    
    while (1){
        uint8_t IR_readings[4];
        uint8_t sw_readings[8];
        
        // get border IR readings
        IR_readings[0] = get_bit(PIND, 4);
        IR_readings[1] = get_bit(PIND, 5);
        IR_readings[3] = get_bit(PIND, 6);
        IR_readings[4] = get_bit(PIND, 7);
        
        // get first 4 switch readings        
        sw_readings[0] = get_bit(SW_REG_1, 0);
        sw_readings[1] = get_bit(SW_REG_1, 1);
        sw_readings[2] = get_bit(SW_REG_1, 2);
        sw_readings[3] = get_bit(SW_REG_1, 3);
        
        // get second 4 switch readings
        sw_readings[4] = get_bit(SW_REG_2, 0);
        sw_readings[5] = get_bit(SW_REG_2, 1);
        sw_readings[6] = get_bit(SW_REG_2, 2);
        sw_readings[7] = get_bit(SW_REG_2, 3);
        
        write_bit(&PORTC, 4, sw_readings[2]);
    }
    
    return (EXIT_SUCCESS);
}