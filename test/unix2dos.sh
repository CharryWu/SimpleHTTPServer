#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 input-file.txt output-file.txt"
  exit 1
fi

if [ ! -f $1 ]; then
  echo "File $1 not found"
  exit 2
fi

sed 's/$'"/`echo \\\r`/" < $1 > $2
