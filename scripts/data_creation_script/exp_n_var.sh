#!/bin/bash

for n in {10..60000..5000}
do
    python3 create_data.py -o 3 -n $n -d 2000 
done
