Mi a különbség az alarm, a setitimer és a timer_create között?
alarm - egszer fut le, SIGALRM-t küld
setitimer - többször, SIGALRM-t küld, csak 3 lehet belőle
timer_create - többször, bármilyen jelzés,???

handler - volatile sig_atomic
asynchronous-safe functions 



