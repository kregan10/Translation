/*
  COMP2510 - Assignment 2
  Trans.c - takes a two strings from the command line, and an input file then
    translates characters from one string to the other string, and outputs the
    translation to an output file.
  Author:
                  Kerry Regan
  Student number:
                  A00915421
  Version:
                  1.0
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/*
  Max size of lines or arrays to be read
*/
#define MAXSIZE 512
/*
  First string entered on command line
*/
char *pattern = NULL;

/*
  Second string entered on command line
*/
char *toMatch = NULL;

/*
  Returns the range between two integers, or chars
*/
int rangeValue(int start, int end) {
  if(start > end) {
    perror("Invalid Range");
  }
    return end - start;
}

/*
  Builds an array from the given string, of starts, and ends of ranges.
  every even index of rangeArr is the starting character of the array,
  and every odd index is the range of the array.
*/
void getRanges(char string[], int rangeArr[]) {

  int stringLength;
  int i;
  int rangeIndx;
  char end;
  char start;
  char c;

  stringLength = strlen(string);
  rangeIndx = 0;

  for(i = 0; i < stringLength - 2; i++) {
    c = string[i];

    start = c;
    i++;
    c = string[i];
    if(c != '-') {
      i = i - 1;
      continue;
    }
    i++;
    c = string[i];
    end = c;
    if(rangeValue(start, end) >= 0) {
      rangeArr[rangeIndx] = start;
      rangeArr[rangeIndx + 1] = rangeValue(start, end);
      rangeArr[rangeIndx + 2] = i - 2;
      rangeIndx =+ rangeIndx + 3;
    } else if(c == '-') {
      string[i] = '-';
    }
  }
}
/*
  Returns 1 if the string passed in contains a range, 0 if it does not contain
  a range
*/
int containRange(char string[]) {

  int stringLength;
  int i;

  stringLength = strlen(string);

  for(i = 1; i < stringLength - 1; i++) {
    if(string[i] == '-')
      return 1;
  }
  return 0;
}
/*
  Go through string inputed, if -, keep in string, if not, check range array and
  fill string with range values.  Fills a range array with the first value of the
  array being the range, second the starting ASCII code of that range, and every
  third value is the value of the original index in the string for which the range
  came from.
*/
void fillStringRanges(char string[], int rangeArr[]) {

  int stringLength = strlen(string);
  char c;
  char temp[MAXSIZE];
  int i;
  int j;
  int tempIndx = 0;
  int rangeIndx = 0;
  char fillChar;
  int upperCharLimit;

   for(i = 0; i < stringLength; i++) {
    c = string[i];
    if(c == '-'  && (i == 0 || string[i + 1] == '-' || string[i + 1] == '\0')) {
      temp[tempIndx] = '-';
      tempIndx++;
     }
     else if(string[i+1] == '\0') {
       temp[tempIndx] = string[i];
       break;
     }
     else if(i != rangeArr[rangeIndx + 2] && (c != '-')) {
       temp[tempIndx] = string[i];
       tempIndx++;
     }
    else {
      fillChar = rangeArr[rangeIndx];
      upperCharLimit = rangeArr[rangeIndx + 1];
      for(j = 0; j <= upperCharLimit; j++) {
        if(fillChar == '-' || isalpha(fillChar)) {
          temp[tempIndx] = fillChar;
          tempIndx++;
        }
        fillChar++;
      }
      i = i + 2;
      rangeIndx = rangeIndx + 3;
    }
   }
   strcpy(string, temp);
   string[tempIndx + 1] = '\0';
}
/*
  Shifts the string one byte, starting at the specified index.
*/
void shiftString(char pattern[], int index) {

	int i;
	int patternLength;

  patternLength = strlen(pattern);

	for (i = index; i < patternLength; i++) {
		pattern[i] = pattern[i + 1];
	}
}

/*
  Iterates through escapeString, and finds any '/' character, and then checks the
  character after, '/', and then puts it into a single value.
*/
void escapeChar(char escapeString[]) {

  int escapeStringLength;
  int i;
  int nullLoc;

  escapeStringLength = strlen(escapeString);

  for(i = 0; i < escapeStringLength; i++) {
      if(escapeString[i] == '\\')
      {
        nullLoc = i + 1;
        switch (escapeString[i+1]) {
          case '\\':
            escapeString[i] = '\\';
            shiftString(escapeString, nullLoc);
            break;
          case 'a':
            escapeString[i] = '\a';
            shiftString(escapeString, nullLoc);
            break;
          case 'b':
            escapeString[i] = '\b';
            shiftString(escapeString, nullLoc);
            break;
          case 'f':
            escapeString[i] = '\f';
            shiftString(escapeString, nullLoc);
            break;
          case 'n':
            escapeString[i] = '\n';
            shiftString(escapeString, nullLoc);
            break;
          case 'r':
            escapeString[i] = '\r';
            shiftString(escapeString, nullLoc);
            break;
          case 't':
            escapeString[i] = '\t';
            shiftString(escapeString, nullLoc);
            break;
          case 'v':
            escapeString[i] = '\v';
            shiftString(escapeString, nullLoc);
            break;
          case '\'':
            escapeString[i] = '\'';
            shiftString(escapeString, nullLoc);
            break;
          case '\"':
            escapeString[i] = '\"';
            shiftString(escapeString, nullLoc);
            break;
          default:
            break;
      }
    }
  }
}

/*
  Matches the number of characters in toMatch, to pattern.  If isExtended is enabled,
  it extends toMatch to pattern
*/
void matchNumOfChars (char pattern[], char toMatch[], const int isExtended) {

  int patternLength;
  int toMatchLength;

  patternLength = strlen(pattern);
  toMatchLength = strlen(toMatch);

  if(!isExtended) {
    toMatch[patternLength] = '\0';
  } else {
      int i;
      const char c = toMatch[toMatchLength - 1];
      for(i = (toMatchLength - 1); i < patternLength; i++) {
        toMatch[i] = c;
      }
      toMatch[patternLength] = '\0';
  }
}

/*
  Main entry into program.
*/
int main(int argc, char **argv) {

  int extend;
  int indexOfLastChar;
  int rangeArr[30];
  int patternLength;
  int toMatchLength;
  int doesContainRange;
  int i;
  char c;
  char newC;
  char patternCopy[MAXSIZE];
  char toMatchCopy[MAXSIZE];

  extend = 0;
  indexOfLastChar = 0;
  doesContainRange = 0;

  if(argc == 3) {
    pattern = argv[1];
    toMatch = argv[2];
  } else if (argc == 4 && argv[1][0] == '-' && argv[1][1] == 'e') {
    pattern = argv[2];
    toMatch = argv[3];
    extend = 1;
  } else {
    perror("Incorrect parameters: need '### ###' or '-e ### ###'");
    return 1;
  }

  strcpy(patternCopy, pattern);
  strcpy(toMatchCopy, toMatch);

  /*
    converts escape characters to single characters
  */
  escapeChar(patternCopy);
  escapeChar(toMatchCopy);

  /*
    Tests if the string contains a range
  */
  doesContainRange = containRange(patternCopy);

  /*
    Gets ranges if the string contains them
  */
  if(doesContainRange) {
    getRanges(patternCopy, rangeArr);
    fillStringRanges(patternCopy, rangeArr);
    doesContainRange = 0;
  }

  /*
    Tests if the string contains a range
  */
  doesContainRange = containRange(toMatchCopy);

  /*
    Gets ranges if the string contains them
  */
  if(doesContainRange) {
    getRanges(toMatchCopy, rangeArr);
    fillStringRanges(toMatchCopy, rangeArr);
  }

  patternLength = strlen(patternCopy);
  toMatchLength = strlen(toMatchCopy);

  /*
    Matching number of characters, will extend if need be.
  */
  matchNumOfChars(patternCopy, toMatchCopy, extend);

  if(patternLength < toMatchLength) {
    matchNumOfChars(patternCopy, toMatchCopy, extend);
  } else {
    matchNumOfChars(toMatchCopy, patternCopy, extend);
  }

  patternLength = strlen(patternCopy);
  toMatchLength = strlen(toMatchCopy);

  while((c = getchar()) != EOF) {
    newC = c;
    for(i = 0; i < patternLength; i++) {
      if(c == patternCopy[i]) {
        newC = toMatchCopy[i];
      }
    }
    putchar(newC);
  }
  return 0;
}
