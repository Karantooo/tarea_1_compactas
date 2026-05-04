#!/bin/bash

# Configuración de rutas
EXECUTABLE="./build/release/apps/experimento"
DATA_DIR="./textos"
RESULTS_DIR="./results"
CSV_OUT="$RESULTS_DIR/experiment_results.csv"

# Archivo temporal para capturar la salida de 'time'
TEMP_MEM=".temp_mem.log"

# Parámetros fijos
PATTERN="questions" 

# Crear carpeta de resultados
mkdir -p "$RESULTS_DIR"

echo "kind;text_path;pattern;count;time_ns;max_memory_kb" > "$CSV_OUT"

# Lista de implementaciones
KINDS=("sdsl_huff_rrr" "sdsl_blcd" "lab_wt" "brute")

for text_file in "$DATA_DIR"/*; do
    if [ -f "$text_file" ]; then
        filename=$(basename "$text_file")
        
        for kind in "${KINDS[@]}"; do
            echo "Ejecutando: $filename [$kind]"
            
            # 1. Ejecutar y capturar la salida del programa en una variable
            # 2. Capturar la métrica de tiempo/memoria en el archivo temporal
            OUTPUT=$({ /usr/bin/time -v "$EXECUTABLE" "$text_file" "$PATTERN" "$kind" 2> "$TEMP_MEM"; } >> /dev/stdout)
            
            # 3. Extraer solo el número de la memoria (Maximum resident set size)
            MEM_KB=$(grep "Maximum resident set size" "$TEMP_MEM" | awk '{print $6}')
            
            # 4. Escribir todo en una sola línea del CSV
            echo "${OUTPUT};${MEM_KB}" >> "$CSV_OUT"
        done
    fi
done

# Limpieza
rm -f "$TEMP_MEM"

echo "Experimentos completados."