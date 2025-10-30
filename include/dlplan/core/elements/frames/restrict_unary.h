#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_RESTRICT_UNARY_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_RESTRICT_UNARY_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>


using namespace std::string_literals;


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class RestrictFrameUnary : public FrameUnary {
private:
    const std::shared_ptr<const FrameUnary> m_frame_unary;
    const std::shared_ptr<const Concept> m_concept;

    void compute_result(const FrameUnaryDenotation& frame_denot, const ConceptDenotation& concept_denot, FrameUnaryDenotation& result) const;

    FrameUnaryDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    FrameUnaryDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    RestrictFrameUnary(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const FrameUnary> frame_unary, std::shared_ptr<const Concept> concept_);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const FrameUnary& other) const override;

    size_t hash_impl() const override;

    FrameUnaryDenotation evaluate(const State& state) const override;

    int compute_complexity_impl() const override;

    void str_impl(std::stringstream& out) const override;

    int compute_evaluate_time_score_impl() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::RestrictFrameUnary>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::RestrictFrameUnary>& left_frame_unary,
            const std::shared_ptr<const dlplan::core::RestrictFrameUnary>& right_frame_unary) const;
    };

    template<>
    struct hash<dlplan::core::RestrictFrameUnary>
    {
        std::size_t operator()(const dlplan::core::RestrictFrameUnary& frame_unary) const;
    };
}

#endif
