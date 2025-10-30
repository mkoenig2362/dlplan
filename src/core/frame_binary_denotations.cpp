#include "../../include/dlplan/core.h"

#include "../utils/logging.h"
#include "../../include/dlplan/utils/hash.h"

#include <sstream>
#include <limits>
#include <cmath>
#include <algorithm>


namespace dlplan::core {
// we assign index undefined since we do not care
FrameBinaryDenotation::FrameBinaryDenotation(int num_objects)
    : Base<FrameBinaryDenotation>(std::numeric_limits<int>::max()), m_num_objects(num_objects), 
            m_data((num_objects * num_objects), std::numeric_limits<double>::quiet_NaN()) { }

FrameBinaryDenotation::FrameBinaryDenotation(const FrameBinaryDenotation& other) = default;

FrameBinaryDenotation& FrameBinaryDenotation::operator=(const FrameBinaryDenotation& other) = default;

FrameBinaryDenotation::FrameBinaryDenotation(FrameBinaryDenotation&& other) = default;

FrameBinaryDenotation& FrameBinaryDenotation::operator=(FrameBinaryDenotation&& other) = default;

FrameBinaryDenotation::~FrameBinaryDenotation() = default;

bool FrameBinaryDenotation::are_equal_impl(const FrameBinaryDenotation& other) const {
    if (m_data.size() != other.m_data.size()) return false;
    for (std::size_t k = 0; k < m_data.size(); ++k) {
        const double a = m_data[k];
        const double b = other.m_data[k];
        if (std::isnan(a) && std::isnan(b)) continue;
        if (a != b) return false;
    }
    return true;
}

void FrameBinaryDenotation::str_impl(std::stringstream& out) const {
    out << "FrameBinaryDenotation(num_objects=" << m_num_objects << ", values=[";
    bool first = true;
    for (ObjectIndex i = 0; i < m_num_objects; ++i)
        for (ObjectIndex j = 0; j < m_num_objects; ++j) {
            double v = get_value({i,j});
            if (!std::isnan(v)) {
                if (!first) out << ", ";
                out << "(" << i << "," << j << "):" << v;
                first = false;
            }
        }
    out << "])";
}

//is this fixed?????????
std::size_t FrameBinaryDenotation::hash_impl() const {
    std::size_t hash = 0;
    for (double value : m_data) {
        std::size_t part = std::isnan(value) ? 0x9e3779b97f4a7c15ULL : std::hash<double>{}(value);
        hash ^= part + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}
/*
FrameBinaryDenotation& FrameBinaryDenotation::operator&=(const FrameBinaryDenotation& other) {
    m_data &= other.m_data;
    return *this;
}

FrameBinaryDenotation& FrameBinaryDenotation::operator|=(const FrameBinaryDenotation& other) {
    m_data |= other.m_data;
    return *this;
}

FrameBinaryDenotation& FrameBinaryDenotation::operator-=(const FrameBinaryDenotation& other) {
    m_data -= other.m_data;
    return *this;
}

FrameBinaryDenotation& FrameBinaryDenotation::operator~() {
    ~m_data;
    return *this;
}
*/
double FrameBinaryDenotation::get_value(const PairOfObjectIndices& indices) const {
    return m_data[indices.first * m_num_objects + indices.second];
}

void FrameBinaryDenotation::set_value(const PairOfObjectIndices& indices, double value) {
    m_data[indices.first * m_num_objects + indices.second] = value;
}

bool FrameBinaryDenotation::contains(const PairOfObjectIndices& indices) const {
    return !std::isnan(get_value(indices));
}

void FrameBinaryDenotation::insert(const PairOfObjectIndices& indices, double value) {
    set_value(indices, value);
}

int FrameBinaryDenotation::size() const {
    return std::count_if(m_data.begin(), m_data.end(), [](double v) { return !std::isnan(v); });
}

bool FrameBinaryDenotation::empty() const {
    return std::none_of(m_data.begin(), m_data.end(), [](double v) { return !std::isnan(v); });
}

bool FrameBinaryDenotation::intersects(const FrameBinaryDenotation& other) const {
    for (std::size_t k = 0; k < m_data.size(); ++k)
        if (!std::isnan(m_data[k]) && !std::isnan(other.m_data[k]))
            return true;
    return false;
}

bool FrameBinaryDenotation::is_subset_of(const FrameBinaryDenotation& other) const {
    for (std::size_t k = 0; k < m_data.size(); ++k)
        if (!std::isnan(m_data[k]) && std::isnan(other.m_data[k]))
            return false;
    return true;
}

PairsOfObjectIndices FrameBinaryDenotation::to_vector() const {
    // In the case of bitset, the to_sorted_vector has best runtime complexity.
    return to_sorted_vector();
}

PairsOfObjectIndices FrameBinaryDenotation::to_sorted_vector() const {
    PairsOfObjectIndices res;
    res.reserve(m_data.size());
    for (ObjectIndex i = 0; i < m_num_objects; ++i)
        for (ObjectIndex j = 0; j < m_num_objects; ++j)
            if (!std::isnan(get_value({i,j})))
                res.emplace_back(i,j);
    return res; 
}

int FrameBinaryDenotation::get_num_objects() const {
    return m_num_objects;
}

}
