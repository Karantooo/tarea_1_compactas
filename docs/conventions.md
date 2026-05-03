# Convenciones de este proyecto

## Estructura de carpetas

- `include/`: headers públicos compartidos por ejecutables y librerías.
- `src/`: implementación de librerías compartidas (sin `main`).
- `apps/`: ejecutables del proyecto (archivos con `main`).
- `docs/`: guías y convenciones de proyecto.
- `cmake/`: módulos reutilizables de CMake.

## Convenciones de nombres

- Archivos `.cpp` y `.hpp` en `snake_case`.
- Namespaces en minúscula (`core`, `io`, `algo`).
- Clases en `PascalCase`.
- Funciones y variables en `snake_case`.
- Constantes en `UPPER_SNAKE_CASE`.

## Convenciones para targets CMake

- Librerías: nombres simples por dominio (`core`, `net`, `parser`).
- Ejecutables de `apps/`: sufijo `_app` para distinguir binarios de librerías.

## Regla práctica

- Si un `.cpp` no tiene `main`, va a `src/` y forma parte de una librería.
- Si necesita `main`, va a `apps/`.
- Un ejecutable siempre enlaza una o más librerías, evitando duplicar código.

## Commits recomendados

- `feat`: nueva funcionalidad.
- `build`: cambios de compilación/CMake.
- `docs`: documentación.
- `refactor`: reestructuración sin cambiar comportamiento.
- `fix`: corrección de bug.
- `test`: pruebas nuevas o corregidas.
