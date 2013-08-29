#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

volatile unsigned int count;
volatile unsigned int prog;
volatile unsigned int cycles;


uint8_t up(volatile uint8_t *port, uint8_t pin) {
    if (! (*port & (1 << pin))) {
        return 1;
    }
    return 0;
}

//signal
ISR(TIM0_OVF_vect) {

    //PORTB ^= 1 << PB4;

    count++;
    switch (count) {
        case 1:
            if (prog == 1) {
                PORTB = 0x1f;
            }
            if (prog == 2) {
                PORTB = 0x21;
            }
            if (prog == 3) {
                PORTB = 0x1f;
            }
            break;
        case 2:
            if (prog == 1) {
                PORTB = 0x37;
            }
            if (prog == 2) {
                PORTB = 0x9;
            }
            if (prog == 3) {
                PORTB = 0x17;
            }
            break;
        case 3:
            if (prog == 1) {
                PORTB = 0x2f;
            }
            if (prog == 2) {
                PORTB = 0x11;
            }
            if (prog == 3) {
                PORTB = 0x7;
            }
            break;
        case 4:
            if (prog == 1) {
                PORTB = 0x3d;
            }
            if (prog == 2) {
                PORTB = 0x3;
            }
            if (prog == 3) {
                PORTB = 0x5;
            }
            break;
        case 5:
            if (prog == 1) {
                PORTB = 0x3b;
            }
            if (prog == 2) {
                PORTB = 0x5;
            }
            if (prog == 3) {
                PORTB = 0x1;
            }
            break;
    }

    if (count == cycles) {
        count = 0;
    }

}

ISR(WDT_vect) {
    if (up(&PINB, PB0)) {
        prog++;
    }
}

void cpuInit () {
    /**
     * output pins PB1, PB2, PB3, PB4, PB5
     * input pins PB0
     */
    DDRB = 0xfe;

    /* pull up on PB0 */
    PORTB = 0xff;

    /* timer overflov */
    TCCR0B = 0x00;
    TCNT0 = 0x00;

    // defaults
    count = 0;
    prog = 0;
    cycles = 5;

    /*
     * Nastaveni citace pro blikani
     */
    TCCR0B |= (1 << CS00) | (1 << CS02);
    TIMSK0 = 0;
    TIMSK0 = (1 << TOIE0);

    // povoleni preruseni
    sei();

    // Watch dog and power down
    WDTCR |= (1 << WDP0) | (1 << WDP1) | (1 << WDP2);
    WDTCR |= (1 << WDTIE);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}


int main() {

    cpuInit();

    // main loop
    while(1) {

        if (prog == 0) {
            sleep_mode();
        }

        if (prog == 4) {
            cli();
            prog = 0;
            PORTB = 0xff;
            sei();
            sleep_mode();
        }
    }
}
