#pragma once

#include <string>
#include <vector>

#include "Hooks/Base/HookBase.hpp"

/**
 * A FeatureSet is a set of features that only make sense to be enabled together.
 * A FeatureSet requires a collection of hooks.
 * If one hook cannot be enabled, none of the features will be enabled.
 * FeatureSets can also contain sub FeatureSets.
 * If the parent FeatureSet is not enabled, the sub FeatureSets will not be enabled either.
 * TODO isolated state for feature sets?
 * 
 * Usage:
 * - Initialize a FeatureSet object.
 * - Add all required hooks with AddRequiredHook.
 * - Add all sub FeatureSets with AddSubFeatureSet.
 * - Call EnableRecursively on the root FeatureSet.
 */
class FeatureSet
{
public:
    FeatureSet(std::string name) : name(name) {};

    void AddRequiredHook(HookBase& hook) { required_hooks.emplace_back(&hook); }
    void AddSubFeatureSet(FeatureSet& feature_set) { sub_featuresets.emplace_back(&feature_set); }

    /**
     * Enable all hooks and sub FeatureSets.
     * If a sub FeatureSet couldn't be enabled, it's own sub FeatureSets will be skipped as well.
     * 
     * Returns an aggregated error message of all FeatureSets that could not be enabled,
     * and a bool whether the original FeatureSet could be enabled.
     * Usage:
     * - Call it once, on the root (core) FeatureSet.
     */
    std::tuple<bool, std::string> EnableRecursively()
    {
        std::string string;

        for (auto& hook : required_hooks)
        {
            if (!hook->IsValid())
            {
                string += name + " could not be enabled!\n";
                return { false, string };
            }
        }
        for (auto& hook : required_hooks)
        {
            hook->Enable();
        }
        for (auto& feature_set : sub_featuresets)
        {
            string += std::get<1>(feature_set->EnableRecursively());
        }

        return { true, string };
    }

private:
    std::string name;
    std::vector<HookBase*> required_hooks;
    std::vector<FeatureSet*> sub_featuresets;
};
