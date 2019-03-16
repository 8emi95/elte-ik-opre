#!bin/bash
# bari's profile, prompt szinek ANSII kod: http://en.wikipedia.org/wiki/ANSI_escape_code
# http://www.understudy.net/custom.html#Bourne_color

# http://www.theorphys.elte.hu/fizinf/OpRendszerek/all.pdf
# http://mek.niif.hu/01200/01249/01249.pdf
# http://home.fazekas.hu/~egmont/unix/shell.html
# http://www.szabilinux.hu/unix/index.html#shell
# http://www.cs.elte.hu/linfo/Shell/script.html
# http://www.cab.u-szeged.hu/local/doc/UNIX/orlando/bev.html





#PS1='barika\[\033[01;32m\]\u@\h:\w\$'
PS1="\033[1m\bari\[\033[01;32m\][\u\033[0m\]@\[\033[34m\]\h]\]\033[0m\]:"

echo ' Szia Adam! =)'

ora=`date +%H`
if [ $ora  -lt 12 ]; then
echo hey mornin !
elif [ $ora -gt 18 ]; then
echo Estet!
else 
echo Gutentaaaag!
fi
