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