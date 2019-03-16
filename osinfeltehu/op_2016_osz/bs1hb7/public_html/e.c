int compare (char* str1, char* str2)
{   #ket karakter lancot hasonlit ossze str1 == str2 nem hasznalhato
  while(str1!=0 || str2 !=0)
  {
    if(str1 != str2)
    {
      return 1;
    }
    else
    {
      str1++;
      str2++;
    }
    
  }
  return 0;
}


#malloc (allocate new memory), fee a mallokal lefoglalt terület felszabaditás
# int main(int argc, char ** argv) argv ben az argumentumok azok megadása: "./e.c hello"
#nincs a végén 0 karakter 