#include "../../include/dlplan/core.h"

#include "../utils/logging.h"
#include "../../include/dlplan/utils/hash.h"

#include <sstream>
#include <limits>
#include <cmath>
#include <algorithm>


namespace dlplan::core {
// we assign index undefined since we do not care
FrameUnaryDenotation::FrameUnaryDenotation(int num_objects)
    : Base<FrameUnaryDenotation>(std::numeric_limits<int>::max()), m_num_objects(num_objects), 
            m_data(num_objects, std::numeric_limits<double>::quiet_NaN()) { }

FrameUnaryDenotation::FrameUnaryDenotation(const FrameUnaryDenotation& other) = default;

FrameUnaryDenotation& FrameUnaryDenotation::operator=(const FrameUnaryDenotation& other) = default;

FrameUnaryDenotation::FrameUnaryDenotation(FrameUnaryDenotation&& other) = default;

FrameUnaryDenotation& FrameUnaryDenotation::operator=(FrameUnaryDenotation&& other) = default;

FrameUnaryDenotation::~FrameUnaryDenotation() = default;

bool FrameUnaryDenotation::are_equal_impl(const FrameUnaryDenotation& other) const {
    if (m_data.size() != other.m_data.size()) return false;
    for (std::size_t k = 0; k < m_data.size(); ++k) {
        const double a = m_data[k];
        const double b = other.m_data[k];
        if (std::isnan(a) && std::isnan(b)) continue;
        if (a != b) return false;
    }
    return true;
}

void FrameUnaryDenotation::str_impl(std::stringstream& out) const {
    out << "FrameUnaryDenotation(num_objects=" << m_num_objects << ", values=[";
    bool first = true;
    for (ObjectIndex i = 0; i < m_num_objects; ++i) {
        double v = get_value({i});
        if (!std::isnan(v)) {
            if (!first) out << ", ";
            out << "(" << i << "):" << v;
            first = false;
        }
    }
    out << "])";
}

//is this fixed?????????
std::size_t FrameUnaryDenotation::hash_impl() const {
    std::size_t hash = 0;
    for (double value : m_data) {
        std::size_t part = std::isnan(value) 
            ? 0x9e3779b97f4a7c15ULL
            : std::hash<double>{}(value);
        hash ^= part + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}
/*
FrameUnaryDenotation& FrameUnaryDenotation::operator&=(const FrameUnaryDenotation& other) {
    m_data &= other.m_data;
    return *this;
}

FrameUnaryDenotation& FrameUnaryDenotation::operator|=(const FrameUnaryDenotation& other) {
    m_data |= other.m_data;
    return *this;
}

FrameUnaryDenotation& FrameUnaryDenotation::operator-=(const FrameUnaryDenotation& other) {
    m_data -= other.m_data;
    return *this;
}

FrameUnaryDenotation& FrameUnaryDenotation::operator~() {
    ~m_data;
    return *this;
}
*/
double FrameUnaryDenotation::get_value(ObjectIndex index) const {
    return m_data[index];
}

void FrameUnaryDenotation::set_value(ObjectIndex index, double value) {
    m_data[index] = value;
}

bool FrameUnaryDenotation::contains(ObjectIndex index) const {
    return !std::isnan(get_value(index));
}

void FrameUnaryDenotation::insert(ObjectIndex index, double value) {
    set_value(index, value);
}

void FrameUnaryDenotation::erase(ObjectIndex index) {
    set_value(index, std::numeric_limits<double>::quiet_NaN());
}

int FrameUnaryDenotation::size() const {
    return std::count_if(m_data.begin(), m_data.end(), [](double v) { return !std::isnan(v); });
}

bool FrameUnaryDenotation::empty() const {
    return std::none_of(m_data.begin(), m_data.end(), [](double v) { return !std::isnan(v); });
}

bool FrameUnaryDenotation::intersects(const FrameUnaryDenotation& other) const {
    for (std::size_t k = 0; k < m_data.size(); ++k)
        if (!std::isnan(m_data[k]) && !std::isnan(other.m_data[k]))
            return true;
    return false;
}

bool FrameUnaryDenotation::is_subset_of(const FrameUnaryDenotation& other) const {
    for (std::size_t k = 0; k < m_data.size(); ++k)
        if (!std::isnan(m_data[k]) && std::isnan(other.m_data[k]))
            return false;
    return true;
}

ObjectIndices FrameUnaryDenotation::to_vector() const {
    // In the case of bitset, the to_sorted_vector has best runtime complexity.
    //what about vector?
    return to_sorted_vector();
}

ObjectIndices FrameUnaryDenotation::to_sorted_vector() const {
    ObjectIndices result;
    result.reserve(size());
    for (ObjectIndex i = 0; i < m_num_objects; ++i)
        if (!std::isnan(m_data[i]))
            result.push_back(i);
    return result;
}

int FrameUnaryDenotation::get_num_objects() const {
    return m_num_objects;
}

}
