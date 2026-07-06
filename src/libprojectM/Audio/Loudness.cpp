#include "Audio/Loudness.hpp"

#include <algorithm>
#include <cmath>

namespace libprojectM {
namespace Audio {

Loudness::Loudness(Loudness::Band band, float lowHz, float highHz, float sampleRate)
    : m_band(band)
{
    Configure(lowHz, highHz, sampleRate);
}

void Loudness::Configure(float lowHz, float highHz, float sampleRate)
{
    m_lowHz = lowHz;
    m_highHz = highHz;

    // The FFT produces SpectrumSamples bins spanning 0 Hz to Nyquist (sampleRate / 2).
    float const binWidthHz = sampleRate / (SpectrumSamples * 2.0f);

    m_startBin = std::clamp(static_cast<int>(lowHz / binWidthHz), 0, SpectrumSamples);
    m_endBin = std::clamp(static_cast<int>(highHz / binWidthHz), 0, SpectrumSamples);

    // Guard against inverted or zero-width ranges from bad config input.
    if (m_endBin <= m_startBin)
    {
        m_endBin = std::min(m_startBin + 1, SpectrumSamples);
    }
}

void Loudness::Update(const std::array<float, SpectrumSamples>& spectrumSamples, double secondsSinceLastFrame, uint32_t frame)
{
    SumBand(spectrumSamples);
    UpdateBandAverage(secondsSinceLastFrame, frame);
}

auto Loudness::CurrentRelative() const -> float
{
    return 1.0f + (m_sensitivity * m_globalMultiplier) * std::max(0.0f, m_currentRelative - 1.0f);
}

auto Loudness::AverageRelative() const -> float
{
    return 1.0f + (m_sensitivity * m_globalMultiplier) * std::max(0.0f, m_averageRelative - 1.0f);
}

auto Loudness::LastPeak() const -> float
{
    return 1.0f + (m_sensitivity * m_globalMultiplier) * std::max(0.0f, m_lastPeak - 1.0f);
}

void Loudness::SetSensitivity(float sensitivity)
{
    m_sensitivity = std::max(0.0f, sensitivity);
}

auto Loudness::GetSensitivity() const -> float
{
    return m_sensitivity;
}

void Loudness::SetGlobalMultiplier(float multiplier)
{
    m_globalMultiplier = std::max(0.0f, multiplier);
}

auto Loudness::GetGlobalMultiplier() const -> float
{
    return m_globalMultiplier;
}

void Loudness::SumBand(const std::array<float, SpectrumSamples>& spectrumSamples)
{
    m_current = 0.0f;
    for (int sample = m_startBin; sample < m_endBin; sample++)
    {
        m_current += spectrumSamples[sample];
    }
}

void Loudness::UpdateBandAverage(double secondsSinceLastFrame, uint32_t frame)
{
    float rate = AdjustRateToFps(m_current > m_average ? 0.2f : 0.5f, secondsSinceLastFrame);
    m_average = m_average * rate + m_current * (1.0f - rate);

    rate = AdjustRateToFps(frame < 50 ? 0.9f : 0.992f, secondsSinceLastFrame);
    m_longAverage = m_longAverage * rate + m_current * (1.0f - rate);

    m_currentRelative = std::fabs(m_longAverage) < 0.001f ? 1.0f : m_current / m_longAverage;
    m_averageRelative = std::fabs(m_longAverage) < 0.001f ? 1.0f : m_average / m_longAverage;

    // Peak-hold-and-decay: snaps up instantly to a new hit, decays gradually
    // back toward baseline between hits, rather than tracking a smoothed average.
    float peakDecayRate = AdjustRateToFps(0.99f, secondsSinceLastFrame);
    m_lastPeak = std::max(m_currentRelative, 1.0f + (m_lastPeak - 1.0f) * peakDecayRate);
}

auto Loudness::AdjustRateToFps(float rate, double secondsSinceLastFrame) -> float
{
    float const perSecondDecayRateAtFps1 = std::pow(rate, 30.0f);
    float const perFrameDecayRateAtFps2 = std::pow(perSecondDecayRateAtFps1, static_cast<float>(secondsSinceLastFrame));

    return perFrameDecayRateAtFps2;
}

} // namespace Audio
} // namespace libprojectM