#!/bin/bash

g++ *.cpp -o main -lwthttp -lwt -lboost_system -O2 -larmadillo -std=c++11 -lpthread -w
