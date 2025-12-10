#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class PimpleJuiceAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  PimpleJuiceAudioProcessorEditor(PimpleJuiceAudioProcessor &);
  ~PimpleJuiceAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  PimpleJuiceAudioProcessor &audioProcessor;

  juce::Slider gainSlider;
  juce::Slider mixSlider;
  juce::Label gainLabel;
  juce::Label mixLabel;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gainAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      mixAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PimpleJuiceAudioProcessorEditor)
};
