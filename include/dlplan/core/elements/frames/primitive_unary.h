#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_PRIMITIVE_UNARY_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_PRIMITIVE_UNARY_H_

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
class PrimitiveFrameUnary : public FrameUnary {
private:
    const Function m_function;
    const int m_pos;

    void compute_result(const State& state, FrameUnaryDenotation& result) const;

    FrameUnaryDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    FrameUnaryDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    PrimitiveFrameUnary(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Function& function, int pos);

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
    struct less<std::shared_ptr<const dlplan::core::PrimitiveFrameUnary>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::PrimitiveFrameUnary>& left_frame_unary,
            const std::shared_ptr<const dlplan::core::PrimitiveFrameUnary>& right_frame_unary) const;
    };

    template<>
    struct hash<dlplan::core::PrimitiveFrameUnary>
    {
        std::size_t operator()(const dlplan::core::PrimitiveFrameUnary& frame_unary) const;
    };
}

#endif
