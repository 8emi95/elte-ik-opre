#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

alias ls='ls --color=auto'
PS1='\[\e[1;36m\][\u@\h \w]\$ \[\e[m\]'
PATH="$PATH:~/bin:."
alias cd..='cd ..'
