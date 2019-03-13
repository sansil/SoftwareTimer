/*
* Inspired from http://www.kohala.com/start/libes.timers.txt
*/

#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H

#define MAX_TIMERS 10
#define MAX_TIME 4000				/* max time */
typedef unsigned long TIME; /* time storage */

typedef struct s_timer
{
	uint8_t inuse; /* TRUE if in use */
	TIME original_time;
	TIME time;	 /* relative time to wait */
	char *event; /* set to TRUE at timeout */
} s_timer;

class Stimer
{
private:
public:
	s_timer *st;
	char *evento;								/* event to notify */
	TIME tiempo;								/* time to set */
	volatile bool timeout_flag; /* own event */
	Stimer(TIME time, char *evento);
	Stimer(char *evento);
	Stimer();
	void (*callback)(void *payload);
	void start();																		 /* start time with last time set*/
	void start(TIME t);															 /* start time with explicit time*/
	void start(TIME t, void (*pfun)(void *payload)); /* start time with function callback*/
	void borrar();																	 /* delete  timer*/
	void begin();
};

// extern s_timer timers[MAX_TIMERS]; /* arreglo de timers */
extern s_timer *timer_next; /* updated on interruption */
extern volatile TIME time_timer_set;
extern volatile TIME time_now;
void timers_update(TIME t);
#endif
