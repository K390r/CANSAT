#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t rx0_receive, rx0_transceive,adc_receive;
volatile float rx0_data, ADCdata;

/*ISR(USART0_RX_vect){
	rx0_receive = 1;
	rx0_data = UDR0;
}*/

ISR(USART1_UDRE_vect){
	if(rx0_transceive){
		UDR1=ADCdata;
		rx0_transceive=0;
	}
}

ISR(ADC_vect){
	adc_receive = 1;
	ADCdata = ADCW;
	ADCSRA = ADCSRA | 0x40;
}

void USART_INIT(){
	UBRR0H = 0;
	UBRR0L = 51;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C = (3<<UCSZ00);
}

void ADC_INIT(){
	ADMUX = 0x00;
	ADCSRA = 0b11001110;
}

int main(){
	rx0_receive = 0;
	sei();
	ADC_INIT();
	USART_INIT();
	while(1){
		if(adc_receive){
			adc_receive = 0;
			ADCdata=(ADCdata*0.0049+5.1*0.095)/(5.1*0.009);
			rx0_transceive = 1;
		}
	}
	return 0;
}
