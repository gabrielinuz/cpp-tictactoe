#!/bin/bash

# 1. Validación de privilegios
# Eliminar archivos de los directorios /usr/ requiere acceso de superusuario.
if [ "$EUID" -ne 0 ]; then
  echo "[Error] Por favor, ejecuta este desinstalador como root: sudo ./uninstall.sh"
  exit 1
fi

echo "Iniciando desinstalación de Ta Te Ti del sistema..."

# 2. Eliminación del Binario
echo "-> Eliminando ejecutable..."
rm -f /usr/local/bin/tictactoe

# 3. Eliminación del Ícono Vectorial
echo "-> Eliminando ícono del sistema..."
rm -f /usr/share/icons/hicolor/scalable/apps/tictactoe.svg

# 4. Eliminación del Lanzador
echo "-> Eliminando acceso del menú de aplicaciones..."
rm -f /usr/share/applications/tictactoe.desktop

# 5. Actualización de cachés del entorno de escritorio
# Es vital refrescar las bases de datos para que el ícono desaparezca visualmente del menú de inmediato.
echo "-> Actualizando cachés del sistema de ventanas..."
update-desktop-database /usr/share/applications/ || true
gtk-update-icon-cache -f -t /usr/share/icons/hicolor/ || true

echo "Desinstalación completada con éxito. El sistema ha quedado limpio."