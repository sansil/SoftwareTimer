#include "hardwareTimer.h"
#include "softwareTimer.h"

s_timer *timer_next = NULL;		/* timer we expect to run down next */
volatile TIME time_timer_set; /* time when hardware timer was set */
volatile TIME time_now;				/* Almacena el tiempo del core*/
s_timer timers[MAX_TIMERS];
s_timer *timer_declare(TIME time, char *event);
void timer_undeclare(s_timer *t);
void timers_init();
/**
 *  Con este constructor se inicia el timer automaticamente sin llamar a start
 */
Stimer::Stimer(TIME time, char *evento)
{
	Stimer::evento = evento;
	Stimer::st = NULL;
	Stimer::callback = NULL;
	Stimer::tiempo = time;
}

Stimer::Stimer(char *evento)
{
	Stimer::evento = evento;
	Stimer::st = NULL;
	Stimer::callback = NULL;
	Stimer::tiempo = 0;
}

Stimer::Stimer()
{
	Stimer::evento = NULL;
	Stimer::st = NULL;
	Stimer::callback = NULL;
	Stimer::tiempo = 0;
}

void Stimer::start(TIME t, void (*pfun)(void *payload))
{
	if (t != 0)
	{
		/* Creo la instancia de softwareTimer */
		Stimer::tiempo = t;
		Stimer::callback = pfun;
		Stimer::timeout_flag = 0;
		if (Stimer::evento == NULL)
		{
			Stimer::st = timer_declare(Stimer::tiempo, (char *)&timeout_flag);
		}
		else
		{
			*Stimer::evento = 0;
			Stimer::st = timer_declare(Stimer::tiempo, (Stimer::evento));
		}
	}
	else if (Stimer::tiempo != 0)
	{
		if (Stimer::evento == NULL)
			Stimer::st = timer_declare(Stimer::tiempo, (char *)&timeout_flag);
		else
			Stimer::st = timer_declare(Stimer::tiempo, (Stimer::evento));
	}
}

void Stimer::start(TIME t = 0)
{
	if (t != 0)
	{
		/* Creo la instancia de softwareTimer */
		Stimer::tiempo = t;
		//Stimer::callback = NULL;
		Stimer::timeout_flag = 0;
		Stimer::st = timer_declare(Stimer::tiempo, (char *)&timeout_flag);
		// Serial.println("start timer");
		// Serial.println(t);
		// Serial.println("*******");
	}
	else if (Stimer::tiempo != 0)
	{
		//Serial.println("error");
		if (Stimer::evento == NULL)
			Stimer::st = timer_declare(Stimer::tiempo, (char *)&timeout_flag);
		else
			Stimer::st = timer_declare(Stimer::tiempo, (Stimer::evento));
	}
}

void Stimer::start()
{
	if (Stimer::tiempo != 0)
	{
		if (Stimer::evento == NULL)
			Stimer::st = timer_declare(Stimer::tiempo, (char *)&timeout_flag);
		else
			Stimer::st = timer_declare(Stimer::tiempo, (Stimer::evento));
	}
}

void Stimer::borrar()
{
	if (Stimer::st != NULL)
	{
		timer_undeclare(Stimer::st);
	}
}

void Stimer::begin()
{
	timers_init();
}

void timers_init()
{
	/*
  * init timers in inuse
  */
	s_timer *t;
	for (t = timers; t < &timers[MAX_TIMERS]; t++)
		t->inuse = false;
}

void timer_undeclare(s_timer *t)
{
	cli();
	if (!t->inuse)
	{

		sei();
		return;
	}
	t->inuse = false;
	/* check if we were waiting on this one */
	if (t == timer_next)
	{
		time_now = update_time_now();
		timers_update(time_now - time_timer_set);
		if (timer_next)
		{
			start_hardware_timer(timer_next->time);
			time_now = update_time_now();
			time_timer_set = time_now;
		}
	}
	sei();
}

/* subtract time from all timers, enabling any that run out along the way */
void timers_update(TIME time)
{

	/* struct temporal por si ni se encuentra un timer disponible*/
	static s_timer timer_last = {
			false /* in use */,
			30000 /* time original*/,
			30000 /* time */,
			NULL /* event pointer */
	};

	s_timer *t;

	timer_next = &timer_last;

	for (t = timers; t < &timers[MAX_TIMERS]; t++)
	{
		if (t->inuse)
		{
			if (time < t->time)
			{ /* unexpired */
				t->time -= time;
				t->original_time -= time;

				if (t->time <= timer_next->time)
					timer_next = t;
				if (timer_next->time > MAX_TIME)
					timer_next->time = MAX_TIME;
			}
			else if (time >= t->time && time < t->original_time)
			{
				t->original_time -= time;
				t->time = t->original_time;
				if (t->time <= timer_next->time)
					timer_next = t;
				if (timer_next->time > MAX_TIME)
					timer_next->time = MAX_TIME;
			}
			else
			{ /* expired */
				/* tell scheduler */
				*t->event = 1;
				//Serial.println("timer expired");
				//Serial.println("****");
				t->inuse = 0; /* remove timer */
			}
		}
		else
		{
		}
	}

	/* reset timer_next if no timers found */
	if (!timer_next->inuse)
		timer_next = 0;
}

s_timer *timer_declare(TIME time, char *event)
{
	s_timer *t;
	cli();
	for (t = timers; t < &timers[MAX_TIMERS]; t++)
	{
		if (!t->inuse)
		{
			break;
		}
	}
	/* out of timers? */
	if (t == &timers[MAX_TIMERS])
	{
		sei();
		Serial.println(F("[ST]: Out of timers"));
		return (0);
	}
	/* install new timer */
	t->event = event;
	t->time = time;
	t->original_time = time;
	time_now = update_time_now();
	if (!timer_next)
	{
		/* no timers set at all, so this is shortest */
		time_timer_set = time_now;
		timer_next = t;
		if (timer_next->time > MAX_TIME)
		{
			timer_next->time = MAX_TIME;
			//timer_next->original_time =- MAX_TIME;
		}
		start_hardware_timer(timer_next->time);
	}
	else if ((time + time_now) < (timer_next->time + time_timer_set))
	{
		/* new timer is shorter than current one, so */
		timers_update(time_now - time_timer_set);
		time_timer_set = time_now;
		timer_next = t;
		if (timer_next->time > MAX_TIME)
		{
			timer_next->time = MAX_TIME;
			//timer_next->original_time -= MAX_TIME;
		}
		start_hardware_timer(timer_next->time);
	}
	else
	{
		/* new timer is longer, than current one */
	}
	t->inuse = true;
	sei();
	return (t);
}
