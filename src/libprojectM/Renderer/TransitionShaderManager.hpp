#pragma once

#include "Renderer/Shader.hpp"

#include <projectM-4/projectM_cxx_export.h>

#include <array>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

namespace libprojectM {
namespace Renderer {

/**
 * @brief Identifiers for the built-in transition shaders, in the same order
 * they are loaded/stored internally.
 */
enum class TransitionType : std::uint8_t
{
    Circle = 0,
    Plasma = 1,
    SimpleBlend = 2,
    Sweep = 3,
    Warp = 4,
    ZoomBlur = 5,
    BeatPulse = 6,
    Datamosh = 7,
    Count = 8 //!< Number of known transition types. Keep last.
};

/**
 * @brief Manages all available transition shaders.
 */
class PROJECTM_CXX_EXPORT TransitionShaderManager
{
public:
    TransitionShaderManager();

    /**
     * @brief Selects a random transition shader from the currently enabled subset.
     * If all transitions are disabled, falls back to selecting from the full set
     * so transitions never stop happening entirely.
     * @return A shared pointer to a transition shader.
     */
    auto RandomTransition() -> std::shared_ptr<Shader>;

    /**
     * @brief Enables or disables a transition type from the random selection pool.
     * @param type Which transition to configure.
     * @param enabled Whether it should be eligible for random selection.
     */
    void SetTransitionEnabled(TransitionType type, bool enabled);

    /**
     * @brief Returns whether a transition type is currently enabled.
     */
    auto IsTransitionEnabled(TransitionType type) const -> bool;

    /**
     * @brief Returns a human-readable name for a transition type, for UI display.
     */
    static auto TransitionName(TransitionType type) -> const char*;

private:
    /**
     * @brief Compiles a single transition shader program.
     * @param shaderBodyCode The mainImage() fragment shader code, without any headers etc.
     */
    static auto CompileTransitionShader(const std::string& shaderBodyCode) -> std::shared_ptr<Shader>;

    std::vector<std::shared_ptr<Shader>> m_transitionShaders; //!< Currently loaded and compiled transition shaders, indexed by TransitionType.

    std::array<bool, static_cast<size_t>(TransitionType::Count)> m_enabled{true, true, true, true, true, true, true, true}; //!< Per-type enabled flags, all on by default.

    std::random_device m_randomDevice; //!< Seed for the random number generator
    std::mt19937 m_mersenneTwister;    //!< Random engine to select shader
};

} // namespace Renderer
} // namespace libprojectM
