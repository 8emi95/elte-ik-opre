int length(char *pointer) {
    int len = 0;
    while(str[len] != 0) {
        len++;
    }
    return len;
}

--

int length(char *pointer) {
    int len = 0;
    while(*pointer != 0) {
        len++;
        pointer++;
    }
    return len;
}

--

int length(char *pointer) {
    int len = 0;
    while(*pointer++ != 0) {
        len++;
    }
    return len;
}

--

int length(char *pointer) {
    int len = 0;
    while(*pointer++) {
        len++;
    }
    return len;
}

--

int length(char *pointer) {
    char *start;
    while(*pointer++);
    return --pointer - start;
}