#!/usr/bin/env bash
set -euo pipefail

# Directorios base
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Defaults (se usan si no pasas args)
PERCENTS=(0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0)
PATHS=("${ROOT_DIR}/textos/english.1024MB" "${ROOT_DIR}/textos/sources.200MB")

if [[ $# -gt 0 ]]; then
  # Si hay args, parseamos y SOBREESCRIBIMOS los defaults
  PERCENTS=()
  PATHS=()
  seen_path=0
  while (( "$#" )); do
    arg="$1"
    if [[ $seen_path -eq 1 ]]; then
      PATHS+=("$arg")
    else
      if [[ -e "$arg" ]]; then
        seen_path=1
        PATHS+=("$arg")
      elif [[ "$arg" =~ ^([0-9]*\.)?[0-9]+$ ]]; then
        PERCENTS+=("$arg")
      else
        echo "Argumento inválido: '$arg' (ni ruta existente ni número)."
        exit 2
      fi
    fi
    shift
  done
fi

# Validaciones finales
if [[ ${#PERCENTS[@]} -eq 0 ]]; then
  echo "Debes proporcionar al menos un porcentaje (o usar los defaults)."
  exit 3
fi
if [[ ${#PATHS[@]} -eq 0 ]]; then
  echo "Debes proporcionar al menos un archivo o directorio (o usar los defaults)."
  exit 4
fi

# Validar rango (0,1]
for p in "${PERCENTS[@]}"; do
  # p > 0  y  p <= 1
  awk -v x="$p" 'BEGIN{ if(!(x>0 && x<=1)) exit 1 }' || {
    echo "Porcentaje fuera de rango (0,1]: $p"
    exit 5
  }
done

# Localizar ejecutable de subset
MK_SUBSET="${ROOT_DIR}/build/release/apps/archivos_subset"
if ! [[ -x "$MK_SUBSET" ]]; then
  echo "ERROR: archivos_subset.out no encontrado. Compila con: make archivos_subset.out"
  exit 6
fi

# Expandir directorios
FILES=()
for p in "${PATHS[@]}"; do
  if [[ -d "$p" ]]; then
    while IFS= read -r -d '' f; do FILES+=("$f"); done < <(find "$p" -type f -print0)
  elif [[ -f "$p" ]]; then
    FILES+=("$p")
  else
    echo "Saltando ruta no regular: $p"
  fi
done
if [[ ${#FILES[@]} -eq 0 ]]; then
  echo "No hay archivos que procesar."
  exit 7
fi

echo "Percents: ${PERCENTS[*]}"
echo "Total files: ${#FILES[@]}"

for f in "${FILES[@]}"; do
  base="$(basename -- "$f")"
  dir="$(dirname -- "$f")"
  for pct in "${PERCENTS[@]}"; do
    out="${dir}/${pct} ${base}"
    # if [[ -f "$out" ]]; then
    #   echo "[=] Ya existe, saltando: $out"
    #   continue
    # fi
    echo "[*] ${pct} -> ${f}"
    "$MK_SUBSET" "$f" "$pct"
  done
done

echo "Done."
