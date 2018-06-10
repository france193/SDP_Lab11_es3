/**
* Name:    Francesco
* Surname: Longo
* ID:      223428
* Lab:     11
* Ex:      3
*
* A one-way bridge is approached by cars in both directions, but being
* one-way it must follow three rules:
* - Only cars from one direction can traverse the bridge in each moment,
*   either the ones moving from left-to-right or the one running from
*   right-to-left.
* - There can be multiple cars going in the same direction.
* - If cars from the opposite direction arrive while there are cars on
*   the bridge, those cars in the opposite direction must wait till the
*   bridge is cleared.
*
* Write a Windows program to synchronize all cars traversing the bridge.
* More specifically, the program must run two threads.
* The first thread is in charge of all cars moving from left-to-right.
* The second one is in charge of all cars moving from right-to-left.
* Each one of the previous two threads run one extra thread to represent
* each car.
*
* Notice that:
* - Cars running from left-to-right arrive at random time intervals,
*   varying from 0 to time_A_L2R (integer value) seconds.
* - Cars running from right-to-left arrive at random time intervals,
*   varying from 0 to time_A_R2L (integer value) seconds.
* - All cars traversing the bridge from left-to-right need time_T_L2R
*   (integer value) seconds
* - All cars traversing the bridge from right-to-left need time_T_R2L
*   (integer value) seconds
* - The program has to terminate only once exactly
*   - number_L2R (integer value) cars have traversed the bridge from
*     left-to-right
*   - number_R2L (integer value) cars have traversed the bridge from
*     right-to-left.
*
* The program must accepts 6 parameters on the command line, namely
* 6 integers:
* time_A_L2R, time_A_R2L, time_T_L2R, time_T_R2L, number_L2R,
* number_R2L.
*
* Suggestion
* -----------
*
* Referring to the classical Reader and Writer logical scheme modify it
* to manipulate two sets of Readers.
*
**/

// !UNICODE
#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif

// !_CRT_SECURE_NO_WARNINGS
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif 

// include
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>

// prototypes
// sem_L2R is a semaphore for cars traversing from left to right
// sem_R2L is a semaphore for cars traversing from right to left
// sem_bridge is a semaphore that avoids having cars in both directions on the bridge
HANDLE sem_L2R, sem_R2L, sem_bridge;
// counter variables for each direction (WAITING or PASSING)
DWORD cars_L2R, cars_R2L;
DWORD timeL2R, timeR2L, timeARRIVAL, nL2R, nR2L;
DWORD WINAPI L2R_handler(LPVOID);
DWORD WINAPI R2L_handler(LPVOID);
LPWSTR getErrorMessageAsString(DWORD errorCode);
int Return(int seconds, int value);

// main
INT _tmain(INT argc, LPTSTR argv[]) {
	LPHANDLE threadsL2R, threadsR2L;
	LPDWORD idCarsL2R, idCarsR2L;
	DWORD i;

	// check input parameters
	if (argc < 6) {
		_ftprintf(stderr, _T("Usage: %s timeL2R, timeR2L, timeARRIVAL, nL2R, nR2L\n"), argv[0]);
		return 1;
	}

	timeL2R = _ttoi(argv[1]);
	timeR2L = _ttoi(argv[2]);
	timeARRIVAL = _ttoi(argv[3]);
	nL2R = _ttoi(argv[4]);
	nR2L = _ttoi(argv[5]);

	_ftprintf(stdout, _T("timeL2R:%u, timeR2L:%u, timeARRIVAL:%u, nL2R:%u, nR2L:%u\n\n"),
		timeL2R, timeR2L, timeARRIVAL, nL2R, nR2L);

	// semaphores required
	sem_L2R = CreateSemaphore(NULL, 1, nL2R, NULL);
	sem_R2L = CreateSemaphore(NULL, 1, nR2L, NULL);
	sem_bridge = CreateSemaphore(NULL, 1, 1, NULL);

	if (sem_L2R == NULL || sem_R2L == NULL || sem_bridge == NULL ) {
		_ftprintf(stderr, _T("Error semaphore:%x\n"), GetLastError());
		return 2;
	}

	cars_L2R = 0; cars_R2L = 0;

	threadsL2R = (LPHANDLE)malloc(nL2R * sizeof(HANDLE));
	idCarsL2R = (LPDWORD)malloc(nL2R * sizeof(DWORD));
	threadsR2L = (LPHANDLE)malloc(nR2L * sizeof(HANDLE));
	idCarsR2L = (LPDWORD)malloc(nR2L * sizeof(DWORD));

	if (threadsL2R == NULL || idCarsL2R == NULL || threadsR2L == NULL || idCarsR2L == NULL) {
		_ftprintf(stderr, _T("Error malloc:%x\n"), GetLastError());
		return 2;
	}

	// create threads for cars that travel from left to right
	// no more than nR2L cars can pass from left to right
	for (i = 0; i < nL2R; i++) {
		idCarsL2R[i] = i;

		threadsL2R[i] = CreateThread(NULL, 0, L2R_handler, &idCarsL2R[i], 0, NULL);

		if (threadsL2R[i] == INVALID_HANDLE_VALUE) {
			_ftprintf(stderr, _T("Error thread creation:%x\n"), GetLastError());
			return 3;
		}
	}

	// create threads for cars that travel from right to left
	// no more than nR2L cars can pass from right to left
	for (i = 0; i < nR2L; i++) {
		idCarsR2L[i] = i;

		threadsR2L[i] = CreateThread(NULL, 0, R2L_handler, &idCarsR2L[i], 0, NULL);

		if (threadsR2L[i] == INVALID_HANDLE_VALUE) {
			_ftprintf(stderr, _T("Error thread creation:%x\n"), GetLastError());
			return 3;
		}
	}

	// main thread waits for completion of all threads
	WaitForMultipleObjects(nL2R, threadsL2R, TRUE, INFINITE);
	WaitForMultipleObjects(nR2L, threadsR2L, TRUE, INFINITE);

	free(threadsL2R);
	free(threadsR2L);

	_ftprintf(stdout, _T("\nPress enter to continue...\n"));
	_gettchar();

	return 0;
}

DWORD WINAPI L2R_handler(LPVOID param) {
	DWORD wait_time;
	LPDWORD idCarL2R;

	// get the id of the car (used only when printing on screen)
	idCarL2R = (LPDWORD)param;

	srand(GetCurrentThreadId()); // seed for random generator

	// Car in each direction arrive at random time interval, 
	// varying from 0 to timeARRIVAL seconds
	wait_time = (DWORD)(rand() % (timeARRIVAL + 1));
	Sleep(wait_time * 1000);

	WaitForSingleObject(sem_L2R, INFINITE);
	cars_L2R++;
	if (cars_L2R == 1) {
		WaitForSingleObject(sem_bridge, INFINITE);
	}
	ReleaseSemaphore(sem_L2R, 1, NULL);

	// start critical section
	_ftprintf(stdout, _T("Car L2R (%u/%u) is passing from left to right\n"), (*idCarL2R) + 1, nL2R);

	// All cars need timeL2R seconds to traverse the bridge from left to right
	Sleep(timeL2R * 1000);
	// end critical section

	WaitForSingleObject(sem_L2R, INFINITE);

	cars_L2R--;

	if (cars_L2R == 0) {
		ReleaseSemaphore(sem_bridge, 1, NULL);
	}

	ReleaseSemaphore(sem_L2R, 1, NULL);

	return 0;
}

DWORD WINAPI R2L_handler(LPVOID param) {
	DWORD wait_time;
	LPDWORD idCarR2L;

	// get the id of the car (used only when printing on screen)
	idCarR2L = (LPDWORD)param;

	srand(GetCurrentThreadId()); // seed for random generator

	// Car in each direction arrive at random time interval, 
	// varying from 0 to timeARRIVAL seconds
	wait_time = (DWORD)(rand() % (timeARRIVAL + 1));
	Sleep(wait_time * 1000);

	WaitForSingleObject(sem_R2L, INFINITE);
	
	cars_R2L++;
	
	if (cars_R2L == 1) {
		WaitForSingleObject(sem_bridge, INFINITE);
	}
	
	ReleaseSemaphore(sem_R2L, 1, NULL);

	// start critical section start
	_ftprintf(stdout, _T("Car R2L (%u/%u) is passing from right to left\n"), (*idCarR2L) + 1, nR2L);

	// All cars need timeR2L seconds to traverse the bridge from right to left
	Sleep(timeR2L * 1000);
	// end critical section

	WaitForSingleObject(sem_R2L, INFINITE);
	
	cars_R2L--;
	
	if (cars_R2L == 0) {
		ReleaseSemaphore(sem_bridge, 1, NULL);
	}
	
	ReleaseSemaphore(sem_R2L, 1, NULL);

	return 0;
}

int Return(int seconds, int value) {
	Sleep(seconds * 1000);
	return value;
}

LPWSTR getErrorMessageAsString(DWORD errorCode) {
	LPWSTR errString = NULL;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		0,
		errorCode,
		0,
		(LPWSTR)&errString,
		0,
		0);

	return errString;
}
