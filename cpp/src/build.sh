#!/bin/bash
bison -d -o fay_parser.cpp fay.y
flex -o fay_tokens.cpp fay.l
