#include <vector>
#include <functional>

template <typename T>
class BloomFilter {
private:
    std::vector<bool> m_bits;
    std::size_t m_size;
    std::size_t m_numHashes;

    std::size_t computeHash(const T& item, std::size_t seed) const {
        std::hash<T> hasher;
        return hasher(item) ^ (seed * 0x9e3779b97f4a7c15ULL);
    }

public:
    BloomFilter(std::size_t size, std::size_t numHashes)
        : m_bits(size, false), m_size(size), m_numHashes(numHashes) {}

    void add(const T& item) {
        for (std::size_t i = 0; i < m_numHashes; ++i) {
            std::size_t idx = computeHash(item, i) % m_size;
            m_bits[idx] = true;
        }
    }

    bool mightContain(const T& item) const {
        for (std::size_t i = 0; i < m_numHashes; ++i) {
            std::size_t idx = computeHash(item, i) % m_size;
            if (!m_bits[idx]) return false;
        }
        return true;
    }
};
