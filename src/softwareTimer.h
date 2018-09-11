#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H

#define MAX_TIMERS  10
#define MAX_TIME	4000	/* maximo tiempo posible */
typedef  unsigned long TIME;		/* como se va a almacenar el tiempo */

typedef struct s_timer {
	uint8_t inuse;		/* TRUE if in use */
	TIME original_time;
	TIME time;		/* relative time to wait */
	char *event;		/* set to TRUE at timeout */
}s_timer;

class Stimer{
	private:
    s_timer* st;
		char* evento;				/* evento a notificar */
		TIME tiempo;        /* tiempo a setear */
	public:
    Stimer(TIME time, char* evento);
    Stimer(char* evento);
		Stimer();
		void start(TIME t); /* arranca el timer*/
    void borrar(); 			/* borra el timer*/
		char timeout_flag;       /* evento propio */
};



// extern s_timer timers[MAX_TIMERS]; /* arreglo de timers */
extern s_timer *timer_next; /* debe ser extern para que lo utilice la interrupcion */
extern volatile TIME time_timer_set;
extern volatile TIME time_now;
void timers_update(TIME t);
#endif
