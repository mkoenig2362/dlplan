#include "primitive_binary.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void PrimitiveFrameBinary::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& function : factory.get_vocabulary_info()->get_functions()) {
        if (function.get_arity() == 2) {
            auto element = factory.make_primitive_frame_binary(function, 0, 1);
            auto denotations = element->evaluate(states, caches);
            if (states.size() == 0 || data.m_frame_binary_hash_table.insert(denotations).second) {
                std::get<5>(data.m_generated_features).push_back(element);
                data.m_frames_binary_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string PrimitiveFrameBinary::get_name() const {
    return "f_primitive_binary";
}

}
