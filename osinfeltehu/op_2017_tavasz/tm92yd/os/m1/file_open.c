//Fájlmegnyitás
  FILE *f
  f = fopen(fn, mód) f == NULL
  fclose(f)
  fseek()
  feof(f) 
  --Text
    fprintf(f, formatum string, valtozo lista)
    fputs(string, f)
    fgets(string, max beolvasandó karakterszám, f)
  --bin
    fwrite(kezdocim, elemmeret, elemszam,f)
    fread(kezdocim, elemmeret, elemszam, f)

//Text
    mód "r": read
        "w": write
        "a": add
        "r+": read-write meglévő
        "w+": read-write új
        "a+": read-add új

--

<unistd.h>
access("filenev", mód)
R_OK - read
W_OK - fwrite
X_OK - vagy-vagy
F_OK - létezés

//bin
    mód "rb": read
        "wb": write
        "ab": add
        "rb+": read-write meglévő
        "wb+": read-write új
        "ab+": read-add új

Tömb t[]
fread(t, sizeof(t[0]), sizeof(t)/sizeof(t[0]), f)
fwrite(t, sizeof(t[0]), sizeof(t)/sizeof(t[0]), f)

Változó v
fread(&v, sizeof(v), 1, f)
fwrite(&v, sizeof(v), 1, f)

structura : char 'a'....'z', int'1'...'n' ---->>> n elemű tömb, n páros

tömb ki fájlba bin
majd visszafelé kettesével egy új tömbe
