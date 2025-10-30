#include "distance.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void DistanceFrame::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (int i = 1; i < target_complexity - 1; ++i) {
        int j = target_complexity - i - 1;
        for (const auto& f : data.m_frames_unary_by_iteration[i]) {
            auto element = factory.make_distance_frame(f);
            auto denotations = element->evaluate(states, caches);
            if (states.size() == 0 || data.m_frame_binary_hash_table.insert(denotations).second) {
                std::get<5>(data.m_generated_features).push_back(element);
                data.m_frames_binary_by_iteration[target_complexity].push_back(std::move(element));
                increment_generated();
            }
        }
    }
}

std::string DistanceFrame::get_name() const {
    return "f_distance";
}

}
