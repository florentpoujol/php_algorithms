#!/bin/bash

# params: c script_name array_count array_size
if [[ $1 == "c" ]]; then
    if [ -f "core" ]; then
        rm core
    fi
    ulimit -c unlimited
    ./c/builds/$2 $3 $4
fi

# params: c script_name array_count array_size
if [[ $1 == "php" ]]; then
    ulimit -c unlimited
    php php/$2.php $3 $4
fi
