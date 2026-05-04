#!/bin/bash

# Configuración 
EXECUTABLE="./build/release/apps/exp_pat_fijo"
DATA_DIR="./textos"
RESULTS_DIR="./results"
CSV_OUT="$RESULTS_DIR/exp_pat_fijo_results.csv"
TEMP_MEM=".temp_mem.log"
PATTERN="questions" 

mkdir -p "$RESULTS_DIR"

# Encabezado del csv
echo "kind;text_path;pattern;count;time_ns;max_memory_kb" > "$CSV_OUT"

KINDS=("sdsl_huff_rrr" "sdsl_blcd" "lab_wt" "brute")

for text_file in "$DATA_DIR"/*; do
    if [ -f "$text_file" ] && [[ "$text_file" != *.fm ]]; then
        filename=$(basename "$text_file")
        
        for kind in "${KINDS[@]}"; do
            echo "Ejecutando: $filename [$kind] | Kind: $kind"
            OUTPUT=$({ /usr/bin/time -v "$EXECUTABLE" "$text_file" "$PATTERN" "$kind" 2> "$TEMP_MEM"; } >> /dev/stdout)
            MEM_KB=$(grep "Maximum resident set size" "$TEMP_MEM" | awk '{print $6}')
            echo "${OUTPUT};${MEM_KB}" >> "$CSV_OUT"
        done
    fi
done

rm -f "$TEMP_MEM"
echo "Completado. Resultados en $CSV_OUT"