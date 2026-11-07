#!/bin/bash

# 1. Validación de privilegios
# La escritura en los directorios /usr/ requiere acceso de superusuario.
if [ "$EUID" -ne 0 ]; then
  echo "[Error] Por favor, ejecuta este instalador como root: sudo ./install.sh"
  exit 1
fi

echo "Iniciando instalación de Ta Te Ti en el sistema..."

# 2. Instalación del Binario
# /usr/local/bin es el estándar para ejecutables compilados por el administrador local.
echo "-> Copiando ejecutable..."
cp ../bin/tictactoe /usr/local/bin/
chmod +x /usr/local/bin/tictactoe

# 3. Instalación del Ícono Vectorial (SVG)
# Se ubica en la jerarquía 'hicolor', que es el tema de fallback universal en GNU/Linux.
echo "-> Registrando ícono..."
mkdir -p /usr/share/icons/hicolor/scalable/apps/
cp ../media/tictactoe.svg /usr/share/icons/hicolor/scalable/apps/tictactoe.svg
chmod 644 /usr/share/icons/hicolor/scalable/apps/tictactoe.svg

# 4. Instalación del Lanzador
# /usr/share/applications es el directorio global que leen GNOME, KDE, XFCE, etc.
echo "-> Creando acceso en el menú de aplicaciones..."
cp ./tictactoe.desktop /usr/share/applications/
chmod 644 /usr/share/applications/tictactoe.desktop

# 5. Actualización de cachés del entorno de escritorio
# Obligamos al sistema a regenerar el menú y los íconos sin necesidad de reiniciar.
echo "-> Actualizando cachés del sistema de ventanas..."
update-desktop-database /usr/share/applications/ || true
gtk-update-icon-cache -f -t /usr/share/icons/hicolor/ || true

echo "Instalación completada con éxito. El juego está disponible en su menú."