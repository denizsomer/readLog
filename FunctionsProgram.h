
#include <fstream>
#include <iostream>
#include <ctime>
#include "MyString/MyString.h"

#ifndef incl_FunctionsProgram_h
#define incl_FunctionsProgram_h


typedef unsigned char uint8_t;
typedef uint8_t boolean;
typedef uint8_t byte;


		void connectDevice(void);
		void readDeviceIdentification(void);
		void readDeviceSerial(void);
		void readLog(void);
		void getAnalogStatus(void);
		void getTristarLog(bool);
		int getLogCSerial(void);
		int testCSerial(void);
		void getLogRealTerm(char *);
		void delay(unsigned int);

		void getStatus(byte);
		void printStatus(void);
		void calcCrc(void);

		byte* PackBytes(byte* , int, int&);
		byte* UnpackBytes(byte* , int, int&);
		void testPDU();

// functions in program
int processMessagefromTristar(void);
//int processMessagefromTristar(int, int*);
int charToInt(char c);
int processTristarLog(char *);
void       prgError                      (int, char*, char*);
void       prgWarning                    (int, char*, char*);


#endif

extern bool debug;

