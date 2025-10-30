#include "../../../../include/dlplan/core/elements/frames/primitive_unary.h"

#include "../../../utils/collections.h"


namespace dlplan::core {
void PrimitiveFrameUnary::compute_result(const State& state, FrameUnaryDenotation& result) const {
    const auto& instance_info = *state.get_instance_info();
    const auto& atoms = instance_info.get_atoms();
    for (int atom_idx : state.get_atom_indices()) {
        const auto& atom = atoms[atom_idx];
        if (atom.is_function() && atom.get_symbol_index() == m_function.get_index()) {
            assert(dlplan::utils::in_bounds(m_pos, atom.get_object_indices()));
            result.insert(atom.get_object_indices()[m_pos], state.value_of(atom_idx));
        }
    }
    for (const auto &atom : state.get_instance_info()->get_static_atoms()) {
        if (atom.is_function() && atom.get_symbol_index() == m_function.get_index()) {
            assert(dlplan::utils::in_bounds(m_pos, atom.get_object_indices()));
            result.insert(atom.get_object_indices()[m_pos], 0.0);
        }
    }
}

FrameUnaryDenotation PrimitiveFrameUnary::evaluate_impl(const State& state, DenotationsCaches&) const {
    FrameUnaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        state,
        denotation);
    return denotation;
}

FrameUnaryDenotations PrimitiveFrameUnary::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    FrameUnaryDenotations denotations;
    denotations.reserve(states.size());
    for (size_t i = 0; i < states.size(); ++i) {
        FrameUnaryDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            states[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

PrimitiveFrameUnary::PrimitiveFrameUnary(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Function& function, int pos)
    : FrameUnary(index, vocabulary_info, function.is_static()), m_function(function), m_pos(pos) {
    if (m_pos >= m_function.get_arity()) {
        throw std::runtime_error("PrimitiveFrameUnary::PrimitiveFrameUnary - object index does not match function arity ("s + std::to_string(m_pos) + " > " + std::to_string(m_function.get_arity()) + ").");
    }
}

bool PrimitiveFrameUnary::are_equal_impl(const FrameUnary& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const PrimitiveFrameUnary&>(other);
        return m_is_static == other_derived.m_is_static
            && m_function == other_derived.m_function
            && m_pos == other_derived.m_pos;
    }
    return false;
}

size_t PrimitiveFrameUnary::hash_impl() const {
    return hash_combine(m_is_static, m_function, m_pos);
}

FrameUnaryDenotation PrimitiveFrameUnary::evaluate(const State& state) const {
    FrameUnaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(state, denotation);
    return denotation;
}

int PrimitiveFrameUnary::compute_complexity_impl() const {
    return 1;
}

void PrimitiveFrameUnary::str_impl(std::stringstream& out) const {
    out << "f_primitive_unary" << "(" << m_function.get_name() << "," << std::to_string(m_pos) << ")";
}

int PrimitiveFrameUnary::compute_evaluate_time_score_impl() const {
    return SCORE_LINEAR;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::PrimitiveFrameUnary>>::operator()(
        const std::shared_ptr<const dlplan::core::PrimitiveFrameUnary>& left_frame_unary,
        const std::shared_ptr<const dlplan::core::PrimitiveFrameUnary>& right_frame_unary) const {
        return *left_frame_unary < *right_frame_unary;
    }

    std::size_t hash<dlplan::core::PrimitiveFrameUnary>::operator()(const dlplan::core::PrimitiveFrameUnary& frame_unary) const {
        return frame_unary.hash();
    }
}
