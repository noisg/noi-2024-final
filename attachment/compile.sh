#!/bin/bash
g++ -DEVAL -std=gnu++17 -O2 -pipe -static -s -o toxic toxic.cpp stub.cpp
