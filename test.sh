#!/bin/sh

gcc -Wall -o my-command eop2_final_09501502.c || exit 1
./db-sample_macos  < stdin.csv > result.log
./my-command < stdin.csv > my-result.log

RESULT=$(diff result.log my-result.log)
if [ -n "$RESULT" ]; then
   echo "Error: 差分が存在します"
fi