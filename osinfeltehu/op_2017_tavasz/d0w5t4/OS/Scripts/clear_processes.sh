#!/bin/bash

ps -U $1 | grep a.out | tr -s ' ' | cut -d' ' -f1 | tac | while read -r line ; do
    kill -9 $line
    # your code goes here
done
