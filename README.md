# Laboratorio: Árbol de Segmentos con Propagación Perezosa (_Segment Tree & Lazy Propagation_)

**Tiempo estimado:** 2 horas  
**Dominio:** Algoritmos avanzados de competencias (IOI / ICPC / Codeforces)  
**Herramientas:** C++17 (`g++` o `clang++`), `make`

---

## 1. Motivación en Programación Competitiva

Considera el siguiente problema común en competencias como la IOI o el ICPC:

> Se tiene un arreglo de $N$ enteros y se reciben $Q$ operaciones en línea. Cada operación puede ser:
>
> 1. **Actualización de rango:** Sumar un valor $v$ a todos los elementos en el intervalo $[l, r]$.
> 2. **Consulta de rango:** Calcular la suma total y el valor mínimo en el intervalo $[l, r]$.
>
> **Límites típicos:** $N \le 2 \times 10^5$, $Q \le 2 \times 10^5$, tiempo límite $= 1.0 \text{ s}$.

### ¿Por qué fallan los enfoques directos?

| Enfoque                    | Actualización         | Consulta              | Complejidad Total                                       | Veredicto                   |
| :------------------------- | :-------------------- | :-------------------- | :------------------------------------------------------ | :-------------------------- |
| **Arreglo ingenuo**        | $\mathcal{O}(N)$      | $\mathcal{O}(N)$      | $\mathcal{O}(Q \cdot N) \approx 4 \times 10^{10}$ op.   | **TLE** ($> 40 \text{ s}$)  |
| **Arreglo de diferencias** | $\mathcal{O}(1)$      | $\mathcal{O}(N)$      | $\mathcal{O}(Q \cdot N)$ (no soporta consultas mixtas)  | **TLE**                     |
| **Segment Tree + Lazy**    | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}((N + Q) \log N) \approx 4 \times 10^6$ op. | **AC** ($< 0.08 \text{ s}$) |

### Pistas/Errores Críticos que Causan WA o TLE en Concursos

1. **El mito del tamaño $2N$:** Un árbol de segmentos binario completo para $N$ elementos requiere a lo sumo $2^{\lceil \log_2 N \rceil + 1} - 1$ nodos. Cuando $N$ no es potencia de 2, el arreglo lineal que representa al árbol debe dimensionarse estrictamente en **$4N$**. Usar $2N$ produce un desbordamiento de memoria (_buffer overflow_) que deriva en **Runtime Error (SIGSEGV)**.
2. **Punteros dinámicos vs. Arreglo plano:** Construir el árbol con estructuras dinámicas y nodos (`new Node`) genera fragmentación de memoria y saltos de caché (_cache misses_), multiplicando el tiempo por 4 o 5. La representación implícita en un arreglo contiguo asegura localidad espacial.
3. **Pérdida de la pereza (_Lazy Invariant Bug_):** El error más sutil consiste en consultar los hijos de un nodo sin antes haber propagado las modificaciones pendientes acumuladas en dicho nodo.

---

## 2. Metas de Aprendizaje

Al finalizar este laboratorio serás capaz de:

1. Representar un árbol binario estático dentro de un arreglo contiguo usando indexación $1$-basada ($2u$ y $2u + 1$).
2. Construir la estructura completa en tiempo lineal $\mathcal{O}(N)$ mediante divide y vencerás.
3. Formular e implementar la técnica de **Propagación Perezosa (_Lazy Propagation_)** para aplazar actualizaciones costosas y mantener la complejidad en $\mathcal{O}(\log N)$.
4. Combinar múltiples estadísticas (suma y mínimo) en un solo nodo sin degradar la eficiencia.

---

## 3. Estructura y Geometría del Árbol

El árbol de segmentos modela el intervalo global $[0, N-1]$. Cada nodo $u$ es responsable de un rango $[tl, tr]$.

```
                    Nodo 1: [0, 7]
                   /              \
         Nodo 2: [0, 3]        Nodo 3: [4, 7]
         /            \        /            \
     Nodo 4: [0,1]   ...      ...        Nodo 7: [6,7]
     /          \                        /          \
 Nodo 8: [0]  Nodo 9: [1]            Nodo 14: [6] Nodo 15: [7]
```

### Reglas de indexación plana (Raíz en el índice 1):

- Hijo izquierdo de $u$: $2 \cdot u$
- Hijo derecho de $u$: $2 \cdot u + 1$
- Punto medio de partición: $tm = \lfloor (tl + tr) / 2 \rfloor$

### Invariantes del Nodo

Cada nodo almacena:

- `sum`: La suma de todos los elementos del subrango $[tl, tr]$.
- `min_val`: El valor mínimo en $[tl, tr]$.
- `lazy`: Valor acumulado que **ya fue aplicado a este nodo**, pero que **aún no se ha transferido a sus hijos**.

---

## 4. Dinámica y Lógica de las Operaciones

### 4.1 La Génesis del Árbol: Construcción Recursiva (`build`)

La construcción sigue una estrategia clásica de _divide y vencerás_ que crece desde las hojas hacia la raíz. La ejecución desciende bisecando continuamente el espacio hasta tocar los casos base: segmentos de longitud uno donde el límite izquierdo coincide con el derecho. En estos extremos atómicos, el nodo no hace deducciones; simplemente toma el valor original del arreglo de entrada y fija su deuda acumulada en cero.

La magia ocurre en el retorno de la recursión. Conforme las llamadas finalizan y regresamos por el árbol, los nodos padres combinan la información que acaban de reportar sus dos descendientes inmediatos. La suma del padre es la adición directa de las sumas de sus hijos, y su mínimo es el ganador entre los mínimos de ambos lados. Dado que cada arista se recorre un número constante de veces, toda la estructura de $4N$ posiciones queda armada y consistente en tiempo estrictamente lineal $\mathcal{O}(N)$.

---

### 4.2 La Filosofía del Aplazamiento: Aplicación y Descenso (`apply_tag` y `push_down`)

El principio central de la propagación perezosa es **no realizar trabajo hasta que sea estrictamente indispensable**. Cuando un nodo descubre que su intervalo completo está afectado por una modificación, no gasta tiempo visitando a sus descendientes. En su lugar, actualiza sus propias estadísticas al instante: si el rango contiene $K$ casillas y sumamos $\Delta$, su suma acumulada se incrementa en $K \times \Delta$ y su valor mínimo sube en $\Delta$. Hecho esto, el nodo anota ese mismo $\Delta$ en su etiqueta pendiente (`lazy`) y detiene la recursión. La deuda queda registrada.

Sin embargo, los secretos no pueden guardarse para siempre. En el momento en que una operación futura (sea otra actualización o una consulta) necesite examinar con precisión el interior de uno de los subárboles de ese nodo, la deuda debe saldarse. La rutina de descenso (`push_down`) entra en acción como un mensajero: verifica si el nodo actual carga una deuda pendiente no nula y, de ser así, transfiere esa misma etiqueta a sus hijos izquierdo y derecho, forzándolos a actualizar sus totales. Una vez transferida la responsabilidad hacia la siguiente generación, el nodo actual borra su etiqueta a cero, restableciendo el invariante de que sus hijos reflejan la verdad antes de que cualquiera los lea.

---

### 4.3 Mutación Eficiente: Actualización por Rango (`range_update`)

Cualquier actualización de rango en $[l, r]$ se reduce a un triaje espacial cuando visitamos un nodo con cobertura $[tl, tr]$:

- **Indiferencia absoluta (Rango disjunto):** Si el segmento del nodo queda completamente fuera de $[l, r]$, la función aborta la búsqueda en esa rama sin tocar nada.
- **Cobertura total:** Si el segmento del nodo está completamente contenido dentro del intervalo objetivo, aplicamos la técnica perezosa. El nodo absorbe el cambio mediante la aplicación de etiqueta, anota su retraso y retorna de inmediato sin molestar a sus hijos.
- **Solapamiento parcial:** Si el rango buscado cruza solo una parte del segmento del nodo, este no puede responder globalmente por una fracción de sí mismo. Primero empuja cualquier etiqueta pendiente hacia abajo para no corromper los datos de sus hijos, luego divide la orden recursivamente entre su mitad izquierda y su mitad derecha, y finalmente, al recomponerse de ambas llamadas, recalcula su propia suma y mínimo para reflejar los cambios que acaban de ocurrir abajo.

Gracias a que en cada nivel del árbol tocamos a lo sumo cuatro nodos con solapamiento parcial, el algoritmo jamás explora más de $\mathcal{O}(\log N)$ estados por actualización.

---

### 4.4 Interrogación del Espacio: Consulta Concurrente (`range_query`)

Consultar el árbol es un espejo de la actualización, con la diferencia de que no alteramos los valores definitivos. Al evaluar la intersección entre la consulta $[l, r]$ y el segmento actual $[tl, tr]$, volvemos a enfrentar las tres condiciones espaciales:

Si el nodo no tiene nada que ver con la consulta, retorna de inmediato un elemento neutro que no altere la operación global: un cero para la suma y un valor positivo astronómico (como $+\infty$) para el mínimo. Si, por el contrario, el nodo cae completamente dentro de la región solicitada, entrega de inmediato sus valores precalculados sin descender más.

Cuando la consulta corta al nodo por la mitad, no podemos confiar a ciegas en los hijos hasta asegurarnos de que no haya deudas pendientes: disparamos la propagación perezosa hacia abajo, formulamos la misma pregunta a ambos descendientes por separado y ensamblamos la respuesta final sumando las cantidades obtenidas y quedándonos con el menor de los dos mínimos parciales. La consulta responde con precisión matemática en tiempo $\mathcal{O}(\log N)$.

---

## 5. Verificación de Rendimiento y Corrección

Ejecuta el arnés de pruebas con sanitizadores:

```bash
make test
```

Los tests estresarán tu código contra:

1. Una implementación ingenua por fuerza bruta con $10\,000$ consultas aleatorias (_differential fuzzing_).
2. Desbordamientos de enteros de 32 bits mediante valores grandes en `long long`.
3. Una prueba masiva de $2 \times 10^5$ operaciones que debe completarse en menos de $0.5$ segundos.
