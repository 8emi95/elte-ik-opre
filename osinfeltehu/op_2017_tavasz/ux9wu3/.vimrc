""""""""""""""""""""""""""""""""
" own .vimrc file              "
" Author: Kovács Adorján Márk  "
""""""""""""""""""""""""""""""""
"Things for Vundle
"set nocompatible 
"filetype off
"set rtp+=~/.vim/bundle/Vundle.vim
"call vundle#begin()
"Plugins
"Plugin 'VundleVim/Vundle.vim'
"call vundle#end()
"filetype plugin indent on

"clang complete
"let g:clang_library_path = "/usr/lib"

" Syntax-highlighting
:syntax on

" Line numbers 
:set nu
:set numberwidth=2

" Wrapping
:set wrap

" Tab to spaces
:set expandtab
:set tabstop=2
:set shiftwidth=2

" Abbreviations
:iabbrev @@ kovacsadorjanmark@gmail.com
:iabbrev nnn Kovács Adorján Márk

" Auto-indentation stuff
:set smartindent
:set autoindent

" World wrap when commit
au FileType gitcommit set tw=75


" Temporary configs
" levverseny
":nnoremap l :%s/\s//g <bar> :%s/\n//g <bar> s%/\r//g <CR>
" End of temporary configs
