

int main(){
  pid_t child = fork();
  printf("Hello %i", getpid());
  return 0;
}