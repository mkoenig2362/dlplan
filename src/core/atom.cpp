#include "../../include/dlplan/core.h"

#include "../utils/collections.h"
#include "../utils/logging.h"

#include <sstream>
#include <cassert>

using namespace std::string_literals;


namespace dlplan::core {

Atom::Atom(
    AtomIndex index,
    const std::string& name,
    SymbolKind    kind,   //Predicate or Function?
    SymbolIndex   symbol_index, //replaces PredicateIndex
    const ObjectIndices &m_object_indices,
    bool is_static)
    : Base<Atom>(index), m_name(name), m_kind(kind),
      m_symbol_index(symbol_index), m_object_indices(m_object_indices), m_is_static(is_static) {
}

Atom::Atom(const Atom& other) = default;

Atom& Atom::operator=(const Atom& other) = default;

Atom::Atom(Atom&& other) = default;

Atom& Atom::operator=(Atom&& other) = default;

Atom::~Atom() = default;

bool Atom::are_equal_impl(const Atom& other) const {
    // remove index comparison when we use factory.
    return (get_name() == other.get_name()) && (get_index() == other.get_index()) && (is_static() == other.is_static());
}

void Atom::str_impl(std::stringstream& out) const {
    out << "Atom("
       << "index=" << m_index << ", "
       << "name=" << m_name << ", "
       << "kind=" << m_kind << ", "
       << "symbol_index=" << m_symbol_index << ", "
       << "object_indices=" << m_object_indices << ", "
       << "is_static=" << m_is_static
       << ")";
}

size_t Atom::hash_impl() const {
    return hash_combine(
        m_name,
        hash_vector(m_object_indices),
        m_is_static);
}

const std::string& Atom::get_name() const {
    return m_name;
}

PredicateIndex Atom::get_predicate_index() const {
    //std::cout << "deprecated";
    return m_symbol_index;
} //deprecated

SymbolIndex Atom::get_symbol_index() const {
    return m_symbol_index;
}

const ObjectIndices& Atom::get_object_indices() const {
    return m_object_indices;
}

bool Atom::is_static() const {
    return m_is_static;
}

bool Atom::is_function() const {
    return m_kind == SymbolKind::Function;
}

bool Atom::is_predicate() const {
    return m_kind == SymbolKind::Predicate;
}
}
