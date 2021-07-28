#ifndef FLASH_H_
#define FLASH_H_


void waitMs(unsigned int milllis);

void startFlash(int ms, int laps);

void lostFlash();

void winFlash();

void restart(unsigned int restartTime);

#endif