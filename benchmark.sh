#!/bin/bash
#
# benchmark.sh
# Automatic execution of ./preprocess with:
#   - Methods: barrier, cond_var, busy_wait
#   - Number of threads: 1, 10, 50, 100
#   - Input files: data/in0.txt, data/in1.txt, data/in2.txt, data/in3.txt, data/in4.txt
# Each combination is executed 3 times,
# and we average real time (s), CPU (%) and RAM (KB) before exporting to CSV.
#

EXECUTABLE="./preprocess"

if [ ! -f "$EXECUTABLE" ]; then
  echo "Compiling project..."
  gcc -Wall -Wextra src/*.c -Iinclude -o preprocess -lpthread
fi

files=(data/in0.txt data/in1.txt data/in2.txt data/in3.txt data/in4.txt)
ndatos=(10     100    1000   10000  1000000)

threads=(1 10 50 100)

methods=(barrier cond_var busy_wait)

OUTPUT_CSV="results.csv"

echo "method,threads,data_size,time(s),cpu(%),ram(KB)" > "$OUTPUT_CSV"

TIME_LOG="time.tmp"

run_once() {
  local nh="$1"
  local infile="$2"
  local metodo="$3"

  # Call /usr/bin/time with format: "%e %P %M"
  #   %e → real time in seconds (float)
  #   %P → CPU percentage (e.g. "98%")
  #   %M → maximum RAM used in KB
  /usr/bin/time -f "%e %P %M" -o "$TIME_LOG" \
    "$EXECUTABLE" "$nh" "$infile" "$metodo" >/dev/null 2>&1

  # Read the three columns from time.tmp
  #   tiempo_raw  → e.g. "0.12"
  #   cpu_raw     → e.g. "97%"
  #   ram_raw     → e.g. "123456"
  read -r tiempo_raw cpu_raw ram_raw < "$TIME_LOG"

  # Strip the '%' character from cpu_raw
  cpu_val="${cpu_raw%\%}"

  # Export as global variables for the caller
  TIME_RESULT="$tiempo_raw"
  CPU_RESULT="$cpu_val"
  RAM_RESULT="$ram_raw"
}

echo "Starting benchmark. This might take several minutes..."

for metodo in "${methods[@]}"; do
  for (( i=0; i<${#files[@]}; i++ )); do
    infile="${files[i]}"
    nd="${ndatos[i]}"

    # Skip 1M rows for condition variable since it's sequential-pipeline and uses huge RAM
    if [ "$metodo" == "cond_var" ] && [ "$nd" -eq 1000000 ]; then
      echo "Skipping cond_var for 1,000,000 rows (too resource intensive/slow)"
      continue
    fi

    for nh in "${threads[@]}"; do
      echo "Testing: Method=$metodo, Threads=$nh, DataSize=$nd"
      
      sum_time=0
      sum_cpu=0
      sum_ram=0

      for rep in {1..3}; do
        run_once "$nh" "$infile" "$metodo"
        sum_time=$(echo "$sum_time + $TIME_RESULT" | bc -l)
        sum_cpu=$(echo "$sum_cpu + $CPU_RESULT"   | bc -l)
        sum_ram=$(echo "$sum_ram + $RAM_RESULT"   | bc -l)
      done

      avg_time=$(echo "scale=6; $sum_time / 3" | bc -l)
      avg_cpu=$(echo "scale=2; $sum_cpu  / 3" | bc -l)
      avg_ram=$(printf "%.0f" "$(echo "$sum_ram / 3" | bc -l)")

      echo "$metodo,$nh,$nd,$avg_time,$avg_cpu,$avg_ram" >> "$OUTPUT_CSV"
    done
  done
done

rm -f "$TIME_LOG"

echo "Benchmark finished. Results stored in: $OUTPUT_CSV"
