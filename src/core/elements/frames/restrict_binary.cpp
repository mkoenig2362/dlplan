#include "../../../../include/dlplan/core/elements/frames/restrict_binary.h"

#include "../../../utils/collections.h"


namespace dlplan::core {
void RestrictFrameBinary::compute_result(const FrameBinaryDenotation& frame_denot, const RoleDenotation& role_denot, FrameBinaryDenotation& result) const {
    result = frame_denot;
    for(const auto& pair : frame_denot.to_vector()) {
        if(!role_denot.contains(pair)) {
            result.erase(pair);
        }
    }
}

FrameBinaryDenotation RestrictFrameBinary::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    FrameBinaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_frame_binary->evaluate(state, caches),
        *m_role->evaluate(state, caches),
        denotation);
    return denotation;
}

FrameBinaryDenotations RestrictFrameBinary::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    FrameBinaryDenotations denotations;
    denotations.reserve(states.size());
    auto frame_binary_denotations = m_frame_binary->evaluate(states, caches);
    auto role_denotations = m_role->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        FrameBinaryDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*frame_binary_denotations)[i],
            *(*role_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

RestrictFrameBinary::RestrictFrameBinary(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const FrameBinary> frame_binary, std::shared_ptr<const Role> role)
    : FrameBinary(index, vocabulary_info, frame_binary->is_static() && role->is_static()), m_frame_binary(frame_binary), m_role(role) {
}

bool RestrictFrameBinary::are_equal_impl(const FrameBinary& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const RestrictFrameBinary&>(other);
        return m_is_static == other_derived.m_is_static
            && m_frame_binary == other_derived.m_frame_binary
            && m_role == other_derived.m_role;
    }
    return false;
}

size_t RestrictFrameBinary::hash_impl() const {
    return hash_combine(m_is_static, m_frame_binary, m_role);
}

FrameBinaryDenotation RestrictFrameBinary::evaluate(const State& state) const {
    FrameBinaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_frame_binary->evaluate(state),
        m_role->evaluate(state),
        denotation);
    return denotation;
}

int RestrictFrameBinary::compute_complexity_impl() const {
    return m_frame_binary->compute_complexity() + m_role->compute_complexity() + 1;
}

void RestrictFrameBinary::str_impl(std::stringstream& out) const {
    out << "f_restrict_binary" << "(";
    m_frame_binary->str(out);
    out << ",";
    m_role->str(out);
    out << ")";
}

int RestrictFrameBinary::compute_evaluate_time_score_impl() const {
    return m_frame_binary->compute_evaluate_time_score() + m_role->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::RestrictFrameBinary>>::operator()(
        const std::shared_ptr<const dlplan::core::RestrictFrameBinary>& left_frame_binary,
        const std::shared_ptr<const dlplan::core::RestrictFrameBinary>& right_frame_binary) const {
        return *left_frame_binary < *right_frame_binary;
    }

    std::size_t hash<dlplan::core::RestrictFrameBinary>::operator()(const dlplan::core::RestrictFrameBinary& frame_binary) const {
        return frame_binary.hash();
    }
}
