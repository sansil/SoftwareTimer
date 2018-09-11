#include "hardwareTimer.h"

void start_hardware_timer(TIME unTime){
 /*
  * Seteo el timer en multiplo de 1seg.
  * unTime esta en ms.
  * prescaler 8  max_ms : 32 ms
  * prescaler 64 max_ms : 262ms
  * prescaler 256 max_ms : 1048 ms
  * prescaler 1024 max_ms : 4164 ms
  */


  TIME ticks=0;
  float prescaler=1;
  cli();

  if ((unTime>=0) & (unTime <= 32)){
    prescaler = 8.0;
  }else if((unTime>=33) & (unTime <= 262)){
    prescaler = 64.0;
  }else if ((unTime>=263) & (unTime <= 1048)){
    prescaler = 256.0;
  }else if((unTime>=1049) & (unTime <= 4164)){
    prescaler = 1024.0;
  }else{
    prescaler = 1024.0; //TODO control de error
  }
//  ticks = round((unsigned long)((float)unTime*15624)/1000); //1s 15624 ticks max value 65535
  //Serial.println(ticks);
 			// disable global interrupts
  TCCR1A = 0;		// set entire TCCR1A register to 0
  TCCR1B = 0;		// same for TCCR1B
  //OCR1A = ticks;
  OCR1A = round((16000.0f/prescaler)*unTime - 1);
  	// turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  	// Set CS10 and CS12 bits for 1024 prescaler:
  	switch ((int)prescaler) {
  	  case 8:
        TCCR1B |= (1 << CS11);
        break;
      case 64:
        TCCR1B |= (1 << CS10);
        TCCR1B |= (1 << CS11);
        break;
      case 256:
        TCCR1B |= (1 << CS12);
        break;
      case 1024:
      default:
        TCCR1B |= (1 << CS10);
        TCCR1B |= (1 << CS12);
        break;
  	}
  	// enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  	// enable global interrupts:
  sei();
}

ISR(TIMER1_COMPA_vect) {
 /*
  * En la interrupcion hago un update con el timepo que expiro y seteo un nuevo timer con el valor de timer_next( si es que hay uno)
  */
  TCCR1A = 0;
  TCCR1B = 0;  //apago el timer sino queda ciclico por defecto
  time_now = update_time_now();
  timers_update(time_now - time_timer_set); // actualizo el nuevo timer_next
    /* start physical timer for next shortest time if one exists */
  if (timer_next) {
      time_timer_set = time_now;
      start_hardware_timer(timer_next->time);

  }
}

TIME update_time_now(){
  return millis();
}
