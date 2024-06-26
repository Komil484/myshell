#!/bin/bash
cmake -Bbuild source
cmake --build build
./build/mysh
