/**
 * @file Loudness.hpp
 * @brief Calculates loudness values in relation to previous frames.
**/

#pragma once

#include "Audio/AudioConstants.hpp"

#include <projectM-4/projectM_cxx_export.h>

#include <array>
#include <cstdint>

namespace libprojectM {
namespace Audio {

/**
 * @brief Calculates beat-detection loudness relative to the previous frame(s).
 */
class PROJECTM_CXX_EXPORT Loudness
{
public:
    /**
     * @brief Frequency bands.
     * These are just labels for external API purposes (matching the classic Milkdrop
     * bass/mid/treb naming). The actual Hz range summed for each band is configurable
     * at runtime via Configure() and defaults to sensible kick/mid/treble-focused values
     * rather than the original spectrum-sixths split.
     */
    enum class Band : std::uint8_t
    {
        Bass = 0,
        Middles = 1,
        Treble = 2
    };

    /**
     * @brief Constructor.
     * @param band The band label to use for this loudness instance (for identification only).
     * @param lowHz Lower boundary of the frequency range to sum, in Hz.
     * @param highHz Upper boundary of the frequency range to sum, in Hz.
     * @param sampleRate The sample rate of the incoming audio data, in Hz. Defaults to 44100,
     *                    the de-facto standard for SDL2 capture. Pass the real capture rate if
     *                    known/different, or boundaries will be proportionally off.
     */
    explicit Loudness(Band band, float lowHz, float highHz, float sampleRate = 44100.0f);

    /**
     * @brief Reconfigures the frequency range summed by this instance at runtime.
     * Safe to call at any time between frames (not thread-safe against concurrent Update()
     * calls from another thread).
     * @param lowHz Lower boundary of the frequency range to sum, in Hz.
     * @param highHz Upper boundary of the frequency range to sum, in Hz.
     * @param sampleRate The sample rate of the incoming audio data, in Hz.
     */
    void Configure(float lowHz, float highHz, float sampleRate = 44100.0f);

    /**
     * @brief Updates the beat detection values and averages.
     * Must only be called once per frame.
     * @param spectrumSamples The current frame's spectrum analyzer samples to use for the update.
     * @param secondsSinceLastFrame (Fractional) seconds passed since the last frame.
     * @param frame The number of frames already rendered since start. For the first few frames, a different dampening factor
     *              will be used to avoid "jumpy" behaviour of the values.
     */
    void Update(const std::array<float, SpectrumSamples>& spectrumSamples, double secondsSinceLastFrame, uint32_t frame);

    /**
     * @brief Returns the current frame's unattenuated loudness relative to the previous frame.
     * This value will revolve around 1.0, with <0.7 being very silent and >1.3 very loud audio.
     * @return The current frame's unattenuated loudness relative to the previous frame.
     */
    auto CurrentRelative() const -> float;

    /**
     * @brief Returns the attenuated loudness averaged over the previous frames.
     * This value will revolve around 1.0, with <0.7 being very silent and >1.3 very loud audio.
     * Does not change as much as the value returned by CurrentRelative().
     * @return The attenuated loudness averaged over the previous frames.
     */
    auto AverageRelative() const -> float;

    /**
     * @brief Returns the value of the last detected peak (local maximum), decaying
     * back toward the baseline between hits. Useful for UI display of "how hard
     * was the last hit" rather than a smoothed average.
     */
    auto LastPeak() const -> float;

    /**
     * @brief Returns the currently configured lower boundary, in Hz.
     */
    auto LowHz() const -> float
    { return m_lowHz; }

    /**
     * @brief Returns the currently configured upper boundary, in Hz.
     */
    auto HighHz() const -> float
    { return m_highHz; }

    /**
     * @brief Sets this band's individual sensitivity multiplier.
     * Scales how far CurrentRelative()/AverageRelative() swing away from their 1.0
     * baseline. 0.0 flatlines the band to 1.0 (looks silent regardless of playback
     * volume). 1.0 (default) passes values through unchanged. >1.0 exaggerates swings.
     * @param sensitivity The sensitivity multiplier, clamped to >= 0.
     */
    void SetSensitivity(float sensitivity);

    /**
     * @brief Returns this band's current sensitivity multiplier.
     */
    auto GetSensitivity() const -> float;

    /**
     * @brief Sets a global multiplier applied on top of this band's own sensitivity.
     * Used by the global "all bands" sensitivity control so it scales each band's
     * existing setting proportionally, rather than overwriting it outright - a band
     * set to 0 stays at 0 regardless of the global multiplier's value.
     */
    void SetGlobalMultiplier(float multiplier);

    /**
     * @brief Returns the current global multiplier.
     */
    auto GetGlobalMultiplier() const -> float;

private:
    /**
     * @brief Sums up the spectrum samples for the configured bin range.
     * @param spectrumSamples The spectrum analyzer samples to use.
     */
    void SumBand(const std::array<float, SpectrumSamples>& spectrumSamples);

    /**
     * @brief Updates the short- and long-term averages and relative values.
     * @param secondsSinceLastFrame (Fractional) seconds passed since the last frame.
     * @param frame The number of frames already rendered since start.
     */
    void UpdateBandAverage(double secondsSinceLastFrame, uint32_t frame);

    /**
     * @brief Adjusts the dampening rate according to the current FPS.
     * @param rate The rate to be dampened.
     * @param secondsSinceLastFrame (Fractional) seconds passed since the last frame.
     * @return The dampened rate value.
     */
    static auto AdjustRateToFps(float rate, double secondsSinceLastFrame) -> float;

    Band m_band{Band::Bass}; //!< The frequency band label for this instance (identification only).

    float m_lowHz{};   //!< Configured lower boundary, in Hz.
    float m_highHz{};  //!< Configured upper boundary, in Hz.
    int m_startBin{0}; //!< Computed start bin index, inclusive.
    int m_endBin{0};   //!< Computed end bin index, exclusive.

    float m_sensitivity{1.0f}; //!< Per-band gain: scales swing from 1.0 baseline. 0 = flat/silent.
    float m_globalMultiplier{1.0f}; //!< Global "all bands" multiplier, applied on top of m_sensitivity.

    float m_current{};     //!< The current frame's sum of all frequency strengths in the current band.
    float m_average{};     //!< The short-term averaged value of m_current.
    float m_longAverage{}; //!< The long-term averaged value of m_current.

    float m_currentRelative{1.0f}; //!< The relative loudness value to the previous frame.
    float m_averageRelative{1.0f}; //!< The attenuated relative loudness value.
    float m_lastPeak{1.0f};        //!< The last detected peak (local max), decaying back toward baseline between hits.
};

} // namespace Audio
} // namespace libprojectM