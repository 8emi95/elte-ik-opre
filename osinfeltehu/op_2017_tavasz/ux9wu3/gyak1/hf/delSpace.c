enum Bool {false, true};
#define bool enum Bool
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    printf("Usage: delSpace string\n");
    return 1;
  }
  int i = 0;
  int ind;
  bool l = false;
  char* text = argv[1];
  int len= strlen(text);
  while (!l  && i<len)
  {
    l = text[i] != ' ';
    ind = i;
    i++;
  }
  if (!l)
  {
    return 0;
  } else
  {
    int back = strlen(text)-1;
    while (text[back] == ' ')
      back--;
    i = 0;
    while (ind <= back)
    {
      text[i] = text[ind];
      i++;
      ind++;
    }
    text[i] = '\0';
    printf("%s",text); //format-string exploit
    return 0;
  }
}
