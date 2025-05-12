#!/bin/bash

# Materials to test
MATERIALS=("Air" "CH4" "CO2")

# Seeds to use
SEEDS=(10)

# Number of threads
THREADS=24

# Base output filename
BASE_OUTPUT="RC"

# Macro file
MACRO="./run.mac"

# Loop through all materials
for mat in "${MATERIALS[@]}"; do
    # Create output directory for this material
    mkdir -p "results_${mat}"
    
    # Loop through all seeds
    for seed in "${SEEDS[@]}"; do
        # Run simulation
        ./sim -m "${MACRO}" -t ${THREADS} -o "results_${mat}/${BASE_OUTPUT}_${mat}_s${seed}" -s ${seed} -mat "${mat}"
        
        # Sleep briefly between runs to avoid any potential conflicts
        sleep 2
    done
done

echo "All simulations completed!"