#!/bin/sh

gcc -Wall -o my-command eop1_05_09501502.c || exit 1
noah db-sample  < stdin.csv > result.log
./my-command < stdin.csv > my-result.log
diff result.log my-result.log
