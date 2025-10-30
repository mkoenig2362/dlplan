#include "../../../../include/dlplan/core/elements/frames/primitive_binary.h"

#include "../../../utils/collections.h"


using namespace std::string_literals;


namespace dlplan::core {
void PrimitiveFrameBinary::compute_result(const State& state, FrameBinaryDenotation& result) const {
    const auto& instance_info = *state.get_instance_info();
    const auto& atoms = instance_info.get_atoms();
    for (int atom_idx : state.get_atom_indices()) {
        const auto& atom = atoms[atom_idx];
        if (atom.is_function() && atom.get_symbol_index() == m_function.get_index()) {
            assert(dlplan::utils::in_bounds(m_pos_1, atom.get_object_indices()));
            assert(dlplan::utils::in_bounds(m_pos_2, atom.get_object_indices()));
            result.insert(std::make_pair(atom.get_object_indices()[m_pos_1], atom.get_object_indices()[m_pos_2]), state.value_of(atom_idx));
        }
    }
    for (const auto& atom : state.get_instance_info()->get_static_atoms()) {
        if (atom.is_function() && atom.get_symbol_index() == m_function.get_index()) {
            assert(dlplan::utils::in_bounds(m_pos_1, atom.get_object_indices()));
            assert(dlplan::utils::in_bounds(m_pos_2, atom.get_object_indices()));
            result.insert(std::make_pair(atom.get_object_indices()[m_pos_1], atom.get_object_indices()[m_pos_2]), 0.0);
        }
    }
}

FrameBinaryDenotation PrimitiveFrameBinary::evaluate_impl(const State& state, DenotationsCaches&) const {
    FrameBinaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        state,
        denotation);
    return denotation;
}

FrameBinaryDenotations PrimitiveFrameBinary::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    FrameBinaryDenotations denotations;
    denotations.reserve(states.size());
    for (size_t i = 0; i < states.size(); ++i) {
        FrameBinaryDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            states[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}


PrimitiveFrameBinary::PrimitiveFrameBinary(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Function& function, int pos_1, int pos_2)
: FrameBinary(index, vocabulary_info, function.is_static()), m_function(function), m_pos_1(pos_1), m_pos_2(pos_2) {
    if (m_pos_1 >= m_function.get_arity() || m_pos_2 >= m_function.get_arity()) {
        throw std::runtime_error("PrimitiveFrameBinary::evaluate_impl - object index does not match function arity ("s + std::to_string(m_pos_1) + " or " + std::to_string(m_pos_2)  + " > " + std::to_string(m_function.get_arity()) + ").");
    }
}

bool PrimitiveFrameBinary::are_equal_impl(const FrameBinary& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const PrimitiveFrameBinary&>(other);
        return m_is_static == other_derived.m_is_static
            && m_function == other_derived.m_function
            && m_pos_1 == other_derived.m_pos_1
            && m_pos_2 == other_derived.m_pos_2;
    }
    return false;
}

size_t PrimitiveFrameBinary::hash_impl() const {
    return hash_combine(m_is_static, m_function, m_pos_1, m_pos_2);
}

FrameBinaryDenotation PrimitiveFrameBinary::evaluate(const State& state) const {
    FrameBinaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(state, denotation);
    return denotation;
}

int PrimitiveFrameBinary::compute_complexity_impl() const {
    return 1;
}

void PrimitiveFrameBinary::str_impl(std::stringstream& out) const {
    out << "f_primitive_binary" << "(" << m_function.get_name() << "," << std::to_string(m_pos_1) << "," << std::to_string(m_pos_2) << ")";
}

int PrimitiveFrameBinary::compute_evaluate_time_score_impl() const {
    return SCORE_LINEAR;
}

const Function& PrimitiveFrameBinary::get_function() const {
    return m_function;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::PrimitiveFrameBinary>>::operator()(
        const std::shared_ptr<const dlplan::core::PrimitiveFrameBinary>& left_frame_binary,
        const std::shared_ptr<const dlplan::core::PrimitiveFrameBinary>& right_frame_binary) const {
        return *left_frame_binary < *right_frame_binary;
    }


    std::size_t hash<dlplan::core::PrimitiveFrameBinary>::operator()(const dlplan::core::PrimitiveFrameBinary& frame_binary) const {
        return frame_binary.hash();
    }
}

