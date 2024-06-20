#ifndef TIMER_INTERRUPT_H
#define TIMER_INTERRUPT_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

void handle_timer_interrupt(int signum);

#endif  // TIMER_INTERRUPT_H