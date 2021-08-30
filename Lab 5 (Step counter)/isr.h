#ifndef ISR_H_
#define ISR_H_

void readAccel(void *arg);

void stepCalculator(void *arg);

void button_isr_handler(void *arg);

void led_handler(void *arg);

#endif 