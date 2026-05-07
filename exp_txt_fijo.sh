#!/bin/bash

# Configuración
EXECUTABLE="./build/release/apps/exp_txt_fijo"
DATA_DIR="./datasets/experiment_txt_fijo"
RESULTS_DIR="./results"
CSV_OUT="$RESULTS_DIR/exp_txt_fijo_results.csv"
TEMP_MEM=".temp_mem_fijo.log"

mkdir -p "$RESULTS_DIR"

# Encabezado del CSV
echo "kind;text_path;patt_size;mode;count;time_ns;max_memory_kb" > "$CSV_OUT"

KINDS=("sdsl_huff_rrr" "sdsl_blcd" "lab_wt" "brute")

# Tamaños de patron
SIZES=(2 4 8 16 32 64 128 256 512)

# Modos de experimento
MODES=("hit" "miss")

REPEATS=1

for text_file in "$DATA_DIR"/*; do
    if [ -f "$text_file" ] && [[ "$text_file" != *.fm ]]; then
        filename=$(basename "$text_file")
        
        for kind in "${KINDS[@]}"; do
            for size in "${SIZES[@]}"; do
                for mode in "${MODES[@]}"; do
                    for ((i=1; i<=REPEATS; i++)); do
                        
                        echo "Ejecutando: $filename | Kind: $kind | Size: $size | Mode: $mode | Run: $i"

                        OUTPUT=$({ /usr/bin/time -v "$EXECUTABLE" "$text_file" "$size" "$kind" "$mode" 2> "$TEMP_MEM"; } >> /dev/stdout)
                        
                        MEM_KB=$(grep "Maximum resident set size" "$TEMP_MEM" | awk '{print $6}')        

                        echo "${OUTPUT};${MEM_KB}" >> "$CSV_OUT"

                    done
                done
            done
        done
    fi
done

rm -f "$TEMP_MEM"
echo "Completado. Resultados en $CSV_OUT"