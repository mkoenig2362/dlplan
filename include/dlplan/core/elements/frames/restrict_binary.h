#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_RESTRICT_BINARY_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_FRAMES_RESTRICT_BINARY_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class RestrictFrameBinary : public FrameBinary {
private:
    const std::shared_ptr<const FrameBinary> m_frame_binary;
    const std::shared_ptr<const Role> m_role;

    void compute_result(const FrameBinaryDenotation& frame_denot, const RoleDenotation& role_denot, FrameBinaryDenotation& result) const;

    FrameBinaryDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    FrameBinaryDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    RestrictFrameBinary(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const FrameBinary> frame_binary, std::shared_ptr<const Role> role);

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
    struct less<std::shared_ptr<const dlplan::core::RestrictFrameBinary>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::RestrictFrameBinary>& left_frame_binary,
            const std::shared_ptr<const dlplan::core::RestrictFrameBinary>& right_frame_binary) const;
    };

    template<>
    struct hash<dlplan::core::RestrictFrameBinary>
    {
        std::size_t operator()(const dlplan::core::RestrictFrameBinary& frame_binary) const;
    };
}

#endif
