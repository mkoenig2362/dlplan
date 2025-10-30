#include "../../include/dlplan/core.h"

#include "../utils/collections.h"
#include "../utils/logging.h"
#include "../../include/dlplan/utils/hash.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <vector>

namespace dlplan::core {

static AtomIndices sort_atom_idxs(AtomIndices&& atom_idxs) {
    std::sort(atom_idxs.begin(), atom_idxs.end());
    return atom_idxs;
}

template <class IdxVec, class ValVec>
void sort_atoms(IdxVec& idx, ValVec& val)
{
    using Pair = std::pair<typename IdxVec::value_type, typename ValVec::value_type>;
    std::vector<Pair> tmp;
    tmp.reserve(idx.size());
    for (std::size_t i = 0; i < idx.size(); ++i)
        tmp.emplace_back(idx[i], val[i]);
    std::sort(tmp.begin(), tmp.end(), [](const Pair& a, const Pair& b){ return a.first < b.first; });
    for (std::size_t i = 0; i < tmp.size(); ++i) {
        idx[i] = tmp[i].first;
        val[i] = tmp[i].second;
    }
}


State::State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, const std::vector<Atom>& atoms)
    : Base<State>(index), m_instance_info(instance_info), m_atom_values(m_atom_indices.size(), 1) {
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const auto& atom){ return !atom.is_static(); })) {
        throw std::runtime_error("State::State - static atom is not allowed in State.");
    }
    m_atom_indices.reserve(atoms.size());
    for (const auto& atom : atoms) {
        int atom_idx = atom.get_index();
        m_atom_indices.push_back(atom_idx);
    }
    if (!std::is_sorted(m_atom_indices.begin(), m_atom_indices.end())) {
        std::sort(m_atom_indices.begin(), m_atom_indices.end());
    }
}

State::State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, const AtomIndices& atom_indices)
    : Base<State>(index), m_instance_info(instance_info), m_atom_indices(std::is_sorted(atom_indices.begin(), atom_indices.end()) ? atom_indices : sort_atom_idxs(AtomIndices(atom_indices))), m_atom_values(m_atom_indices.size(), 1) {
    const auto& atoms = instance_info->get_atoms();
    if (!std::all_of(m_atom_indices.begin(), m_atom_indices.end(), [&](int atom_idx){ return utils::in_bounds(atom_idx, atoms); })) {
        throw std::runtime_error("State::State - atom index out of range.");
    }
}

State::State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, AtomIndices&& atom_indices)
    : Base<State>(index), m_instance_info(instance_info), m_atom_indices(std::is_sorted(atom_indices.begin(), atom_indices.end()) ? atom_indices : sort_atom_idxs(std::move(atom_indices))), m_atom_values(m_atom_indices.size(), 1) {
    const auto& atoms = instance_info->get_atoms();
    if (!std::all_of(m_atom_indices.begin(), m_atom_indices.end(), [&](int atom_idx){ return utils::in_bounds(atom_idx, atoms); })) {
        throw std::runtime_error("State::State - atom index out of range.");
    }
}


State::State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, const std::vector<Atom>& atoms, const AtomValues& atom_values)
    : Base<State>(index), m_instance_info(instance_info), m_atom_values(atom_values) {
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const auto& atom){ return !atom.is_static(); })) {
        throw std::runtime_error("State::State - static atom is not allowed in State.");
    }
    m_atom_indices.reserve(atoms.size());
    for (const auto& atom : atoms) {
        int atom_idx = atom.get_index();
        m_atom_indices.push_back(atom_idx);
    }
    if (!std::is_sorted(m_atom_indices.begin(), m_atom_indices.end())) {
        sort_atoms(m_atom_indices, m_atom_values);
    }  
}

State::State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, const AtomIndices& atom_indices, const AtomValues& atom_values)
    : Base<State>(index), m_instance_info(instance_info), m_atom_indices(atom_indices), m_atom_values(atom_values) {
    if (!std::is_sorted(m_atom_indices.begin(), m_atom_indices.end()))
        sort_atoms(m_atom_indices, m_atom_values);
    const auto& atoms = m_instance_info->get_atoms();
    if (!std::all_of(m_atom_indices.begin(), m_atom_indices.end(),
                     [&](int i){ return utils::in_bounds(i, atoms); }))
        throw std::runtime_error("State::State - atom index out of range.");
}


State::State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, AtomIndices&& atom_indices, AtomValues&& atom_values)
    : Base<State>(index), m_instance_info(instance_info), m_atom_indices(std::move(atom_indices)), m_atom_values(std::move(atom_values)) {
     if (!std::is_sorted(m_atom_indices.begin(), m_atom_indices.end()))
        sort_atoms(m_atom_indices, m_atom_values);
    const auto& atoms = m_instance_info->get_atoms();
    if (!std::all_of(m_atom_indices.begin(), m_atom_indices.end(),
                     [&](int i){ return utils::in_bounds(i, atoms); }))
        throw std::runtime_error("State::State - atom index out of range.");
}

State::State(const State&) = default;

State& State::operator=(const State&) = default;

State::State(State&& other) = default;

State& State::operator=(State&& other) = default;

State::~State() = default;

bool State::are_equal_impl(const State& other) const {
    return (get_atom_indices() == other.get_atom_indices()) && (get_instance_info() == other.get_instance_info());
}

void State::str_impl(std::stringstream& out) const {
    out << "(instance index=" << get_instance_info()->get_index()
           << ", state index=" << get_index()
           << ", atoms={";
    const auto& atoms = get_instance_info()->get_atoms();
    for (int atom_idx : m_atom_indices) {
        assert(dlplan::utils::in_bounds(atom_idx, atoms));
        const auto& atom = atoms[atom_idx];
        out << atom.get_name();
        if (atom_idx != m_atom_indices.back()) {
            out << ", ";
        }
    }
    out << "})";
}

size_t State::hash_impl() const {
    assert(std::is_sorted(m_atom_indices.begin(), m_atom_indices.end()));
    return hash_combine(hash_vector(m_atom_indices), m_instance_info);
}

std::shared_ptr<InstanceInfo> State::get_instance_info() const {
    return m_instance_info;
}

const AtomIndices& State::get_atom_indices() const {
    return m_atom_indices;
}

const AtomValues& State::get_atom_values() const {
    return m_atom_values;
}

double State::value_of(int atom_id) const {
    auto it = std::lower_bound(m_atom_indices.begin(), m_atom_indices.end(), atom_id);
    if (it == m_atom_indices.end() || *it != atom_id) throw std::out_of_range("State::value_of – atom id not found");
    std::size_t pos = std::distance(m_atom_indices.begin(), it);
    return m_atom_values[pos];
}

}
