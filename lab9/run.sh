#!/bin/bash

if [ $# -ne 2 ]; then
  echo "usage: $0 <option> <num_nodes>"
  echo "option 1 - regular"
  echo "option 2 - Karatshuba"
  exit 1
fi


file=""
if [ $1 -eq 1 ]; then
  echo "Regular on $2 nodes"
  file=main_regular
elif [ $1 -eq 2 ]; then
  echo "Karatshuba on $2 nodes"
  file=main_karatsuba
else
  echo "Invalid option"
  exit 1
fi

make "$file" && mpirun --oversubscribe -np $2 ./"$file"
