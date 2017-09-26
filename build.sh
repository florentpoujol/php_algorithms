#!/bin/bash

if [[ $1 == "z" ]]; then
    cd zephir
    zephir build
    cd ../
fi
  
# params: c script_name
if [[ $1 == "c" ]]; then 
    gcc c/$2.c -g -o c/builds/$2
    echo "build end"
fi

if [[ $1 == "cc" ]]; then 
    cppcheck --enable=all --inconclusive --suppress=missingIncludeSystem c/$2.c
    gcc c/$2.c -g -o c/builds/$2
    echo "build end"
fi