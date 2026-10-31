#include "segment_tree.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <cassert>
#include <chrono>

// Colores ANSI para terminal
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_YELLOW  "\033[33m"

static int g_tests_passed = 0;
static int g_tests_total = 0;

#define RUN_TEST(fn) do { \
    ++g_tests_total; \
    std::cout << COLOR_CYAN << "[EJECUTANDO] " << #fn << COLOR_RESET << " ... " << std::flush; \
    try { \
        fn(); \
        std::cout << COLOR_GREEN << "OK" << COLOR_RESET << "\n"; \
        ++g_tests_passed; \
    } catch (const std::exception& e) { \
        std::cout << COLOR_RED << "FALLO (Excepcion: " << e.what() << ")" << COLOR_RESET << "\n"; \
    } catch (...) { \
        std::cout << COLOR_RED << "FALLO (Excepcion desconocida)" << COLOR_RESET << "\n"; \
    } \
} while(0)

// Implementación ingenua O(N) para pruebas cruzadas (Differential Testing / Fuzzing)
class NaiveArray {
public:
    explicit NaiveArray(const std::vector<long long>& data) : arr_(data) {}

    void range_update(size_t l, size_t r, long long delta) {
        for (size_t i = l; i <= r; ++i) {
            arr_[i] += delta;
        }
    }

    SegmentTree::QueryResult range_query(size_t l, size_t r) const {
        long long sum = 0;
        long long min_val = std::numeric_limits<long long>::max();
        for (size_t i = l; i <= r; ++i) {
            sum += arr_[i];
            min_val = std::min(min_val, arr_[i]);
        }
        return {sum, min_val};
    }

private:
    std::vector<long long> arr_;
};

// =============================================================================
// CASOS DE PRUEBA
// =============================================================================

void test_single_element() {
    std::vector<long long> data = {42};
    SegmentTree st(data);

    assert(st.size() == 1);
    auto res = st.range_query(0, 0);
    assert(res.sum == 42);
    assert(res.min_val == 42);

    st.range_update(0, 0, 10);
    res = st.range_query(0, 0);
    assert(res.sum == 52);
    assert(res.min_val == 52);
}

void test_build_and_static_queries() {
    std::vector<long long> data = {5, 2, 9, -3, 7, 4, 1, 8};
    SegmentTree st(data);

    // Consulta total [0, 7]
    auto res = st.range_query(0, 7);
    assert(res.sum == 33);
    assert(res.min_val == -3);

    // Subrango [1, 4] -> {2, 9, -3, 7}
    res = st.range_query(1, 4);
    assert(res.sum == 15);
    assert(res.min_val == -3);

    // Subrango [4, 7] -> {7, 4, 1, 8}
    res = st.range_query(4, 7);
    assert(res.sum == 20);
    assert(res.min_val == 1);
}

void test_range_updates_accumulated() {
    std::vector<long long> data = {0, 0, 0, 0, 0};
    SegmentTree st(data);

    // Sumar 5 en [1, 3] -> {0, 5, 5, 5, 0}
    st.range_update(1, 3, 5);
    auto res = st.range_query(0, 4);
    assert(res.sum == 15);
    assert(res.min_val == 0);

    // Sumar 3 en [2, 4] -> {0, 5, 8, 8, 3}
    st.range_update(2, 4, 3);
    res = st.range_query(0, 4);
    assert(res.sum == 24);
    assert(res.min_val == 0);

    res = st.range_query(2, 3);
    assert(res.sum == 16);
    assert(res.min_val == 8);
}

void test_64bit_overflow_safety() {
    // Verificar que maneja enteros más grandes que 32 bits sin desbordamiento
    long long large = 2000000000LL; // 2 * 10^9
    std::vector<long long> data = {large, large, large};
    SegmentTree st(data);

    st.range_update(0, 2, large);
    auto res = st.range_query(0, 2);
    // Cada elemento ahora vale 4 * 10^9, la suma es 1.2 * 10^10 (supera int32)
    assert(res.sum == 12000000000LL);
    assert(res.min_val == 4000000000LL);
}

void test_out_of_bounds() {
    std::vector<long long> data = {1, 2, 3};
    SegmentTree st(data);

    bool caught = false;
    try {
        st.range_query(0, 5);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught && "Debe lanzar std::out_of_range para limites invalidos");
}

void test_differential_fuzzing_random() {
    // Fuzzing comparativo contra fuerza bruta con 10,000 operaciones mixtas
    constexpr size_t N = 200;
    constexpr size_t OPS = 10000;

    std::mt19937_64 rng(1337);
    std::uniform_int_distribution<long long> val_dist(-1000, 1000);
    std::uniform_int_distribution<size_t> idx_dist(0, N - 1);
    std::uniform_int_distribution<int> type_dist(0, 1);

    std::vector<long long> init_data(N);
    for (size_t i = 0; i < N; ++i) {
        init_data[i] = val_dist(rng);
    }

    SegmentTree st(init_data);
    NaiveArray naive(init_data);

    for (size_t op = 0; op < OPS; ++op) {
        size_t a = idx_dist(rng);
        size_t b = idx_dist(rng);
        size_t l = std::min(a, b);
        size_t r = std::max(a, b);

        if (type_dist(rng) == 0) {
            // Actualización
            long long delta = val_dist(rng);
            st.range_update(l, r, delta);
            naive.range_update(l, r, delta);
        } else {
            // Consulta
            auto st_res = st.range_query(l, r);
            auto naive_res = naive.range_query(l, r);
            assert(st_res.sum == naive_res.sum);
            assert(st_res.min_val == naive_res.min_val);
        }
    }
}

void test_stress_scale_performance() {
    // 200,000 operaciones en escala de competencia IOI/ICPC
    constexpr size_t N = 200000;
    constexpr size_t OPS = 200000;

    std::vector<long long> data(N, 1);
    auto t_start = std::chrono::high_resolution_clock::now();

    SegmentTree st(data);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<size_t> idx_dist(0, N - 1);
    std::uniform_int_distribution<long long> val_dist(1, 10);

    for (size_t i = 0; i < OPS; ++i) {
        size_t a = idx_dist(rng);
        size_t b = idx_dist(rng);
        size_t l = std::min(a, b);
        size_t r = std::max(a, b);

        if (i % 2 == 0) {
            st.range_update(l, r, val_dist(rng));
        } else {
            auto res = st.range_query(l, r);
            (void)res;
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

    std::cout << COLOR_YELLOW << "(Tiempo total para 2x10^5 ops: " << elapsed_ms << " ms) " << COLOR_RESET;
    // En optimizado (-O2) debe tardar típicamente menos de 500 ms
    assert(elapsed_ms < 2000.0 && "El árbol de segmentos está excediendo el límite razonable de tiempo");
}

// =============================================================================
// MAIN
// =============================================================================

int main() {
    std::cout << COLOR_YELLOW << "=== Batería de Pruebas: Segment Tree con Lazy Propagation ===" << COLOR_RESET << "\n\n";

    RUN_TEST(test_single_element);
    RUN_TEST(test_build_and_static_queries);
    RUN_TEST(test_range_updates_accumulated);
    RUN_TEST(test_64bit_overflow_safety);
    RUN_TEST(test_out_of_bounds);
    RUN_TEST(test_differential_fuzzing_random);
    RUN_TEST(test_stress_scale_performance);

    std::cout << "\n------------------------------------------------------------\n";
    if (g_tests_passed == g_tests_total) {
        std::cout << COLOR_GREEN << "RESUMEN: Todos los tests pasaron exitosamente ("
                  << g_tests_passed << "/" << g_tests_total << ")." << COLOR_RESET << "\n";
        return 0;
    } else {
        std::cout << COLOR_RED << "RESUMEN: Fallaron " << (g_tests_total - g_tests_passed)
                  << " de " << g_tests_total << " tests." << COLOR_RESET << "\n";
        return 1;
    }
}