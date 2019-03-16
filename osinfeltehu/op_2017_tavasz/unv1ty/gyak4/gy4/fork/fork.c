//2 gyerek létrehozása:

pid_t child_a, child_b;
child_a = fork();

if (child_a == 0) {
    /* Child A code */
} else {
    child_b = fork();

    if (child_b == 0) {
        /* Child B code */
    } else {
        /* Parent Code */
    }
}

// n folyamat

for (i = 0; i < n; ++i) {
    pid = fork();
    if (pid) {
        continue;
    } else if (pid == 0) {
        break;
    } else {
        printf("fork error\n");
        exit(1);
    }
}

