#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    //Randomly selects one of the characters in the array that represent the possible choices for state
    const char characters[9] = {'[','(','{',' ', 'a', 'x', '}', ')', ']'};
    return characters[rand() % 9];
}

char *inputString()
{
    //Create a fixed length string of randomly selected lower case letters
    int len = 5;
    int i;
    //allocate memory for the string
    char* str = malloc(len+1);
        
    //Generates a random string of five lower case letters and a null terminator
    for(i = 0; i < len; i++)
    {
      //Select a random ASCII character in range [97-122] which represents the lower case letters
      str[i] = (rand() % (123-97)) + 97;
    }
    str[i] = '\0';      
      
    return str;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
