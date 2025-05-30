# Sistema Paralelo de Preprocesamiento de Datos.

### Universidad del Magdalena - Ingeniería de Sistemas.
**Curso:** Sistemas Operativos.
**Tema:** Sincronización y Procesamiento Paralelo con pthread.
**Autores:** Jiménez Rossimar y Rincones Carlos.

---

## Objetivo.

Diseñar e implementar una solución multihilo que realice, en paralelo, tareas de **preprocesamiento de datos textuales**, aplicando técnicas comunes de NLP (Procesamiento de Lenguaje Natural), utilizando métodos de sincronización como:

- Barreras.
- Variables de condición.
- Espera activa.

---

## Técnicas de Preprocesamiento Usadas.

Cada hilo aplica las siguientes técnicas sobre sus filas de la matriz cargadas en ram con anterioridad:

1. Conversión a minúsculas.
2. Eliminación de signos de puntuación.
3. Eliminación de números.
4. Eliminación de stopwords.  

---

## Arquitectura General del Sistema.

El sistema se basa en un modelo de procesamiento por bloques:

- Los datos se representan como una matriz `N x M`, donde cada fila contiene un comentario.
- Cada hilo procesa una o varias filas de esta matriz por ronda.
- De este problema se implementarán tres versiones de la solución para poder compararlas en ciertos aspectos, las versiones de la solución se harán cada uno con:

  - **Barrera.**
  - **Variable de condición.**
  - **Espera activa.**

- Debido a la carga computacional que genera la creación de los hilos, se mantiene la persistencia de los hilos durante todas las rondas.
