# Instalación de Requerimientos

Este laboratorio es 100% C++ de consola: no hay dependencias de librerías externas. Solo se necesita:

| Requerimiento       | Versión mínima | Uso en el laboratorio                          |
| :------------------ | :------------- | :--------------------------------------------- |
| Compilador C++ (g++ o clang++) | C++17          | Compilar el scaffold y el arnés de pruebas      |
| `make`              | Cualquiera     | Automatizar compilación, pruebas y limpieza     |
| `git`               | Cualquiera     | Clonar el repositorio y registrar tu progreso   |

---

## Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install -y build-essential git
```

`build-essential` instala `g++`, `make` y las cabeceras estándar de C.

Verificación:

```bash
g++ --version    # Debe reportar 9.0 o superior (soporte completo de C++17)
make --version
git --version
```

Si tu versión de `g++` es demasiado antigua (Ubuntu 18.04 o anterior), instala una más reciente:

```bash
sudo apt install -y g++-11
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100
```

### Fedora / Arch

```bash
# Fedora
sudo dnf install -y gcc-c++ make git

# Arch
sudo pacman -S --needed gcc make git
```

---

## Windows

Hay tres opciones. Elige **una**.

### Opción 1: MSYS2 con MinGW-w64 (recomendada)

1. Descarga e instala MSYS2 desde [msys2.org](https://www.msys2.org/).

   Deja la carpeta de instalación por defecto (`C:\msys64`). Si instalaste en otra
   carpeta, anótala: la necesitarás en el paso 3.

2. Abre la terminal **UCRT64** (no la MSYS convencional). Para abrirla:

   - Pulsa la tecla `Windows` del teclado.
   - Escribe `UCRT64` y confirma con `Enter`.
   - Se abrirá una ventana negra (la terminal): ahí se escribe una línea a la vez y
     se confirma con `Enter`.

   Ejecuta en esa terminal:

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make git
```

3. Agrega la carpeta `C:\msys64\ucrt64\bin` al **PATH** de Windows.

   **¿Qué es el PATH?** Es la lista de carpetas donde Windows busca los programas
   cuando los llamas por su nombre. Sin este paso, escribir `g++` en una terminal
   da el error *"...is not recognized as an internal or external command"*, aunque
   el programa sí esté instalado.

   Elige **una** de las dos formas:

   **Forma A — con ventanas (recomendada):**
   1. Pulsa la tecla `Windows` y escribe `variables de entorno`.
   2. En los resultados, haz clic en **"Editar las variables de entorno del sistema"**
      (así, exactamente; si apareciera solo "Editar variables de entorno", también sirve).
   3. En la ventana que se abre, haz clic en el botón **"Variables de entorno..."**.
   4. En la mitad de abajo ("Variables del sistema"), haz clic sobre la fila `Path`
      (una sola vez, solo selección) y luego en el botón **"Editar..."**.
   5. En la nueva ventana, haz clic en **"Nuevo"** y escribe exactamente:
      `C:\msys64\ucrt64\bin`
      (si instalaste MSYS2 en otra carpeta en el paso 1, usa `<esacarpeta>\ucrt64\bin`).
   6. Acepta con **"Aceptar"** en **las tres ventanas** — si solo cierras con la `X`,
      el cambio no se guarda.
   7. Opcional, para comprobarlo: abre el Explorador de archivos, pega
      `C:\msys64\ucrt64\bin` en la barra de direcciones y pulsa `Enter`. Debes ver
      un archivo llamado `g++.exe`.

   **Forma B — con PowerShell:**
   1. Pulsa la tecla `Windows`, escribe `powershell` y confirma con `Enter`.
   2. Copia y pega estas dos líneas (clic derecho pega en PowerShell), una a una:

   ```powershell
   $actual = [Environment]::GetEnvironmentVariable('Path', 'User')
   [Environment]::SetEnvironmentVariable("Path", "$actual;C:\msys64\ucrt64\bin", 'User')
   ```

   3. Si no aparece ningún mensaje de error, el cambio quedó guardado.

   **Importante:** cualquier terminal abierta **antes** de este cambio sigue usando
   el PATH viejo. Ciérrala por completo y ábrete una nueva en el paso de verificación.

4. Verificación. Abre una terminal **nueva**:
   - Pulsa la tecla `Windows`, escribe `cmd` y confirma con `Enter`.
   - Escribe estos comandos, uno por línea:

```bat
g++ --version
mingw32-make --version
git --version
```

   Debes ver líneas de versión de cada uno:

   ```
   g++.exe (GCC) 14.x.x
   GNU Make 4.x
   git version 2.x
   ```

   Si alguno falla con *"...is not recognized"*, revisa la sección
   **Solución de problemas** al final de este documento.

5. En MSYS2, `make` se instala como `mingw32-make`. Para que este laboratorio funcione sin cambios, crea un alias permanente:

```bash
echo 'alias make=mingw32-make' >> ~/.bashrc
source ~/.bashrc
```

> **Nota sobre sanitizadores:** la versión de MinGW-w64 de GCC no incluye los runtime de
> AddressSanitizer/UBSan. `make debug` y las pruebas normales funcionan, pero el target
> `make test` (que usa `-fsanitize=address,undefined`) requiere la Opción 2 o 3.

### Opción 2: WSL2 (Ubuntu sobre Windows)

Ideal si quieres exactamente el comportamiento de Linux (incluidos los sanitizadores):

```powershell
wsl --install
```

Reinicia, abre la terminal de Ubuntu y ejecuta el bloque de comandos de la sección Linux.

### Opción 3: Visual Studio + Developer Command Prompt

Si prefieres compilar con el toolchain de Microsoft:

1. Instala **Visual Studio 2022** con la carga de trabajo **Desarrollo para el escritorio con C++**.
2. Prefiere la Opción 1 u 2 para usar el `Makefile` sin cambios; VS usa `cl.exe` y no consume `make` directamente.

---

## Creación del laboratorio (todas las plataformas)

```bash
git clone <URL-del-repositorio>
cd segment-tree-lab
make test
```

Comandos disponibles:

| Comando           | Descripción                                     |
| :---------------- | :---------------------------------------------- |
| `make` / `make test` | Compila con ASan y UBSan y ejecuta la suite |
| `make debug`      | Genera un binario `-O0` para depurar con gdb     |
| `make valgrind`   | Ejecuta la suite bajo Valgrind (Linux únicamente)|
| `make clean`      | Borra los binarios generados                     |

En Windows con MSYS2/MinGW, sustituye `make` por `mingw32-make` (o usa el alias).

Para depurar directamente (sin `make`):

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 -g st_test.cpp segment_tree.cpp -o segment_tree_runner
./segment_tree_runner
```

---

## Solución de problemas

- **`g++` no se reconoce en Windows (Forma A/B del PATH):**
  1. ¿Cerraste TODAS las terminales y abriste una nueva? El PATH no se recarga en
     terminales que ya estaban abiertas.
  2. Verifica que la carpeta existe: en el Explorador de archivos pega
     `C:\msys64\ucrt64\bin` en la barra de direcciones. Si la carpeta no existe,
     tu instalación de MSYS2 está en otra ubicación o el paso 2 no terminó;
     reinstala/instala GCC con la UCRT64 del paso 2.
  3. Reabre "Editar variables de entorno del sistema" y comprueba que `Path`
     contenga exactamente `C:\msys64\ucrt64\bin` (una fila propia, sin comillas,
     sin espacios finales).
- **`make: command not found`** (Windows): usa `mingw32-make` o completa el paso de `PATH`.
- **`error: unrecognized command line option '-fsanitize=...'`**: tu compilador no soporta los
  sanitizadores (MinGW sobre Windows). Usa la Opción 2 (WSL2) o compila con:

  ```bash
  g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -g st_test.cpp segment_tree.cpp -o segment_tree_runner
  ```

- **El warning `-Werror` detiene la compilación**: es intencional, el `Makefile` exige código
  limpio. Compila sin `-Werror` solo para depurar:
  `g++ -std=c++17 -O0 -g st_test.cpp segment_tree.cpp -o segment_tree_runner`
