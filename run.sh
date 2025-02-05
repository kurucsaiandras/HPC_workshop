#!/bin/bash

# Array of matrix sizes
# L1 = 90
# L2 = 256
# L3 = 724
SIZES="20 40 60 80 100 200 400 600 800 1000 1200 1400 1600"
PERMUTATIONS="mnk mkn nmk nkm kmn knm"

# Output file name
output_name="results.dat"

# Delete all .dat files
rm -f *.dat

# Run matmul with different sizes and save the output
for P in $PERMUTATIONS
do
    for S in $SIZES
    do
        ./matmul $S $S $S $P >> ${P}_$output_name
    done
done

python3 plot_results.py