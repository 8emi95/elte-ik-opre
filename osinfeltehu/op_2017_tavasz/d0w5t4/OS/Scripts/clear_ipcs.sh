#!/bin/bash

ipcs | grep $1 | tr -s ' ' | cut -d' ' -f2 | while read -r line ; do
    ipcrm shm $line
    ipcrm msg $line
    ipcrm sem $line
    # your code goes here
done
