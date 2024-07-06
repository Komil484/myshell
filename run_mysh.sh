#!/bin/bash

cmake -Bbuild source -Werror=dev

if [ $? -ne 0 ]; then
    exit 1
fi

cmake --build build

if [ $? -ne 0 ]; then
    exit 1
fi

./build/mysh
