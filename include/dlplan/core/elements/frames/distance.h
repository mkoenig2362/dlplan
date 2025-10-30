#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_DISTANCE_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_DISTANCE_H_

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
class DistanceFrame : public FrameBinary {
private:
    const std::shared_ptr<const FrameUnary> m_frame_unary;

    void compute_result(const FrameUnaryDenotation& frame_denot, FrameBinaryDenotation& result) const;

    FrameBinaryDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    FrameBinaryDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    DistanceFrame(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const FrameUnary> frame_unary);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const FrameBinary& other) const override;

    size_t hash_impl() const override;

    FrameBinaryDenotation evaluate(const State& state) const override;

    int compute_complexity_impl() const override;

    void str_impl(std::stringstream& out) const override;

    int compute_evaluate_time_score_impl() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::DistanceFrame>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::DistanceFrame>& left_frame_binary,
            const std::shared_ptr<const dlplan::core::DistanceFrame>& right_frame_binary) const;
    };

    template<>
    struct hash<dlplan::core::DistanceFrame>
    {
        std::size_t operator()(const dlplan::core::DistanceFrame& frame_binary) const;
    };
}

#endif
