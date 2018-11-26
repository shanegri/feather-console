#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_LEFT (1 << PB4)
#define BUTTON_RIGHT (1 << PB3)
#define BUTTON_SHOOT (1 << PB0)
#define LISTENER (1 << PB2)
#define SENDER (1 << PB1)
#define LED_A (1 << PB2)
#define LED_B (1 << PB1)
#define send_1  PORTB |= SENDER
//interrupt pin: PB2


uint8_t button_pressed;
//xxxx x(left)(right)(shoot), 0 for no press, 1 for pressed
volatile enum button_state {Pressed, Released} left_state, right_state, shoot_state;


void turn_a_on(){
    DDRB = DDRB | LED_A;
    PORTB = PORTB | LED_A;
}
void turn_b_on(){
    DDRB = DDRB | LED_B;
    PORTB = PORTB | LED_B;
}

void turn_a_off(){
    DDRB = DDRB | LED_A;
    PORTB &= ~LED_A;
}
void turn_b_off(){
    DDRB = DDRB | LED_B;
    PORTB &= ~LED_B;
}

void clear(){
    PORTB &= ~SENDER;
}


void output_state(){
    
    if(left_state == Pressed){
        turn_a_on();
    }else{
        turn_a_off();
    }
    if(right_state == Pressed){
        turn_b_on();
    }else{
        turn_b_off();
    }
    if(shoot_state == Pressed){
        turn_a_on();
    }else{
        turn_a_off();
    }
    
}
//not finished yet
ISR(PCINT0_vect){
    
    if(PINB & LISTENER){
        send_1;
        _delay_ms(100);
        clear();
        _delay_ms(100);
        send_1;
        _delay_ms(100);
        clear();
        _delay_ms(100);
        send_1;
        _delay_ms(100);
        clear();
    }else{
        
    }
    
}

void main(){
    DDRB |= SENDER;
    PORTB = 0;
    button_pressed = 0;
    uint8_t history_l =0;
    uint8_t history_r =0;
    uint8_t history_s =0;
    uint8_t pin_read = 0;
    send_1;
    _delay_ms(100);
    clear();
    _delay_ms(100);
    send_1;
    _delay_ms(100);
    clear();
    PCMSK |= (1<<PCINT2);
    GIMSK |= (1 <<PCIE);
    sei();
    while(1){
        //read button states
        history_l = history_l << 1;
        history_r = history_r << 1;
        history_s = history_s << 1;
        
        pin_read = PINB;
        if(pin_read & BUTTON_LEFT){
            history_l |=0x01;
        }
        
        if(pin_read & BUTTON_RIGHT){
            history_r |=0x01;
        }
        
        if(pin_read & BUTTON_SHOOT){
            history_s |=0x01;
        }
        
        if(history_l == 0){
            left_state = Released;
        }
        if(history_r == 0){
            right_state = Released;
        }
        if(history_s == 0){
            shoot_state = Released;
        }
        
        
        if(history_l == 0xff){
            left_state = Pressed;
        }
        if(history_r == 0xff){
            right_state =Pressed;
        }
        if(history_s == 0xff){
            shoot_state = Pressed;
        }
        
    }
    
    
}
