/*
** This file is automatically generated by GenerateTest.py.
*/

#ifndef _TESTDECLARATION_H_
#define _TESTDECLARATION_H_

#define PASSED 0
#define FAILED 1

extern int (*tests[])();    //the list of test functions which are listed below
extern char *names[];       //the list of test functions' names
extern int num;	            //the number of test functions

int testAdamsPECE();
int testAdaptiveSimpson();
int testBisection();
int testChasing();
int testClassicRK();
int testDividedDiff();
int testGaussianEli();
int testHermiteIpl();
int testLeastSq();
int testMuller();
int testNewtonMethod();
int testPicardRecurtion();
int testQR();
int testRKF();
int testRomberg();
int testSODERungeKutta();
int testSecent();
int testSplineIpl();
int testSteffensen();
int testall();

#endif