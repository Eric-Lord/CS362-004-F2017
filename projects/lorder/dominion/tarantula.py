#!/usr/bin/env python

#Eric Lord
#CS 362 Tarantula For Dominion
#December 8, 2017
#
from __future__ import division
import subprocess

class colors:
    BLUE = '\033[94m'
    GREEN = '\033[32m'
    RED = '\033[91m'
    YELLOW = '\033[33m'
    ENDCOLOR = '\033[0m'

# The following functions are used to print the lines in color
def print_green_text(text):
    print colors.GREEN + text + colors.ENDCOLOR

def print_yellow_text(text):
    print colors.YELLOW + text + colors.ENDCOLOR

def print_red_text(text):
    print colors.RED + text + colors.ENDCOLOR

def get_result():
    file = open("testOutput.txt","r")

    #read from the file
    status = file.read(6)

    # close the file when done
    file.close()

    if status == "FAILED":
        return 0
    else:
        return 1


# Function to calculate how suspicious a line of code is
# Param input: passed - number tests passed that execute the statement
# Param input: failed - number tests failed that execute the statement
# Param input: total_passed - number tests failed that execute the statement
# Param input: total_failed - number tests failed that execute the statement
# Param output: how suspicious a statement is
def suspiciousness(passed, failed, total_passed, total_failed):
    if total_passed == 0 and total_failed == 0:
        return 0.0
    if total_failed == 0:
        return 0.0
    elif total_passed == 0:
        return 1.0
    else:
        numerator = failed / total_failed
        denominator = passed / total_passed + numerator
    return numerator/denominator

def main():
    
     # source file is the dominion file
    source = 'dominion.c'

    # number of lines in the source file
    line_count = sum(1 for line in open(source))

    # create an array to hold number of passing/failing tests for each line
    # initialize the arrays to zero for each line number
    arrPassed = [0 for i in range(line_count + 1)]
    arrFailed = [0 for i in range(line_count + 1)]

    # tests to be run are the four unit and the four card tests from previous assignments
    unit_tests = ['unittest1', 'unittest2', 'unittest3', 'unittest4']
    card_tests = ['cardtest1', 'cardtest2', 'cardtest3', 'cardtest4']
    all_tests = unit_tests + card_tests

    # variables for total passed/failed tests
    total_passed = 0
    total_failed = 0

    # run all tests
    for test in all_tests:
        # call make clean to remove any compiled code and gcov files
        subprocess.call(['make', 'clean'])
        subprocess.call(['make', test])
        subprocess.call(['./' + test])

        result = get_result()
        # increment the total passed or failed depending on the test result
        if (result == 1):
            total_passed += 1
        else:
            total_failed += 1
        
        # run gcov on source file
        subprocess.call(['gcov', source])

        # go through output line by line, recording whether the lines were executed
        # split on delimiter : symbol to the left indicates the line execution
        # number on the right indicates the line number
        f = open(source + '.gcov', 'r')
        for line in iter(f):
            line_parts = line.split(':')
            line_execution = line_parts[0].lstrip()
            line_number = int(line_parts[1])

            if line_execution.startswith('-') or line_execution.startswith('#'):
                # lines that start with - or # are not executed
                continue
            elif result == 1:
                # line executed by passing test
                arrPassed[line_number] += 1
            else:
                # line executed by failing test
                arrFailed[line_number] += 1
        f.close()

    line_counter = 1
    # go through source file line by line printing in color representing suspiciousness
    file_out = open('tarantula.out','w') 
    f = open(source, 'r')
    for line in iter(f):
        # take the entire line except the last character which is the newline and add line number
        line_text = str(line_counter) + ':' + line.rstrip('\n')
        # check if line was a part of any passing or failing tests
        if arrPassed[line_counter] == 0 and arrFailed[line_counter] == 0:
            # no test executed this line
            print line_text
            file_out.write('   ' + line_text + '\n') 
        else:
            suspicious = suspiciousness(arrPassed[line_counter], arrFailed[line_counter], total_passed, total_failed)
            
            if suspicious == 0.0:
                # least suspicious
                print_green_text(line_text)
                file_out.write('#  ' + line_text + '\n') 
            elif suspicious <= 0.5:
                # moderately suspicious
                print_yellow_text(line_text)
                file_out.write('## ' + line_text + '\n') 
            else:
                # most suspicious
                print_red_text(line_text)
                file_out.write('###' + line_text + '\n') 

        #increment line counter
        line_counter += 1
    
    f.close()
    file_out.close()

if __name__ == '__main__':
    main()