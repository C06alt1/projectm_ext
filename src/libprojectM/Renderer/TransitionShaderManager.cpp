#include "Renderer/TransitionShaderManager.hpp"

#include "BuiltInTransitionsResources.hpp"

namespace libprojectM {
namespace Renderer {

TransitionShaderManager::TransitionShaderManager()
    : m_transitionShaders({CompileTransitionShader(kTransitionShaderBuiltInCircleGlsl330),
                           CompileTransitionShader(kTransitionShaderBuiltInPlasmaGlsl330),
                           CompileTransitionShader(kTransitionShaderBuiltInSimpleBlendGlsl330),
                           CompileTransitionShader(kTransitionShaderBuiltInSweepGlsl330),
                           CompileTransitionShader(kTransitionShaderBuiltInWarpGlsl330),
                           CompileTransitionShader(kTransitionShaderBuiltInZoomBlurGlsl330),
                           CompileTransitionShader(kTransitionShaderBuiltInBeatPulseGlsl330),
                           CompileTransitionShader(kTransitionShaderBuiltInDatamoshGlsl330)})
    , m_mersenneTwister(m_randomDevice())
{
}

auto TransitionShaderManager::RandomTransition() -> std::shared_ptr<Shader>
{
    if (m_transitionShaders.empty())
    {
        return {};
    }

    // Build the list of indices currently eligible for selection.
    std::vector<size_t> eligible;
    for (size_t i = 0; i < m_transitionShaders.size(); i++)
    {
        if (m_enabled[i])
        {
            eligible.push_back(i);
        }
    }

    // If the user has disabled every transition, fall back to the full set
    // rather than returning nothing - transitions should never stop entirely.
    if (eligible.empty())
    {
        return m_transitionShaders.at(m_mersenneTwister() % m_transitionShaders.size());
    }

    return m_transitionShaders.at(eligible.at(m_mersenneTwister() % eligible.size()));
}

void TransitionShaderManager::SetTransitionEnabled(TransitionType type, bool enabled)
{
    auto index = static_cast<size_t>(type);
    if (index < m_enabled.size())
    {
        m_enabled[index] = enabled;
    }
}

auto TransitionShaderManager::IsTransitionEnabled(TransitionType type) const -> bool
{
    auto index = static_cast<size_t>(type);
    return index < m_enabled.size() ? m_enabled[index] : false;
}

auto TransitionShaderManager::TransitionName(TransitionType type) -> const char*
{
    switch (type)
    {
        case TransitionType::Circle:
            return "Circle";
        case TransitionType::Plasma:
            return "Plasma";
        case TransitionType::SimpleBlend:
            return "Fade";
        case TransitionType::Sweep:
            return "Sweep";
        case TransitionType::Warp:
            return "Warp";
        case TransitionType::ZoomBlur:
            return "Zoom Blur";
        case TransitionType::BeatPulse:
            return "Beat Pulse";
        case TransitionType::Datamosh:
            return "Datamosh";
        default:
            return "Unknown";
    }
}

auto TransitionShaderManager::CompileTransitionShader(const std::string& shaderBodyCode) -> std::shared_ptr<Shader>
{
#ifdef USE_GLES
    // GLES also requires a precision specifier for variables and 3D samplers
    constexpr char versionHeader[] = "#version 300 es\n\nprecision mediump float;\nprecision mediump sampler3D;\n";
#else
    constexpr char versionHeader[] = "#version 330\n\n";
#endif

    std::string fragmentShaderSource(static_cast<const char*>(versionHeader));
    fragmentShaderSource.append(kTransitionShaderHeaderGlsl330);
    fragmentShaderSource.append("\n");
    fragmentShaderSource.append(shaderBodyCode);
    fragmentShaderSource.append("\n");
    fragmentShaderSource.append(kTransitionShaderMainGlsl330);

    try
    {
        auto transitionShader = std::make_shared<Shader>();
        transitionShader->CompileProgram(static_cast<const char*>(versionHeader) + kTransitionVertexShaderGlsl330, fragmentShaderSource);
        return transitionShader;
    }
    catch (const ShaderException&)
    {
        // ToDo: Log proper shader compile error once logging API is in place
        return {};
    }
}

} // namespace Renderer
} // namespace libprojectM
