int uaz(char* s1, char* s2)
{
  while (s1!=0 || s2!=0)
  {
  if (s1 != s2)
  {
     return 1;
  }
  else
  {
     s1++;
     s2++;
  }
  return 0;
}

int main()
{
   printf(uaz("Hello", "hello"));
}