inc_cterm="-Iexternal/cterm/ external/cterm/cterm.c"

execute() {
    echo "$1"
    eval "$1"
}

execute "cc -o 2048 main.c -Wall -Wextra -g $inc_cterm"
