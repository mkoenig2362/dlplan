#include "maximum.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void MaximumNumerical::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& frame_unary : data.m_frames_unary_by_iteration[target_complexity-1]) {
        auto element = factory.make_maximum_numerical(frame_unary);
        auto denotations = element->evaluate(states, caches);
        if (data.m_numerical_hash_table.insert(denotations).second) {
            std::get<1>(data.m_generated_features).push_back(element);
            data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
    for (const auto& frame_binary : data.m_frames_binary_by_iteration[target_complexity-1]) {
        auto element = factory.make_maximum_numerical(frame_binary);
        auto denotations = element->evaluate(states, caches);
        if (data.m_numerical_hash_table.insert(denotations).second) {
            std::get<1>(data.m_generated_features).push_back(element);
            data.m_numericals_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string MaximumNumerical::get_name() const {
    return "n_maximum";
}

}
