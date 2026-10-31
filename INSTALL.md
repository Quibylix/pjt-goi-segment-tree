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
2. Abre la terminal **UCRT64** (no la MSYS convencional) y ejecuta:

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make git
```

3. Agrega `C:\msys64\ucrt64\bin` a la variable de entorno `PATH`:
   - Busca "Editar las variables de entorno del sistema" → *Variables de entorno* → edita `Path` → añade la carpeta.
4. En MSYS2, `make` se instala como `mingw32-make`. Para que este laboratorio funcione sin cambios, crea un alias permanente:

```bash
echo 'alias make=mingw32-make' >> ~/.bashrc
source ~/.bashrc
```

Verificación (en CMD o PowerShell):

```bat
g++ --version
mingw32-make --version
git --version
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

- **`make: command not found`** (Windows): usa `mingw32-make` o completa el paso de `PATH`.
- **`error: unrecognized command line option '-fsanitize=...'`**: tu compilador no soporta los
  sanitizadores (MinGW sobre Windows). Usa la Opción 2 (WSL2) o compila con:

  ```bash
  g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -g st_test.cpp segment_tree.cpp -o segment_tree_runner
  ```

- **El warning `-Werror` detiene la compilación**: es intencional, el `Makefile` exige código
  limpio. Compila sin `-Werror` solo para depurar:
  `g++ -std=c++17 -O0 -g st_test.cpp segment_tree.cpp -o segment_tree_runner`
