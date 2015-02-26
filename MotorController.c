/*
 * MotorController.c
 *
 *  Created on: Jan 6, 2015
 *      Author: woot
 */

#include "MotorController.h"

bool isRunning = TRUE;
static int robotSpeed = 100;
static int session;

// motor 1 and 2
motor_t m1 = {1, 0, 0, FALSE };
motor_t m2 = {2, 0, 0, FALSE };

//timer
static clock_t _current_time = 0;
void *(*p_start_timer)(void *);

/* MAIN WITH UART * */
int main(int argc, char *argv[]){
	unsigned int command = atoi(argv[1]);
	if (command == 1) {
		//exit
		return 0;
	}

	initMotors();
	printf("---motors initialized---\n");

	initUART();
	printf("---UART initialized---\n");

	gameLoop();
    return 0;
}

void initMotors() {
	runCommand(3, 0);
}

void gameLoop() {
	while (isRunning) {
		lc_protocol_t proc = getCommand();
		printf("command received\n");
		runCommand(proc.command, proc.parameter);
	}
	stop();
	closeUART();
}

void runCommand(unsigned int command, unsigned long param) {
	session = DMCCstart(BOARD_NUM);
	//heartbeatReceived();

	switch (command) {
	    case 0:
	    	printf("heartbeat received \n");
	    	break;
	    case 1:
	    	goForward(); printf("forward\n"); break;
	    case 2:
	    	goBackward(); printf("backward\n"); break;
	    case 3:
	    	stop(); printf("stop\n"); break;
	    case 4:
	    	turnRightStatic(); printf("turn right static\n"); break;
	    case 5:
	    	turnLeftStatic(); printf("turn left static\n"); break;
	    case 6:
	    	turnRightDynamic(param); printf("turn right dynamic; degree: %lu \n", param); break;
	    case 7:
	    	turnLeftDynamic(param); printf("turn left dynamic; degree: %lu \n", param); break;
	    case 8:
	    	if (isValidSpeed(param) == TRUE) {
	    		robotSpeed = MAX_ROBOT_SPEED/100*param;
	    		printf("new speed value %d \n", robotSpeed);
	    	}
	    	break;
	    case 9:
	    	isRunning = FALSE; break;
	    default:
	    	printf("not supported command \n");
	    	printUsage();
	    	break;
	}

	DMCCend(session);
}

void heartbeatReceived() {
	/*
	pthread_t thread_id;
	int milliseconds = 3000; // pwm range 1.000-10.000

	p_start_timer = &startTimer;
	int rc = 0;

	rc = pthread_create(&thread_id, NULL, startTimer, (void*)milliseconds);

	if(rc)printf("failed to create thread\n");

	pthread_join(thread_id,NULL);
	*/
}

void printUsage(){
    printf("usage: ./MotorController <command> <param>\n");
    printf("       <commands> 0 (Heartbeat), 1 (forward), 2 (reverse), 3 (stop), 4 (turn right static), 5 (turn left static), 6 (turn right dynamic), 7 (turn left dynamic), 8 (speed setting)\n");
    printf("	   <params> case when rotation: 0- 360 degrees; case when speed: 0-100 percent \n");
    printf("example: ./MotorController 0 \n");
}

int isValidRotation(int degree) {
	if (degree >= 0 && degree <= 360) return TRUE;
	return FALSE;
}

int isValidSpeed(int speed){
	if (speed >= 0 && speed <= 100) return TRUE;
	return FALSE;
}

void stop() {
	m1.targetPwmValue = 0;
	m2.targetPwmValue = 0;
	m1.curPwmValue = m1.targetPwmValue;
	m2.curPwmValue = m2.targetPwmValue;
	setMotorPower(session, 1, 0);
	setMotorPower(session, 2, 0);
}

void goForward() {
	safeRelease();
	m1.targetPwmValue = MAX_ROBOT_SPEED/100*robotSpeed;
	m2.targetPwmValue = MAX_ROBOT_SPEED/100*robotSpeed;
	linearAcceleration();
}

void goBackward() {
	safeRelease();
	m1.targetPwmValue = MAX_ROBOT_SPEED/100*robotSpeed * (-1);
	m2.targetPwmValue = MAX_ROBOT_SPEED/100*robotSpeed * (-1);
	linearAcceleration();
}

void linearAcceleration() {
	int stepResolution = MAX_ROBOT_SPEED/100*10;
	while (m1.curPwmValue != m1.targetPwmValue || m2.curPwmValue != m2.targetPwmValue) {
		if (m1.curPwmValue > m1.targetPwmValue) {
			m1.curPwmValue -= stepResolution;
		} else {
			m1.curPwmValue += stepResolution;
		}

		if (m2.curPwmValue > m2.targetPwmValue) {
			m2.curPwmValue -= stepResolution;
		} else {
			m2.curPwmValue += stepResolution;
		}

		setMotorPower(session, 1, m1.curPwmValue);
		setMotorPower(session, 2, m2.curPwmValue);
		delay(500000);
	}
}

void safeRelease() {
    stop();
    delay(50000);
}

void delay(int t){
	volatile int delayTime = t;
	volatile int i = 0;
	while (i < delayTime) {
		i++;
	}
}

void turnLeftStatic() {
	safeRelease();
	m1.targetPwmValue = MAX_ROBOT_SPEED/100*robotSpeed * (-1);
	m2.targetPwmValue = MAX_ROBOT_SPEED/100*robotSpeed;

	pthread_t thread_id;
	int seconds = DEGREES_PER_SEC / m2.targetPwmValue * 90; // pwm range 1.000-10.000
	printf("timer seconds %d\n", seconds);
	p_start_timer = &startTimer;
	int rc = 0;

	rc = pthread_create(&thread_id, NULL, startTimer, (void*)seconds);
	if(rc)printf("failed to create thread\n");

	pthread_join(thread_id,NULL);
	goForward();
}

void turnRightStatic() {
	safeRelease();
	m1.targetPwmValue = MAX_ROBOT_SPEED/100*robotSpeed;
	m2.targetPwmValue = MAX_ROBOT_SPEED/100*robotSpeed * (-1);

	pthread_t thread_id;
	unsigned int seconds = DEGREES_PER_SEC/ m1.targetPwmValue * 90; // pwm range 1.000-10.000
	printf("timer seconds %d\n", seconds);
	p_start_timer = &startTimer;
	int rc = 0;

	rc = pthread_create(&thread_id, NULL, startTimer, (void*)seconds);
	if(rc)printf("failed to create thread\n");

	pthread_join(thread_id,NULL);
	goForward();
}

void turnLeftDynamic(unsigned long degree) {
	safeRelease();
	int pwm = MAX_ROBOT_SPEED/100*robotSpeed;
	if (degree > 10) {
		degree = 10;
	}
	m1.targetPwmValue = pwm -(100*degree);
	m2.targetPwmValue = pwm;
	linearAcceleration();
}

void turnRightDynamic(unsigned long degree) {
	safeRelease();
	int pwm = MAX_ROBOT_SPEED/100*robotSpeed;
	if (degree > 10) {
		degree = 10;
	}
	m1.targetPwmValue = pwm;
	m2.targetPwmValue = pwm - (100*degree);
	linearAcceleration();
}

void* startTimer(void* secs)
{
	volatile int i = 1;
    int seconds = (int)secs;
    _current_time = clock() + seconds * sysconf(_SC_CLK_TCK);	//CLOCKS PER SECOND

    linearAcceleration();

    while(clock() < _current_time){
    	//printf("run turn static..\n");
    	i++;
    }

    printf("Amount of ticks %d\n", i);
    pthread_exit(NULL);
    return NULL;
}
