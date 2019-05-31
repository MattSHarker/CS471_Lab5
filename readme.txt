
**************************** ABOUT ****************************
This is a program created by Matthew Harker for his fifth project for his
CS471 Optimization class. All work in this project is his own, with the
exception of the ThreadPool.h file, which was used with permission from
Jakob Progsch and Václav Zeman.


This program showcases three different flowshop scheduling algorithms:
    > basic flowshop scheduling
    > flowshop scheduling with blocking
    > flowshop scheduling with no wait


************************* DATA FILES **************************
This program will read the flowshop data from various files. All of these
files can be found in the DataFiles directory. These files follow the naming
convention of "<unsigned_integer>.txt", so if you want to add or modify any
files, they must follow this naming convention. A file with the name "5392.txt"
will be properly ran, while a file name "data.txt" will not.

The files contains two different types of information. The first line has
two numbers; the first being the number of columns in the dataset, the second
being the number of columns in the dataset.
    The second type of information are the values that will represent the time
each job will take on each machine. These must be integers as the program is
currently not set up to take in other types of data.


************************ PARAMETERS **************************
There are two parameter files included in this program. The first file,
"parameters.txt", contains three different lines of parameters.
    The first two lines are the first file to process and the last file to
process. The program process every job inbetween as well, so keep this in mind
when running mulitiple files.
    The third line determines which algorithms the files will be ran through.
There are four allowable values for this: 0, 1, 2, and 3.
    0: Runs all three algorithms
    1: Runs the FSS algorithm
    2: Runs the FSSB algorithm
    3: Runs the FSSNW algorithm

The second parameter file, "custPerm.txt", is used to run a single datafile
through one algorithm using a custom job sequence. This file has five lines:
the first line determines whether the user wants to run a customer sequence
or not. A value of "1" will run a customer sequence whereas any other value
will run all sequences.
    The second line contains the name of the datafile to be ran. The structure
of this datafile must follow the same structure for files as detailed earlier.
The exception to this rule is that the name of the file can be anything.
    The third line determines which algorithm can be used. The allowable values
are 1, 2, and 3, and are the same as the numbers in the other parameter file.
    The fourth line determines how many jobs there are in the datafile, and it
must match the second value of the first line of the datafile itself.
    The fifth line contains the customer sequence itself. There must be the same
quantity of values as the number in line four of the parameter file. The
program does not check to see if the sequence contains all consecutive numbers
from 1-n, so sequences with repeating or missing numbers will output values that
would be otherwise impossible to produce.


*************************** RESULTS ***************************
After the program finishs running, the data files in the results directory will
be updated with the most recent values. There are two main sub-directories, one
all of the data in a human readable format called "rawData", and one with the 
start and end times formatted to be read in by scripts. For general use, the files
in rawData will be easier to understand.


*************** BUILDING, RUNNING, AND CLEANING ***************
In order to run the program, the system must first be built:
    In a terminal, cd to the directory this readme is in,
    Execute the following command:
        ./build.sh
    
Once the system is built, it can be ran at any time:
    cd to this directory, and execute the following command:
        ./run.sh

How to remove the build files:
    cd to this directory, and execute the following command:
    $ ./clean.sh


**************************** NOTES *****************************
This program was developed on the Ubuntu 18.04 operating system.
The code was developed on the Visual Studio Code text editor
The CPU of the primary computer used was an Intel Core i5-7300HQ.
