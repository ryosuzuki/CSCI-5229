# CSCI-5229 Computer Graphics


## Checklist

Before submitting your homeworks to moodle, please check the following:

1)  The upload must contain everything I need, but only what I need.  Please do make clean before uploading because I will do a make clean and delete those anyway.

2)  Make sure that your files unzip in the base directory.  OSX users beware!
For Mac users
```
zip 'name of zipped archive' 'file1' 'file2' ... 'filen'
```
For example, 
```
zip hw2.zip hw2.o hw2.c makefile README
```
then when you unzip with:
```
unzip hw2
```

3)  Make sure you provide a working makefile for Linux.

4)  Make sure your executable is named hw3 (for this week).  Note that hw is lower case.

Moodle puts each person's homework in a separate directory for me, so I want to simply do this:

for dir in *;do (cd $dir;make clean;make;hw3);done

This will build and show me the homeworks of everybody who followed the instructions.

5)  Make sure that when your program starts, the state is initialized in such a way that I see what you want me to see right away.  Also provide a way to exit your program (ESC is what I use).

6)  Include the hours you spent in the README, not a separate file.  if you want me to pay ettention to something, make sure that it is prominently mentioned in the README.  Call the file README all upper case.

7)  Make sure that your name is displayed on the window frame.

8)  Make sure your program will compile cleanly on gcc-5.4 which is the compiler on Ubunutu 16.04.  If you are unsure what that means, use the graphics machine in CSEL to test your homework.  Specifically watch out for features language features from C11 or C14 that is not enabled by default on this compiler and may cause compiler warning or errors.  clang and other gcc versions may have different defaults.

9)  Make sure your homework will impress your friends.  In case it does impress your friends make sure you attend class because I may show your homework.
