" config.vim

" {{{ Behaviour
set nocompatible
let mapleader= ","
let g:map_leader = ","
set number                " Show numbers on the left
"set relativenumber        " Its better if you use motions like 10j or 5yk
set hlsearch              " Highlight search results
set ignorecase            " Search ignoring case
set smartcase             " Do not ignore case if the search patter has uppercase
set noerrorbells          " I hate bells when an error occurs
set belloff=esc           " Disable bell if type <esc> multiple times
set tabstop=4             " Tab size of 4 spaces
set softtabstop=4         " On insert use 4 spaces for tab
set shiftwidth=4
set expandtab             " Use appropriate number of spaces
set nowrap                " Wrapping sucks (except on markdown)
set noswapfile            " Do not leave any backup files
set backup                " Make a backup of file being edited
set history=108           " Remember last N commands
set columns=120           " Set the number of chars per line
set mouse=nvci            " Enable mouse on Normal, Visual, Command and Insert modes
set fileformats=unix,dos

"set clipboard=unnamed,unnamedplus     " Use the OS clipboard
set showmatch             " Highlights the matching parenthesis
set termguicolors         " Required for some themes
set splitright splitbelow " Changes the behaviour of vertical and horizontal splits
set foldlevel=1           " Better for markdown and PHP classes
set cursorline            " Highlight the current cursor line (Can slow the UI)
set signcolumn=yes        " Always show the signcolumn, otherwise it would shift the text
set hidden
filetype plugin indent on " Enable file type detection.
let &t_EI = "\e[2 q"      " Make cursor a line in insert on Vim
let &t_SI = "\e[6 q"      " Make cursor a line in insert on Vim

" Keep Visual Mode after indenting a block with > or <
vmap < <gv
vmap > >gv

" Move Visual blocks up or down with J an K
vnoremap J :m '>+1<CR>gv=gv
vnoremap K :m '<-2<CR>gv=gv

" YY/XX Copy/Cut into the system clipboard
noremap YY "+y<CR>
noremap XX "+x<CR>

" search will center on the line it's found in.
nnoremap n nzzzv
nnoremap N Nzzzv

" Enable native markdown folding (hopefully will be integrated in nvim)
let g:markdown_folding = 1
" }}}

" {{{ Auto Commands
" Enable wrap on Markdown and Text files
if !exists('*s:setupWrapping')
  function s:setupWrapping()
    setlocal wrap
    setlocal noshowmatch
    nnoremap <buffer> j gj
    nnoremap <buffer> k gk
  endfunction
endif
augroup vimrc-enable-wrap-on-text-files
  autocmd!
  autocmd BufRead,BufNewFile *.txt,*.md call s:setupWrapping()
augroup END

" Auto command to remember last editing position
augroup vimrc-remember-cursor-position
  autocmd!
  autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g`\"" | endif
augroup END

" Install vim-plug
if empty(glob('~/.vim/autoload/plug.vim'))
  silent !curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
  autocmd VimEnter * PlugInstall --sync | source $MYVIMRC
endif

" AutoOpen NetRW of called without parammters
augroup netrw-auto-open-if-no-params
  autocmd!
  autocmd VimEnter * if argc() == 0 | Explore! | endif
augroup END
" }}}

" {{{ Plugins
call plug#begin()  					" ('~/.config/nvim/plugged')
Plug 'tpope/vim-sensible'                               " Makes vim work as you'd expect

" Themes
Plug 'drewtempelmeyer/palenight.vim'                    " Soothing color scheme based on material palenight
Plug 'haishanh/night-owl.vim'                           " Hight contrast but relaxed theme
Plug 'mhartington/oceanic-next'                         " Beautiful pastell-y colors
Plug 'nanotech/jellybeans.vim'                          " Hight contrast but colorfoul

Plug 'sheerun/vim-polyglot'                             " Metapackage with a bunch of syntax highlight libs

" Make VIM an IDE
" Plugins adding Functional value
Plug 'preservim/tagbar'                               " Plugin for Tagbar
Plug 'neoclide/coc.nvim', {'branch': 'master', 'do': 'yarn install --frozen-lockfile'}
" Plug 'neoclide/coc.nvim', {'branch': 'release', 'do': 'yarn install --frozen-lockfile'}

" Plug 'liuchengxu/vista.vim'                             " Like Ctags but for LSP (CoC)
"Plug 'dense-analysis/ale'                               " Code sniffing using external tools
Plug 'tpope/vim-fugitive'                               " Like :!git but better


" Plug 'feline-nvim/feline.nvim'                            " Beautify status line

Plug 'itchyny/lightline.vim'                            " Beautify status line
Plug 'josa42/vim-lightline-coc'                         " Show CoC diagnostics in LightLine

Plug 'scrooloose/nerdtree', { 'on':  'NERDTreeToggle' } " File navigator with <C-k><C-k>
Plug 'Xuyuanp/nerdtree-git-plugin'                      " Show git status on NERDTree
Plug 'airblade/vim-gitgutter'                           " Show which lines changed on gutter
Plug 'editorconfig/editorconfig-vim'                    " Configure tab or spaces per project
Plug 'bogado/file-line'                                 " Enable opening vim like - vim my_file.php:8
Plug 'ryanoasis/vim-devicons'                           " Icons on NERDtree and Vista

Plug 'terryma/vim-multiple-cursors'                     " Multiple cursors like Sublime with <C-n>
Plug 'junegunn/vim-easy-align'                          " Align text by characters or reguex
Plug 'mattn/emmet-vim'                                  " Emmet support with <C-y>,
Plug 'tpope/vim-surround'
Plug 'tpope/vim-commentary'

Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }     " Install fuzzy finder binary
Plug 'junegunn/fzf.vim'                                 " Enable fuzzy finder in Vim with <C-p>
Plug 'iamcco/markdown-preview.nvim', { 'do': 'cd app && yarn install'  }

Plug 'semanticart/tag-peek.vim'                         " Plugin to peek at definitions of symbols

Plug 'morhetz/gruvbox'					" GruvBox colorscheme

call plug#end()
" }}}

" {{{ Theme(s) settings
if !has('nvim')
    " Enable italics in Vim 8
    let &t_ZH="\e[3m"
    let &t_ZR="\e[23m"
endif
let g:gruvbox_material_background = 'hard'
let g:gruvbox_material_enable_italic = 1
let g:gruvbox_material_palette = 'mix'
let g:palenight_terminal_italics = 1
let g:vim_monokai_tasty_italic = 1
let g:jellybeans_use_term_italics = 1

" Disable printing startup warnings
let g:coc_disable_startup_warning = 1

" set laststatus=2
" if !has('gui_running')
"  set t_Co=256
" endif


silent! colorscheme gruvbox
" silent! colorscheme night-owl
" silent! colorscheme palenight
" silent! colorscheme OceanicNext
" silent! colorscheme jellybeans
 " }}}

" set guifont=Monospace\ 11   "specify the font and its properties"
set guifont=Source\ Code\ Pro\ 12   "specify the font and its properties"

" {{{ CoC extensions to be auto installed
let g:coc_node_args = ['--max-old-space-size=8192']
let g:coc_global_extensions = [
    \ 'coc-css',
    \ 'coc-diagnostic',
    \ 'coc-eslint',
    \ 'coc-clangd',
    \ 'coc-html',
    \ 'coc-json',
    \ 'coc-marketplace',
    \ 'coc-phpls',
    \ 'coc-prettier',
    \ 'coc-stylelintplus',
    \ 'coc-tsserver',
    \]
" }}}
    " \ 'coc-python',

" {{{ CoC (taken from github.com/neoclide/coc.nvim some comments removed)

" Use tab for trigger completion with characters ahead and navigate.
" NOTE: Use command ':verbose imap <tab>' to make sure tab is not mapped by
" other plugin before putting this into your config.
inoremap <silent><expr> <TAB>
      \ pumvisible() ? "\<C-n>" :
      \ <SID>check_back_space() ? "\<TAB>" :
      \ coc#refresh()
inoremap <expr><S-TAB> pumvisible() ? "\<C-p>" : "\<C-h>"

function! s:check_back_space() abort
  let col = col('.') - 1
  return !col || getline('.')[col - 1]  =~# '\s'
endfunction

" GoTo code navigation.
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)

" Use K to show documentation in preview window.
nnoremap <silent> K :call <SID>show_documentation()<CR>

function! s:show_documentation()
  if (index(['vim','help'], &filetype) >= 0)
    execute 'h '.expand('<cword>')
  elseif (coc#rpc#ready())
    call CocActionAsync('doHover')
  else
    execute '!' . &keywordprg . " " . expand('<cword>')
  endif
endfunction

" Add `:Format` command to format current buffer.
command! -nargs=0 Format :call CocAction('format')

" Add `:Fold` command to fold current buffer.
command! -nargs=? Fold :call     CocAction('fold', <f-args>)

" Add `:OR` command for organize imports of the current buffer.
command! -nargs=0 OR   :call     CocAction('runCommand', 'editor.action.organizeImport')

" Mappings for CoCList
" Show all diagnostics.
nnoremap <silent><nowait> <space>a  :<C-u>CocList diagnostics<cr>
" }}}

" {{{ Vista
" let g:vista_default_executive = 'coc'
" let g:vista#renderer#enable_icon = 1
" let g:vista_sidebar_width = 45
" let g:vista_close_on_jump = 1
" let g:vista_close_on_fzf_select = 1
" let g:vista_fzf_preview = ['right:50%']
" nnoremap <C-k><C-o> :Vista finder fzf:coc<cr>
" }}}

" {{{ ALE
let g:ale_disable_lsp = 1
let g:ale_linters_explicit = 1
let g:ale_linters = {
  \ 'python': ['pylint']
  \ }
let g:ale_fixers = {
  \ 'php': ['phpcbf'],
  \ '*': ['remove_trailing_lines', 'trim_whitespace'],
  \}
let g:ale_lint_on_text_changed = 'never'
let g:ale_lint_on_insert_leave = 0
"}}}

" {{{ LightLine
function! LightLineFilename()
  return expand('%')
endfunction

" Define what is shown on status line
set laststatus=2
set statusline+=%h

" Configure the sections of the statusline
" Path to file: https://github.com/itchyny/lightline.vim/issues/87#issuecomment-119130738
let g:lightline = { 'active': {  } }
let g:lightline.component = {
  \  'lineinfo': "[%{printf('%03d/%03d',line('.'),line('$'))}]"
  \}
let g:lightline.component_function = {
  \    'gitbranch': 'fugitive#head',
  \    'method': 'NearestMethodOrFunction',
  \    'filename': 'LightLineFilename'
  \  }
" When using ALE for diagnostics
" let g:lightline.component_expand = {
"       \  'linter_checking': 'lightline#ale#checking',
"       \  'linter_infos': 'lightline#ale#infos',
"       \  'linter_warnings': 'lightline#ale#warnings',
"       \  'linter_errors': 'lightline#ale#errors',
"       \  'linter_ok': 'lightline#ale#ok',
"       \ }
" " When using CoC's diagnostics-languageserver for diagnostics
let g:lightline.component_expand = {
  \   'linter_warnings': 'lightline#coc#warnings',
  \   'linter_errors': 'lightline#coc#errors',
  \   'linter_ok': 'lightline#coc#ok',
  \   'status': 'lightline#coc#status',
  \ }
let g:lightline.component_type = {
      \     'linter_checking': 'right',
      \     'linter_infos': 'right',
      \     'linter_warnings': 'warning',
      \     'linter_errors': 'error',
      \     'linter_ok': 'right',
      \ }
let g:lightline.active.left = [
  \      [ 'linter_checking', 'linter_errors', 'linter_warnings', 'linter_infos', 'linter_ok' ],
  \      [ 'coc_errors', 'coc_warnings', 'coc_ok' ], [ 'coc_status'  ],
  \      [ 'gitbranch', 'readonly', 'filename', 'tagbar', 'modified', 'method' ]
  \]
let g:lightline.active.right = [
  \      ['lineinfo'], ['fileformat', 'filetype']
  \]
" https://github.com/itchyny/lightline.vim/tree/master/autoload/lightline/colorscheme
" let g:lightline.colorscheme = 'PaperColor'
" let g:lightline.colorscheme = 'powerlineish'
let g:lightline.colorscheme = 'molokai'
" let g:lightline.colorscheme = 'material'
" let g:lightline.colorscheme = 'selenized_dark' " Goes great with night owl
" }}}


"
" Bind F8 to turn on tabar
" <C-t> means Ctrl t
" @note: Open Tagbar to the left
"
let g:tagbar_left = 1
let g:tagbar_active = 1
nmap <F8> :TagbarToggle<CR>
nnoremap <C-t> :TagbarToggle<CR>


"
" Key mapping for NERDTree plugin
" @note: Bind +/- to indicate filetree
" <C-f> means Ctrl f opens file tree pane
" ,f makes file tree pane active
"
" {{{ NERDTree
" let NERDTreeMinimalUI=1    " Remove the help at the top
" let NERDTreeQuitOnOpen=1   " Close sidebar when opening a file
" let NERDTreeShowHidden=1   " Do I have to explain this
" let NERDTreeWinPos='right' " Or this?
" let NERDTreeWinSize=45     " Increase the sidebar size
" map <C-k><C-k> :NERDTreeToggle<cr>
" map <C-k><C-f> :NERDTreeFind<cr>
nmap <F9> :NERDTreeToggle<CR>
nnoremap <C-f> :NERDTreeToggle<CR>
let g:nerdtree_left = 1
let g:nerdtree_active = 1
let g:NERDTreeDirArrowExpandable="+"
let g:NERDTreeDirArrowCollapsible="~"


" Do not show lightline on NERDTree
augroup nerdtree-normal-statusline
    autocmd!
    autocmd BufEnter,FileType nerdtree setlocal statusline=%#Normal#
augroup END
" }}}

" {{{ EasyAlign. Start interactive modes in visual and motion/text objects
xmap ga <Plug>(EasyAlign)
nmap ga <Plug>(EasyAlign)
" }}}

" {{{ Vim commentary
autocmd FileType php setlocal commentstring=//\ %s
" }}}

" {{{ FzF
let $FZF_DEFAULT_COMMAND = 'rg --files --hidden --glob "!.git/*"'
let g:fzf_layout = { 'window': { 'width': 0.9, 'height': 0.9 } }
map <C-p> :Files<cr>
map <C-k><C-p> :GFiles<cr>
map <C-k><C-l> :Buffers<cr>
nmap ?? :Rg!!<cr>
" }}}

" {{{ Markdown Preview. Do not autoclose on change buffer and refresh only on normal
let g:mkdp_auto_close = 0
let g:mkdp_refresh_slow = 1
let g:mkdp_preview_options = {
  \ 'sync_scroll_type': 'relative',
  \ 'disable_sync_scroll': 1,
  \ 'disable_filename': 1
  \ }
" }}}

" vim: ts=2 sw=2 et fdm=marker

" let g:map_leader = ','
" Show definition when using ,p key
nnoremap <leader>p :call tag_peek#ShowTag()<CR>

