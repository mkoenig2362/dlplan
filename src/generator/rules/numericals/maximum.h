#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_MAXIMUM_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_MAXIMUM_H_

#include "../rule.h"


namespace dlplan::generator::rules {
class MaximumNumerical : public Rule {
public:
    void generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override;

    std::string get_name() const override;
};

}

#endif
