#include "../../../../include/dlplan/core/elements/frames/distance.h"

#include "../../../utils/collections.h"


namespace dlplan::core {
void DistanceFrame::compute_result(const FrameUnaryDenotation& frame_denot_1, const FrameUnaryDenotation& frame_denot_2, FrameBinaryDenotation& result) const {
    int objects_1 = frame_denot_1.get_num_objects();
    int objects_2 = frame_denot_2.get_num_objects();
    double distance = 0.0;
    for (int i = 0; i < objects_1; ++i) {
        for (int j = 0; j < objects_2; ++j) {
            if (j != i && !std::isnan(frame_denot_1.get_value(i)) && !std::isnan(frame_denot_2.get_value(j))) {
                result.insert(std::make_pair(i, j), std::abs(frame_denot_1.get_value(i) - frame_denot_2.get_value(j)));
            } 
        }
    }
}

FrameBinaryDenotation DistanceFrame::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    FrameBinaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_frame_unary_1->evaluate(state, caches),
        *m_frame_unary_2->evaluate(state, caches),
        denotation);
    return denotation;
}

FrameBinaryDenotations DistanceFrame::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    FrameBinaryDenotations denotations;
    denotations.reserve(states.size());
    auto frame_unary_1_denotations = m_frame_unary_1->evaluate(states, caches);
    auto frame_unary_2_denotations = m_frame_unary_2->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        FrameBinaryDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*frame_unary_1_denotations)[i],
            *(*frame_unary_2_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

DistanceFrame::DistanceFrame(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const FrameUnary> frame_unary_1, std::shared_ptr<const FrameUnary> frame_unary_2)
    : FrameBinary(index, vocabulary_info, frame_unary_1->is_static() || frame_unary_2->is_static()), m_frame_unary_1(frame_unary_1), m_frame_unary_2(frame_unary_2) {
}

bool DistanceFrame::are_equal_impl(const FrameBinary& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const DistanceFrame&>(other);
        return m_is_static == other_derived.m_is_static
            && m_frame_unary_1 == other_derived.m_frame_unary_1
            && m_frame_unary_2 == other_derived.m_frame_unary_2;
    }
    return false;
}

size_t DistanceFrame::hash_impl() const {
    return hash_combine(m_is_static, m_frame_unary_1, m_frame_unary_2);
}

FrameBinaryDenotation DistanceFrame::evaluate(const State& state) const {
    FrameBinaryDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_frame_unary_1->evaluate(state),
        m_frame_unary_2->evaluate(state),
        denotation);
    return denotation;
}

int DistanceFrame::compute_complexity_impl() const {
    return m_frame_unary_1->compute_complexity() + m_frame_unary_2->compute_complexity() + 1;
}

void DistanceFrame::str_impl(std::stringstream& out) const {
    out << "f_distance" << "(";
    m_frame_unary_1->str(out);
    out << ",";
    m_frame_unary_2->str(out);
    out << ")";
}

int DistanceFrame::compute_evaluate_time_score_impl() const {
    return m_frame_unary_1->compute_evaluate_time_score() + m_frame_unary_2->compute_evaluate_time_score() + SCORE_QUADRATIC;
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
