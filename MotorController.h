/*
 * MotorController.h
 *
 *  Created on: Jan 7, 2015
 *      Author: woot
 */

#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include "DMCC.h"
#include "uart_processing.h"
#include "lc_protocol.h"
#include <pthread.h>
#include <time.h>
#include "stopwatch_timer.h"

#define quadrat(x) x*x

#define BOARD_NUM 0
#define MAX_ROBOT_SPEED 3200 //pwm parameter

#define TRUE 1
#define FALSE 0

#define DEGREES_PER_SEC 560000
#define HEARTBEAT_TIMER 2000 //in ms

typedef int bool;

typedef struct {
	int id;
	int curPwmValue;
	int targetPwmValue;
	bool isAccelerating;
} motor_t;

void initMotors();
void gameLoop();
void runCommand(unsigned int command, unsigned long param);
void heartbeatReceived();
void printUsage();
int isValidRotation(int degree);
int isValidSpeed(int speed);
void stop();
void goForward();
void goBackward();
void turnLeftStatic();
void safeRelease();
void turnRightStatic();
void turnLeftDynamic(unsigned long degree);
void turnRightDynamic(unsigned long degree);
void linearAcceleration();
void* startTimer(void* secs);
void delay(int t);

#endif /* MOTORCONTROLLER_H_ */
