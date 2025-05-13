#!/bin/bash

MATERIALS=("Air" "CH4" "CO2")      
SEEDS=(10)                          
THREADS=24                          
BASE_OUTPUT="RC"                    
MACROS=("./run05.mac" "./run1.mac" "./run5.mac" "./run10.mac") 

get_energy() {
    local macro=$1
    local num_part=$(basename "$macro" .mac | sed 's/run//')

    if [[ $num_part =~ ^0 ]]; then
        echo "0.${num_part:1}"
    else
        echo "$num_part"
    fi
}

TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
MAIN_RESULTS_DIR="simulation_results_${TIMESTAMP}"
mkdir -p "$MAIN_RESULTS_DIR"

for macro in "${MACROS[@]}"; do

    ENERGY=$(get_energy "$macro")
    

    ENERGY_DIR="${MAIN_RESULTS_DIR}/E${ENERGY}GeV"
    mkdir -p "$ENERGY_DIR"
    

    for mat in "${MATERIALS[@]}"; do
    
        MAT_DIR="${ENERGY_DIR}/${mat}"
        mkdir -p "$MAT_DIR"
        
    
        for seed in "${SEEDS[@]}"; do
            echo "Running simulation: Energy=${ENERGY}GeV, Material=${mat}, Seed=${seed}"
            
            ./sim -m "$macro" -t $THREADS -o "${MAT_DIR}/${BASE_OUTPUT}_E${ENERGY}_${mat}_s${seed}" -s $seed -mat "$mat"
            
            sleep 2
        done
    done
done

echo -e "\nAll simulations completed!"
echo "Results saved in: $MAIN_RESULTS_DIR"
echo "Directory structure:"
tree -L 3 "$MAIN_RESULTS_DIR"