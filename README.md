# Implementación Experimental de FM-index

Comparación experimental de múltiples implementaciones de FM-index utilizando distintas estrategias para calcular la operación `Occ(v,k)` mediante consultas `rank` sobre la Transformada de Burrows-Wheeler (BWT).

El proyecto fue desarrollado para la asignatura **Estructuras de Datos Compactas** de la Universidad de Concepción.

## Implementaciones

El proyecto compara cuatro implementaciones:

* `lab_wt` → Implementación propia de Wavelet Tree desarrollada a partir del laboratorio
* `sdsl_blcd` → Wavelet Tree balanceado utilizando SDSL
* `sdsl_huff_rrr` → Wavelet Tree comprimido con Huffman y bitvectors RRR utilizando SDSL
* `brute` → Implementación fuerza bruta mediante recorrido lineal sobre la BWT

## Requisitos

El proyecto fue desarrollado y probado en Ubuntu utilizando:

* `g++ 13.3.0`
* `C++17`
* `CMake >= 3.16`

Además, es necesario instalar previamente la librería SDSL-lite:

[https://github.com/simongog/sdsl-lite](https://github.com/simongog/sdsl-lite)

## Configuración de datasets

Antes de ejecutar los experimentos, los textos deben descargarse y ubicarse en:

```text
./datasets/experiment_txt_fijo
```

Los experimentos fueron realizados utilizando textos del corpus Pizza&Chili.

## Compilación

Compilar el proyecto en modo release:

```bash
cmake --preset release
cmake --build --preset release
```

Luego ejecutar el script de configuración:

```bash
./setup
```

## Ejecución de experimentos

### Experimento 1

Variación del tamaño del texto con patrón fijo:

```bash
./exp_txt_fijo.sh
```

### Experimento 2

Variación del largo del patrón con texto fijo:

```bash
./exp_pat_fijo.sh
```

Los scripts generan automáticamente archivos CSV con los tiempos de ejecución y mediciones de memoria.

## Estructura del proyecto

```text
.
├── CMakeLists.txt
├── CMakePresets.json
├── cmake/
├── include/
├── src/
├── apps/
├── datasets/
├── results/
└── docs/
```

## Notas

* Las mediciones de tiempo se realizan utilizando `std::chrono`.
* Las mediciones de memoria utilizan `/usr/bin/time -v` extrayendo el valor `Maximum resident set size`.
* Cada experimento se repite 32 veces y se calculan automáticamente promedios y varianzas.

## Autores

* Carlos Tomás Álvarez Norambuena
* Benjamín Alonso Espinoza Henríquez
