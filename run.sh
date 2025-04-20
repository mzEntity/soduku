#!/bin/bash

make clean;
if make; then
    ./main
fi
