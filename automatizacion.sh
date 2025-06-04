#!/bin/bash
#
# benchmark.sh
# Ejecución automática de ./prueba con:
#   - Métodos: barrera, variable, espera
#   - Número de hilos: 1, 10, 50, 100
#   - Archivos de entrada: in0.txt, in1.txt, in2.txt, in3.txt, in4.txt
# Cada combinación se ejecuta 3 veces, 
# y se promedian tiempo (s), CPU (%) y RAM (KB) antes de volcar a CSV.
#

# Ruta al ejecutable
EXECUTABLE="./prueba"

# Lista de archivos de entrada y su correspondiente número de datos
files=(in0.txt in1.txt in2.txt in3.txt in4.txt)
ndatos=(10     100    1000   10000  1000000)

# Número de hilos a probar
threads=(1 10 50 100)

# Métodos a probar
methods=(barrera variable espera)

# Nombre del archivo CSV de salida
OUTPUT_CSV="resultados.csv"

# Header del CSV
echo "metodo,nhilos,ndatos,tiempo(s),cpu(%),ram(KB)" > "$OUTPUT_CSV"

# Archivo temporal para capturar la salida de /usr/bin/time
TIME_LOG="time.tmp"

# Función para ejecutar una sola corrida y devolver tiempo, cpu y ram
# Usa: run_once <n_hilos> <input_file> <metodo>
# Lee de time.tmp:  %e  %P  %M
run_once() {
  local nh="$1"
  local infile="$2"
  local metodo="$3"

  # Llama a /usr/bin/time con formato: "%e %P %M"
  #   %e → tiempo real en segundos (float)
  #   %P → porcentaje de CPU (p. ej "98%")
  #   %M → máximo de RAM usada en KB
  /usr/bin/time -f "%e %P %M" -o "$TIME_LOG" \
    "$EXECUTABLE" "$nh" "$infile" "$metodo" >/dev/null 2>&1

  # Leer las tres columnas de time.tmp
  #   tiempo_raw → p. ej "0.123"
  #   cpu_raw    → p. ej "97%"
  #   ram_raw    → p. ej "123456"
  read -r tiempo_raw cpu_raw ram_raw < "$TIME_LOG"

  # Quitar el carácter '%' de cpu_raw
  cpu_val="${cpu_raw%\%}"

  # Exportar como variables globales para usar en el caller
  TIME_RESULT="$tiempo_raw"
  CPU_RESULT="$cpu_val"
  RAM_RESULT="$ram_raw"
}

# -----------------------------------------------------
# Bucle principal: métodos × hilos × archivos × 3 repeticiones
# -----------------------------------------------------
for metodo in "${methods[@]}"; do
  for (( i=0; i<${#files[@]}; i++ )); do
    infile="${files[i]}"
    nd="${ndatos[i]}"

    for nh in "${threads[@]}"; do
      # Inicializar sumas en cero
      sum_time=0
      sum_cpu=0
      sum_ram=0

      # Ejecutar 3 veces y acumular
      for rep in {1..3}; do
        run_once "$nh" "$infile" "$metodo"
        # TIME_RESULT, CPU_RESULT y RAM_RESULT quedan poblados
        # Acumular (como floats)
        sum_time=$(echo "$sum_time + $TIME_RESULT" | bc -l)
        sum_cpu=$(echo "$sum_cpu + $CPU_RESULT"   | bc -l)
        sum_ram=$(echo "$sum_ram + $RAM_RESULT"   | bc -l)
      done

      # Calcular promedios (división por 3)
      avg_time=$(echo "scale=6; $sum_time / 3" | bc -l)
      avg_cpu=$(echo "scale=2; $sum_cpu  / 3" | bc -l)
      # Promedio de RAM (puede redondearse a entero)
      avg_ram=$(printf "%.0f" "$(echo "$sum_ram / 3" | bc -l)")

      # Escribir línea en CSV
      echo "$metodo,$nh,$nd,$avg_time,$avg_cpu,$avg_ram" >> "$OUTPUT_CSV"
    done
  done
done

# Limpiar archivo temporal
rm -f "$TIME_LOG"

echo "Benchmark finalizado. Resultados en: $OUTPUT_CSV"


