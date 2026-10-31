#include "segment_tree.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

// Scaffold del ejercicio: los cuerpos están vacíos a propósito.
// Los parámetros se marcan [[maybe_unused]] y los retornos devuelven
// QueryResult{} solo para que el arnés de pruebas compile antes de
// implementar; quitar dichos marcadores al escribir cada cuerpo.

// Constructor que inicializa y construye el árbol a partir de un arreglo base
SegmentTree::SegmentTree(const std::vector<long long>& data) {
    build(data);
}

void SegmentTree::build([[maybe_unused]] const std::vector<long long>& data) {
}

void SegmentTree::range_update([[maybe_unused]] size_t l, [[maybe_unused]] size_t r, [[maybe_unused]] long long delta) {
}

SegmentTree::QueryResult SegmentTree::range_query([[maybe_unused]] size_t l, [[maybe_unused]] size_t r) {
    return QueryResult{};
}

size_t SegmentTree::size() const noexcept {
    return n_;
}

void SegmentTree::push_up([[maybe_unused]] size_t u) {
}

void SegmentTree::apply_tag([[maybe_unused]] size_t u, [[maybe_unused]] size_t tl, [[maybe_unused]] size_t tr, [[maybe_unused]] long long delta) {
}

void SegmentTree::push_down([[maybe_unused]] size_t u, [[maybe_unused]] size_t tl, [[maybe_unused]] size_t tr) {
}

void SegmentTree::build_internal([[maybe_unused]] size_t u, [[maybe_unused]] size_t tl, [[maybe_unused]] size_t tr, [[maybe_unused]] const std::vector<long long>& data) {
}

void SegmentTree::update_internal([[maybe_unused]] size_t u, [[maybe_unused]] size_t tl, [[maybe_unused]] size_t tr, [[maybe_unused]] size_t l, [[maybe_unused]] size_t r, [[maybe_unused]] long long delta) {
}

SegmentTree::QueryResult SegmentTree::query_internal([[maybe_unused]] size_t u, [[maybe_unused]] size_t tl, [[maybe_unused]] size_t tr, [[maybe_unused]] size_t l, [[maybe_unused]] size_t r) {
    return QueryResult{};
}

void SegmentTree::check_range([[maybe_unused]] size_t l, [[maybe_unused]] size_t r) const {
}
