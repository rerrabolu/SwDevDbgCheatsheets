
To get NVIM going I needed to install various executable and libraries.
One good reference that got me going is:

https://marioyepes.com/vim-setup-for-modern-web-development

sudo apt install nvim
sudo apt install nodejs
sudo apt install yarn
sudo apt install yarn

I  could not get a floating window to show definitions of various
symbols - functions, structures, macros, defines, etc by default
I had to add semanticart/tag-peek.vim plugin to get that working
The most important thing was to define the "leader" key that would
trigger showing the floating window. The relevant init.vim statements
in this regard are:

    let mapleader= ","
    let g:map_leader = ","
    nnoremap <leader>p :call tag_peek#ShowTag()<CR>

Once I did this I was able to enter ",p" to show the definitions

The command Plug will look at github by default. To provide another
source to get the plugin one can use http or https protocols

    " Plugin command will look in github
    " Plugin to peek at definitions of symbols
    Plug 'semanticart/tag-peek.vim'

    " Plugin command using http and https protocols
    " Surrounding ysw)
    Plug 'http://github.com/tpope/vim-surround'
    " CSS Color Preview
    Plug 'https://github.com/ap/vim-css-color'

After initial launch make sure to run the following commands
in nvim command mode:

    // This will download the various plugins
    :PlugInstall
    // This will update any plugins that have updates
    :PlugUpdate
    // Will check if plugins are properly setup
    :checkhealth

Lastly nvim by default is console mode only. To get a graphical
mode one has to install a graphical skin. I used neovide as my
graphical skin. One disadvantage over gvim is the lack of menu
items on the graphical skin of neovide. For example I am unable
to copy/past/cut from neovide to another window. This is a major
inconvenience.

I tried nvim-qt but it didn't work out. Lastly

