#include <stdio.h>

struct mystruct{
  char ch;
  int in;
};

int main(){
  const int n = 10;
  struct mystruct stv[n], st =('a',1);

  int i;
  for(i = 0; i < n; ++i){
    stv[i]=st;
    st.ch++;
    st.in++;
  }

  FILE *f;
  f=fopen("bin.dat", "wb");
  fwrite(stv, sizeof(stv[0]), 2, f);
  fclose(f);

  f=fopen("bin.dat", "rb");
  for(i = n-2; i >= 0; i -= 2){
    
    fread(&stv[i], sizeof(stv[0]), 2, f);
  }
  fclose(f);

  for(i = 0; i < n; ++i){
    printf("%c, %i \n0", stv[i].ch, stv[i].in);
  }
}

