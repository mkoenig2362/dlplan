#include "../../../../include/dlplan/core/elements/frames/restrict_unary.h"

#include "../../../utils/collections.h"


namespace dlplan::core {
void RestrictFrameUnary::compute_result(const FrameUnaryDenotation& frame_denot, const ConceptDenotation& concept_denot, FrameUnaryDenotation& result) const {
    result = frame_denot;
    for(const auto& object : frame_denot.to_vector()) {
        if(!concept_denot.contains(object)) {
            result.erase(object);
        }
    }
}

FrameUnaryDenotation RestrictFrameUnary::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    FrameUnaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_frame_unary->evaluate(state, caches),
        *m_concept->evaluate(state, caches),
        denotation);
    return denotation;
}

FrameUnaryDenotations RestrictFrameUnary::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    FrameUnaryDenotations denotations;
    denotations.reserve(states.size());
    auto frame_unary_denotations = m_frame_unary->evaluate(states, caches);
    auto concept_denotations = m_concept->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        FrameUnaryDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*frame_unary_denotations)[i],
            *(*concept_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

RestrictFrameUnary::RestrictFrameUnary(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const FrameUnary> frame_unary, std::shared_ptr<const Concept> concept_)
    : FrameUnary(index, vocabulary_info, frame_unary->is_static() && concept_->is_static()), m_frame_unary(frame_unary), m_concept(concept_) {
}

bool RestrictFrameUnary::are_equal_impl(const FrameUnary& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const RestrictFrameUnary&>(other);
        return m_is_static == other_derived.m_is_static
            && m_frame_unary == other_derived.m_frame_unary
            && m_concept == other_derived.m_concept;
    }
    return false;
}

size_t RestrictFrameUnary::hash_impl() const {
    return hash_combine(m_is_static, m_frame_unary, m_concept);
}

FrameUnaryDenotation RestrictFrameUnary::evaluate(const State& state) const {
    FrameUnaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_frame_unary->evaluate(state),
        m_concept->evaluate(state),
        denotation);
    return denotation;
}

int RestrictFrameUnary::compute_complexity_impl() const {
    return m_frame_unary->compute_complexity() + m_concept->compute_complexity() + 1;
}

void RestrictFrameUnary::str_impl(std::stringstream& out) const {
    out << "f_restrict_unary" << "(";
    m_frame_unary->str(out);
    out << ",";
    m_concept->str(out);
    out << ")";
}

int RestrictFrameUnary::compute_evaluate_time_score_impl() const {
    return m_frame_unary->compute_evaluate_time_score() + m_concept->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::RestrictFrameUnary>>::operator()(
        const std::shared_ptr<const dlplan::core::RestrictFrameUnary>& left_frame_unary,
        const std::shared_ptr<const dlplan::core::RestrictFrameUnary>& right_frame_unary) const {
        return *left_frame_unary < *right_frame_unary;
    }

    std::size_t hash<dlplan::core::RestrictFrameUnary>::operator()(const dlplan::core::RestrictFrameUnary& frame_unary) const {
        return frame_unary.hash();
    }
}
