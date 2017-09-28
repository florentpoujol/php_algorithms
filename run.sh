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
    php -d xdebug.max_nesting_level=100000 php/$2.php $3 $4
fi
