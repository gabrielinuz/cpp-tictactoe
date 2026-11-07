En los entornos de escritorio modernos de GNU/Linux (como GNOME en Debian), la ejecución directa de archivos binarios con doble clic suele estar bloqueada por motivos de seguridad. Además, la extensión `.app` es propia del ecosistema de macOS; en Linux, el estándar del sistema de ventanas (freedesktop.org) utiliza archivos **`.desktop`** para crear lanzadores visuales.

La solución más elegante, que además te permitirá lucir el ícono SVG que creaste, es armar un lanzador nativo.

### 1. Crear el archivo Lanzador

En el directorio raíz de tu proyecto (o en tu Escritorio), crea un archivo de texto plano llamado `tateti.desktop` y pega esta configuración. Asegúrate de reemplazar `/ruta/absoluta/a/tu/proyecto/` por la ruta real donde tenés tu carpeta:

```ini
[Desktop Entry]
Version=1.0
Type=Application
Name=Ta Te Ti
Comment=Videojuego Tic-Tac-Toe bajo el modelo GREENSOFT
Exec=/ruta/absoluta/a/tu/proyecto/bin/tictactoe
Icon=/ruta/absoluta/a/tu/proyecto/media/icon.svg
Terminal=false
Categories=Game;BoardGame;

```

* **`Terminal=false`**: Es la instrucción clave que evita que se abra una ventana negra de consola por detrás del juego.
* **`Exec` y `Icon**`: Requieren rutas absolutas para que el sistema operativo sepa exactamente dónde encontrar el binario y tu ícono, independientemente de desde dónde lances el acceso directo.

### 2. Otorgar permisos de ejecución

Para que Debian reconozca este archivo como un lanzador legítimo y no como un simple archivo de texto, debes darle permisos de ejecución. Abre una terminal en la carpeta donde guardaste el `tateti.desktop` y ejecuta:

```bash
chmod +x tateti.desktop

```

### 3. Autorizar el Lanzador (Solo en GNOME)

Si utilizas el entorno por defecto de Debian (GNOME) y colocaste este archivo en tu Escritorio, al hacerle doble clic por primera vez te aparecerá un cartel de "Lanzador de aplicación no confiable". Simplemente haz clic derecho sobre el archivo `tateti.desktop` y selecciona **"Permitir lanzar"** (Allow launching).

En ese instante, el archivo tomará el nombre limpio "Ta Te Ti", adoptará tu ícono vectorial verde, y al hacerle doble clic ejecutará la interfaz de FLTK de forma totalmente nativa.

## ¿Qué pasa si quiero usar un método estándar y no rutas absolutas?
El estándar freedesktop.org (XDG) y el Filesystem Hierarchy Standard (FHS) de GNU/Linux están diseñados exactamente para resolver este problema. No necesitas codificar rutas absolutas; si colocas los archivos en los directorios del sistema correctos, el entorno de escritorio los descubrirá y resolverá por sí mismo.

Al instalar un programa de forma manual (sin un gestor de paquetes como `apt` o `pacman`), la convención estricta es utilizar el prefijo `/usr/local/` para no interferir con los archivos propios de la distribución.

Este es el archivo **`tictactoe.desktop`** genérico. Nota que `Exec` e `Icon` ahora solo tienen el nombre base, sin rutas y sin extensión para el ícono:

```ini
[Desktop Entry]
Version=1.0
Type=Application
Name=Ta Te Ti
Comment=Videojuego Tic-Tac-Toe bajo el modelo GREENSOFT
Exec=tictactoe
Icon=tictactoe
Terminal=false
Categories=Game;BoardGame;

```

* **`Exec=tictactoe`**: El sistema buscará el ejecutable dentro de la variable de entorno global `$PATH` (la cual incluye `/usr/local/bin/`).
* **`Icon=tictactoe`**: El sistema buscará automáticamente un archivo llamado `tictactoe.svg` o `tictactoe.png` dentro de las carpetas estándar de íconos del sistema.

Para automatizar la distribución, este es el archivo **`install.sh`** documentado. Debe ejecutarse en el mismo directorio donde se encuentra el código fuente ya compilado:

```bash
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
cp ./bin/tictactoe /usr/local/bin/
chmod +x /usr/local/bin/tictactoe

# 3. Instalación del Ícono Vectorial (SVG)
# Se ubica en la jerarquía 'hicolor', que es el tema de fallback universal en GNU/Linux.
echo "-> Registrando ícono..."
mkdir -p /usr/share/icons/hicolor/scalable/apps/
cp ./media/icon.svg /usr/share/icons/hicolor/scalable/apps/tictactoe.svg
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

```

El parámetro `-p` (que significa *parents* o carpetas padre) es una característica de seguridad y conveniencia del comando `mkdir` que realiza exactamente dos acciones:

1. **Evita errores si el directorio ya existe:** Si la ruta completa `/usr/share/icons/hicolor/scalable/apps/` ya está creada en el sistema (lo cual es muy común en distribuciones con interfaz gráfica), el comando simplemente pasa de largo. No modifica, no sobrescribe y no borra el contenido preexistente, ni tampoco frena el script con un mensaje de error (algo que sí haría un `mkdir` estándar sin el `-p`).
2. **Crea directorios intermedios faltantes:** Si la ruta no existe, o si falta alguna carpeta en el medio (por ejemplo, si existe `hicolor/` pero no la subcarpeta `scalable/`), el comando crea todo el "árbol" de carpetas de una sola pasada.

Es la práctica estándar y más segura en los scripts de instalación (`install.sh`), ya que te garantiza de forma idempotente que el directorio de destino estará disponible para el comando `cp` en la siguiente línea, sin poner en riesgo los archivos del sistema operativo.

Si en el futuro decides crear un script de desinstalación (`uninstall.sh`), simplemente debes eliminar esos tres archivos exactos (`rm /usr/local/bin/tictactoe`, etc.) y volver a ejecutar la actualización de las cachés al final. Esta estructura es la misma que utilizan las aplicaciones profesionales open-source al compilarse desde el código fuente mediante herramientas como `make install`. 

Aquí tienes el script `uninstall.sh` estructurado con el mismo rigor y documentación que el instalador, asegurando que no quede ningún rastro del juego en el sistema.

```bash
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

```

### Detalles de la implementación:

* **Uso de `rm -f` (force):** Se utiliza el parámetro `-f` para que el script no se detenga a preguntar por confirmaciones interactivas ni arroje errores molestos si el archivo ya había sido borrado previamente por el usuario.
* **Limpieza sin residuos:** Al seguir estrictamente el estándar FHS (Filesystem Hierarchy Standard), sabemos exactamente dónde fue a parar cada byte durante la instalación, evitando el problema clásico de Windows donde los desinstaladores dejan carpetas vacías o registros huérfanos.

Al igual que con el instalador, recuerda otorgarle permisos de ejecución (`chmod +x uninstall.sh`) antes de correrlo con `sudo`.