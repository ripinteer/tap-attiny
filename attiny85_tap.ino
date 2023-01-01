#include <avr/io.h>   // provides pin names and other stuff

#define F_CPU 32000000UL    // Select the 32MHz CPU frequency (this value will override what you set in Arduion IDE toolbar)
#define DELAY 40    // Delay definition
#define TRIGGER 6    // Basically sensitivity

void pin_setup()
{
 DDRB&=~(1<<PB3); // PB3 as input for ADC
 DDRB|=(1<<PB2);  // PB2 as output for LED indication
 DDRB|=(1<<PB4);  // PB4 as output for signal pin for sink
 PORTB&=~(1<<PB4);// Disable pull-up on PB4
}

void adc_setup()
{
 ADCSRA|=(1<<ADEN);   // Enable ADC module
 ADMUX|=(1<<0)|(1<<1)|(1<<REFS1);   // Select ADC3 channel for conversion
 ADMUX&=~(1<<REFS2);    // and
 ADMUX&=~(1<<REFS0);    // select the internal 1.1V voltage reference
 ADCSRB=0x00;   // Set up ADC in free running mode
 ADCSRA|=(1<<ADSC)|(1<<ADATE);    // Start ADC conversion and enable auto trigger
}

int main()
{
 pin_setup();
 adc_setup();
 while(1)
 {
 int adc_l=ADCL;                // Read ADCL register
 int adc_val=(ADCH<<8)|adc_l;   // Read ADCH register and combine data
 if (adc_val>TRIGGER)
  {
   DDRB&=~(1<<PB4);   // Set up PB4 as input (open-drain) for Signal pin
   PORTB|=(1<<PB2);   // Turn LED on
   adc_val=0;         // Reset ADC value
   _delay_ms(DELAY);  // A small delay for bounce settling
   PORTB&=~(1<<PB2);  // Turn LED off
   DDRB|=(1<<PB4);    // Set up PB4 as output for (sinking)
  }
 }
 return 0;
}
