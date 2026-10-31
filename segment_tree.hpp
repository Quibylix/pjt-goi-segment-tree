#ifndef SEGMENT_TREE_HPP
#define SEGMENT_TREE_HPP

#include <cstddef>
#include <vector>

class SegmentTree {
public:
    struct QueryResult {
        long long sum;
        long long min_val;
    };

    // Constructor vacío
    SegmentTree() : n_(0) {}

    // Constructor que inicializa y construye el árbol a partir de un arreglo base
    explicit SegmentTree(const std::vector<long long>& data);

    // Inicializa el árbol con los datos de entrada en O(N)
    void build(const std::vector<long long>& data);

    // Actualización de rango en [l, r]: suma 'delta' a cada elemento en O(log N)
    // Coordenadas basadas en 0: 0 <= l <= r < n
    void range_update(size_t l, size_t r, long long delta);

    // Consulta de rango en [l, r]: retorna {suma, mínimo} en O(log N)
    // Coordenadas basadas en 0: 0 <= l <= r < n
    QueryResult range_query(size_t l, size_t r);

    // Acceso al tamaño lógico del arreglo administrado
    size_t size() const noexcept;

private:
    struct Node {
        long long sum{0};
        long long min_val{0};
        long long lazy{0};
    };

    size_t n_{0};
    std::vector<Node> tree_;

    // Recalcula la suma y el mínimo del nodo u a partir de sus hijos izquierdo y derecho
    void push_up(size_t u);

    // Aplica directamente un incremento 'delta' al nodo u que cubre [tl, tr]
    void apply_tag(size_t u, size_t tl, size_t tr, long long delta);

    // Transfiere la etiqueta diferida de 'u' hacia sus hijos inmediatos
    void push_down(size_t u, size_t tl, size_t tr);

    void build_internal(size_t u, size_t tl, size_t tr, const std::vector<long long>& data);

    void update_internal(size_t u, size_t tl, size_t tr, size_t l, size_t r, long long delta);

    QueryResult query_internal(size_t u, size_t tl, size_t tr, size_t l, size_t r);

    void check_range(size_t l, size_t r) const;
};

#endif // SEGMENT_TREE_HPP
