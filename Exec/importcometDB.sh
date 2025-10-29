#!/bin/bash

# Uso: ./csv_to_sqlite.sh archivo.csv base_de_datos.db nombre_tabla

CSV_FILE="$1"
DB_FILE="$2"
TABLE_NAME="$3"

if [[ -z "$CSV_FILE" || -z "$DB_FILE" || -z "$TABLE_NAME" ]]; then
  echo "Uso: $0 archivo.csv base_de_datos.db nombre_tabla">>/dev/tty1
  sleep 4
  exit 1
fi

if [[ ! -f "$CSV_FILE" ]]; then
  echo "Error: el fichero $CSV_FILE no existe.">>/dev/tty1
sleep 4
  exit 1
fi

# Leer encabezados (primera línea)
HEADER=$(head -n 1 "$CSV_FILE")
IFS=',' read -r -a COLUMNS <<< "$HEADER"

# Leer segunda línea (para inferir tipo)
SECOND_LINE=$(sed -n '2p' "$CSV_FILE")
IFS=',' read -r -a SAMPLE <<< "$SECOND_LINE"

# Crear sentencia CREATE TABLE
SQL="CREATE TABLE IF NOT EXISTS $TABLE_NAME ("
for i in "${!COLUMNS[@]}"; do
  RAW_NAME="${COLUMNS[$i]}"
  # Limpiar el nombre:
  # - Quitar espacios iniciales/finales
  # - Quitar comillas y retornos de carro
  # - Eliminar caracteres no alfanuméricos excepto '_'
  # - Si empieza con número, prefijar con 'c'
  CLEAN_NAME=$(echo "$RAW_NAME" \
    | sed 's/^[[:space:]]*//;s/[[:space:]]*$//' \
    | tr -d '\r' \
    | tr -d '"' \
    | tr -d "'" \
    | sed 's/[^[:alnum:]_]//g')
  if [[ "$CLEAN_NAME" =~ ^[0-9] ]]; then
    CLEAN_NAME="c$CLEAN_NAME"
  fi

  VALUE=${SAMPLE[$i]}
  # Inferir tipo básico
  if [[ "$VALUE" =~ ^[0-9]+$ ]]; then
    TYPE="INTEGER"
  elif [[ "$VALUE" =~ ^[0-9]+\.[0-9]+$ ]]; then
    TYPE="REAL"
  else
    TYPE="TEXT"
  fi

  SQL+="$CLEAN_NAME $TYPE"
  [[ $i -lt $((${#COLUMNS[@]}-1)) ]] && SQL+=", "
done
SQL+=");"
DROP_TABLE="DROP TABLE IF EXISTS $TABLE_NAME;";
echo "$DROP_TABLE" | sqlite3 "$DB_FILE"
# Crear tabla en SQLite
echo "Creando tabla '$TABLE_NAME' en $DB_FILE..."
echo "$SQL" | sqlite3 "$DB_FILE"

# Importar datos
echo "Importando datos desde $CSV_FILE..."
sqlite3 "$DB_FILE" <<EOF
.mode csv
.import $CSV_FILE $TABLE_NAME
UPDATE $TABLE_NAME SET full_name=TRIM(full_name);
EOF

echo "✅ Tabla '$TABLE_NAME' creada e importada correctamente."
