#include <avr/io.h>
#include <avr/interrupt.h>

#define CS  PD5
#define CLK PD6
#define DIO PD7

#define OC0B_PIN PB1

#define H 0x01
#define G 0x02
#define F 0x04
#define E 0x08
#define D 0x10
#define C 0x20
#define B 0x40
#define A 0x80


uint8_t hex[] ={
    A|B|C|D|E|F,   // 0
    B|C,           // 1
    A|B|D|E|G,     // 2
    A|B|C|D|G,     // 3
    B|C|F|G,       // 4
    A|C|D|F|G,     // 5
    A|C|D|E|F|G,   // 6
    A|B|C,         // 7
    A|B|C|D|E|F|G, // 8
    A|B|C|D|F|G,   // 9
    A|B|C|E|F|G,   // a
    C|D|E|F|G,     // b
    A|D|E|F,       // c
    B|C|D|E|G,     // d
    A|D|E|F|G,     // e
    A|E|F|G,       // f
};

static inline void initTimer0(void)
{
    // Timer 0 configuration : Fast PWM
    TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00)  // Prescaler = 1024
              | (1 << WGM02); //Fast PWM
    TCCR0A |= ((1 << COM0B1) | (0 << COM0B0) // Clear PORT OC0B on OCR0B match
               | (1 << WGM00) | (1 << WGM01));

    OCR0A = 0x7F; // Counter Top
    OCR0B = 0x20;
}

void write_byte(uint8_t byte_value){

    PORTD &= ~(1 << CLK);

    for (volatile int i = 0; i < 8; i++)
    {
        if (byte_value & 0x80)
        {
            PORTD |= (1 << DIO);
        }else{
            PORTD &= ~(1 << DIO);
        }
        for (volatile int j = 0; j < 0x2f; ++j);
        PORTD |= (1 << CLK);
        byte_value = byte_value << 1;
        for (volatile int j = 0; j < 0x2f; ++j);
        PORTD &= ~(1 << CLK);
        
    }
}

int main(void)
{
    /* setup */
    // DDRB |=  (1 << OC0B_PIN);

    DDRD |=  (1 << CS) | (1 << CLK) | (1 << DIO);
    DDRB |=  (1 << PB0);

    // initTimer0();

    // sei(); // enable global interrupts

    uint8_t a = 0;
    PORTD |= (1 << CS);
    PORTB |= (1 << PB0);
    /* loop */
    while (1){
        PORTD &= ~(1 << CS);
        PORTB &= ~(1 << PB0);
        // for (volatile int j = 0; j < 0x1; j++);
        // write_byte((0x04));
        write_byte(1 + (a & 0x03));
        // for (volatile int j = 0; j < 0x1; j++);
        write_byte(hex[0x0 + (a & 0x0f)]);
        for (volatile int j = 0; j < 0x1f; j++);
        a++;
        PORTD |= (1 << CS);
        PORTB |= (1 << PB0);
        for (volatile int32_t j = 0; j < 0x1ffff; j++);
    };
}
