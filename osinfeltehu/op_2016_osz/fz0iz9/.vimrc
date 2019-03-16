set number
syntax on
filetype plugin indent on
colorscheme desert


let mapleader=","

"-Searching"
set hlsearch
set incsearch
nmap <Leader>h :noh <cr>

"-Mappings"
nmap <Leader>ev :tabedit $MYVIMRC<cr>
nmap <C-h> <C-w><C-h>
nmap <C-j> <C-w><C-j>
nmap <C-k> <C-w><C-k>
nmap <C-l> <C-w><C-l>


"-AutoCommands"
autocmd BufWritePost vimrc source %

"-Split"
set splitbelow
set splitright

"-Indent"
set pastetoggle=<f5>
set tabstop=4
set expandtab
set shiftwidth=4
set autoindent
set smartindent
set cindent
