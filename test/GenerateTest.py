#! /usr/bin/env python
'''
Author: Lu Xu
Generate the header files including all the declarations of test functions,
a array of functions, a array of function names and the number of functions.
'''
import os

testh = 'Test.h'
testinfo = 'TestFunctions.c'
testc = '`ls test*`'

def readfuncs(filename):
    '''
    read files and find test function declarations or definitions.
    return a list of sorted function names.
    invoke linux command here, so it is not goint to work under Windows.
    '''
    # TODO: change to platform-independent
    regex = 'int +test[1-9|a-z|A-Z|_]* *\( *\)*'
    command = "awk '/%s/{print$2}' %s|sort|awk -F'(' '{print $1}'"  % (regex, filename)
    return os.popen(command).read().split()

def gentest(funcs):
    '''
    generate test header file and testfunctions source file
    '''
    description = \
'''/**
 * This file is automatically generated by GenerateTest.py, collecting function
 * information from source files.
 *
 * If you want to change the content of this file, go to GenerateTest.py and
 * modify the script. Or you can maintain this file by yourself after you delete
 * the related lines in the makefile.
 */
'''
    # The header file contains the declarations of all the test functions
    headercontent = description + \
'''
#ifndef _TESTDECLARATION_H_
#define _TESTDECLARATION_H_

#define PASSED 0
#define FAILED 1

extern int (*tests[])();    //the list of test functions which are listed below
extern char *names[];       //the list of test functions' names
extern int num;	            //the number of test functions

//declarations of testfunctions
%s
void testall();

#endif
''' % ''.join(['int %s();\n' % func for func in funcs])
    # the source file contains the definition of the function array and name
    # string array of all test functions and the number of test functions
    cfilecontent = description + \
'''
#include "Test.h"

int (*tests[])() = {
%s
};

char *names[] = {
%s
};

int num = %d;
''' %  (''.join(['\t%s,\n' % func for func in funcs]),
        ''.join(['\t\"%s\",\n' % func for func in funcs]),
        len(funcs))
    # write into files
    with open(testh, 'w') as header:
        header.write(headercontent)
    with open(testinfo, 'w') as cfile:
        cfile.write(cfilecontent)

def main():
    '''
    If the header file exists and the functions in it are the same as those in
    source file 'Test.c', then it is up to date. Otherwise update the header
    file and another source file.
    '''
    funclist = readfuncs(testc)
    if os.path.exists(testh) and funclist == readfuncs(testh):
        print 'File ' + testh + ' and ' + testinfo + ' are up to date.'
    else:
        gentest(funclist)
        print 'File ' + testh + ' and ' + testinfo + ' are (re)generated.'

if __name__ == '__main__':
    main()
