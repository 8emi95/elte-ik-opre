#!/bin/bash

COL=80
ROW=$(tput lines)

cleanup() {
  wait
  printf '\033[2J\033[1;1H\033[?25h\033[0;0m'
}

drop() {
  for ((r=1; r<$2+$3+1; r++)); do
    [ $r -lt $2 ] && printf "\033[$r;$1H\033[0;0m\u$[RANDOM % 59 + 21]"
    [ $r -le $2 ] && printf "\033[$[r-1];$1H\033[0;32m\u$[RANDOM % 59 + 21]"
    [ $r -gt $3 ] && printf "\033[$[r-$3];$1H "
    sleep .1
  done
}

trap cleanup EXIT
printf '\033[2J\033[?25l'

while sleep .1; do
  drop $[RANDOM % COL] $[RANDOM % ROW + ROW/3] $[RANDOM % 10 + 5] &
done