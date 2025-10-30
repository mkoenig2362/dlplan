#include "../../../../include/dlplan/core/elements/frames/distance.h"

#include "../../../utils/collections.h"


namespace dlplan::core {
void DistanceFrame::compute_result(const FrameUnaryDenotation& frame_denot, FrameBinaryDenotation& result) const {
    int objects = frame_denot.get_num_objects();
    double distance = 0.0;
    for (int i = 0; i < objects; ++i) {
        for (int j = 0; j < objects; ++j) {
            if (j != i && !std::isnan(frame_denot.get_value(i)) && !std::isnan(frame_denot.get_value(j))) {
                result.insert(std::make_pair(i, j), std::abs(frame_denot.get_value(i) - frame_denot.get_value(j)));
            } 
        }
    }
}

FrameBinaryDenotation DistanceFrame::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    FrameBinaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_frame_unary->evaluate(state, caches),
        denotation);
    return denotation;
}

FrameBinaryDenotations DistanceFrame::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    FrameBinaryDenotations denotations;
    denotations.reserve(states.size());
    auto frame_unary_denotations = m_frame_unary->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        FrameBinaryDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*frame_unary_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

DistanceFrame::DistanceFrame(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const FrameUnary> frame_unary)
    : FrameBinary(index, vocabulary_info, frame_unary->is_static()), m_frame_unary(frame_unary) {
}

bool DistanceFrame::are_equal_impl(const FrameBinary& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const DistanceFrame&>(other);
        return m_is_static == other_derived.m_is_static
            && m_frame_unary == other_derived.m_frame_unary;
    }
    return false;
}

size_t DistanceFrame::hash_impl() const {
    return hash_combine(m_is_static, m_frame_unary);
}

FrameBinaryDenotation DistanceFrame::evaluate(const State& state) const {
    FrameBinaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_frame_unary->evaluate(state),
        denotation);
    return denotation;
}

int DistanceFrame::compute_complexity_impl() const {
    return m_frame_unary->compute_complexity() + 1;
}

void DistanceFrame::str_impl(std::stringstream& out) const {
    out << "f_distance" << "(";
    m_frame_unary->str(out);
    out << ")";
}

int DistanceFrame::compute_evaluate_time_score_impl() const {
    return m_frame_unary->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::DistanceFrame>>::operator()(
        const std::shared_ptr<const dlplan::core::DistanceFrame>& left_frame_binary,
        const std::shared_ptr<const dlplan::core::DistanceFrame>& right_frame_binary) const {
        return *left_frame_binary < *right_frame_binary;
    }

    std::size_t hash<dlplan::core::DistanceFrame>::operator()(const dlplan::core::DistanceFrame& frame_binary) const {
        return frame_binary.hash();
    }
}
