#!/bin/bash

# Tárgy: Elte IK Operációs rendszerek 1. Beadandó
# Készítette: Sándor Balázs - AZA6NL
# Git: https://github.com/SandorBalazsHU/elte-ik-os-1

# A fordítást segítő szkriptfájl.

clear
rm Kos
gcc -O3 ./Kos.c  view/* model/* controller/* tests/* simulation/* -O3 -o Kos
if [ $? -ne 0 ]; then
    echo "Fordítás sikertelen!";
else
    echo "Fordítás sikeres!";
    read -n1 -r -p "Üss entert a futtatáshoz vagy d -t a kos.data törléséjez vagy t -t a tesztek futtatásért:" key;
    echo $key + ";";
    if [ "$key" == "t" ]; then
        ./Kos -t
    else
        if [ "$key" == "d" ]; then
            rm kos.data
            ./Kos
        else
            ./Kos
        fi
    fi
fi