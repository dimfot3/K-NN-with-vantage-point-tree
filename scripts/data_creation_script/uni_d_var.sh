#!/bin/bash

for d in {10..30000..5000}
do
    python3 create_data.py -o 1 -n 20000 -d $d
done
