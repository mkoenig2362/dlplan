#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_PRIMITIVE_BINARY_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_PRIMITIVE_BINARY_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class PrimitiveFrameBinary : public FrameBinary {
private:
    const Function m_function;
    const int m_pos_1;
    const int m_pos_2;

    void compute_result(const State& state, FrameBinaryDenotation& result) const;

    FrameBinaryDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    FrameBinaryDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    PrimitiveFrameBinary(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Function& function, int pos_1, int pos_2);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const FrameBinary& other) const override;
    size_t hash_impl() const override;

    FrameBinaryDenotation evaluate(const State& state) const override;

    int compute_complexity_impl() const override;

    void str_impl(std::stringstream& out) const override;

    int compute_evaluate_time_score_impl() const override;

    const Function& get_function() const;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::PrimitiveFrameBinary>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::PrimitiveFrameBinary>& left_frame_binary,
            const std::shared_ptr<const dlplan::core::PrimitiveFrameBinary>& right_frame_binary) const;
    };

    template<>
    struct hash<dlplan::core::PrimitiveFrameBinary>
    {
        std::size_t operator()(const dlplan::core::PrimitiveFrameBinary& frame_binary) const;
    };
}

#endif
