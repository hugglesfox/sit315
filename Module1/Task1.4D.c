// The task sheet wasn't clear if the inputs have to be on seperate pin
// interupt groups so I put them all on PCINT0.
//
// Also to keep things simple I've created a sort of game where the timer will
// turn the LED on and any of the 3 connected buttons will turn it back
// off.
//
// Lastly the arduino library is painful if you actually want to use the
// hardware's features so this is written in plain C 
//
// Note that the 3 buttons are on Arduino pins D4, D5, D6 which conveniently
// are pins 4, 5 and 6 on port D of the atmega328p. The LED is on arduino pin
// 13 which is pin 5 of port B.

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LED 5

#define BTN1 4
#define BTN2 5
#define BTN3 6

// Pin change 2 interrupt handler
ISR(PCINT2_vect) {
	// Turn the LED off
	PORTB = 0;
}

// Timer1 overflow interrupt handler
ISR(TIMER1_OVF_vect) {
	// Turn the LED on
	PORTB = 1 << LED;
}

void setup_serial(void)
{
	// Configure 8bit data, 1 stop bit
	UCSR0C |= 3 << UCSZ00;

	// Enable USART0 transmitting
	UCSR0B |= 1 << TXEN0;

	// Set the baud rate to 9600
	UBRR0 = 103;
}

void serial_write(char *msg)
{
	for(size_t i = 0; i < strlen(msg); i++) {
		// Wait for USART to be ready to send
		while (!(UCSR0A & (1 << UDRE0)));

		// Send character
		UDR0 = msg[i];
	}
}

void setup_led(void)
{
	// Enable output mode for pin 13
	DDRB |= 1 << LED;
}

void setup_buttons(void) 
{
	// Enable pull up resistors for pins 4, 5 and 6
	PORTD |= (1 << BTN1) | (1 << BTN2) | (1 << BTN3);
	
	// Enable pin change interrupt 2 (contains arduino digital pins 0 to 7)
	PCICR |= 1 << PCIE2;
	
	// Enable PCI for pins pins 4, 5 and 6
	PCMSK2 |= (1 << BTN1) | (1 << BTN2) | (1 << BTN3);
}

void setup_timer(void)
{
	// My arduino has a 16MHz clock so a full cycle of the 16 bit timer
	// (Timer 1) would take 2^16 / 16MHz which is about 0.004 seconds. If
	// we apply a 1024 clock prescaler we would get 2^16 / 15KHz or about 4
	// seconds.
	//
	// Therefore we can use the timer overflow interrupt for an interrupt
	// every 4 seconds

	// Enable Timer1 Overflow interrupt
	TIMSK1 |= (1 << TOIE1);

	// Start the timer using the 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

int main(void)
{
	setup_buttons();
	setup_led();
	setup_serial();
	setup_timer();

	// Enable interrupts
	sei();

	char *timer = malloc(sizeof(char) * 10);

	for (;;) {
		// Print the timer value
		sprintf(timer, "%u\r\n", TCNT1);
		serial_write(timer);

		// If the LED is on
		if ((PORTB >> LED) & 1) {
			serial_write("LED on\r\n");
		} else {
			serial_write("LED off\r\n");
		}
	}
}

