#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
PimpleJuiceAudioProcessorEditor::PimpleJuiceAudioProcessorEditor(
    PimpleJuiceAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(400, 300);

  // Gain Slider
  gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
  addAndMakeVisible(gainSlider);

  // Mix Slider
  mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
  addAndMakeVisible(mixSlider);

  // Labels
  gainLabel.setText("Gain", juce::dontSendNotification);
  gainLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(gainLabel);

  mixLabel.setText("Mix", juce::dontSendNotification);
  mixLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(mixLabel);

  // Attachments
  gainAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getAPVTS(), "gain", gainSlider);
  mixAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getAPVTS(), "mix", mixSlider);
}

PimpleJuiceAudioProcessorEditor::~PimpleJuiceAudioProcessorEditor() {}

//==============================================================================
void PimpleJuiceAudioProcessorEditor::paint(juce::Graphics &g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(juce::Colour(0xff1a1a2e));

  g.setColour(juce::Colour(0xff16213e));
  g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(10), 15.0f);

  g.setFont(juce::Font(28.0f, juce::Font::bold));
  g.setColour(juce::Colour(0xffe94560));
  g.drawText("PimpleJuice", getLocalBounds().removeFromTop(60),
             juce::Justification::centred, true);

  g.setFont(juce::Font(12.0f));
  g.setColour(juce::Colour(0xff0f3460));
  g.drawText("A Better Sampler", getLocalBounds().removeFromTop(80),
             juce::Justification::centred, true);
}

void PimpleJuiceAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  auto bounds = getLocalBounds().reduced(20);
  bounds.removeFromTop(80);

  auto sliderArea = bounds.reduced(20);
  int sliderWidth = sliderArea.getWidth() / 2;

  auto leftArea = sliderArea.removeFromLeft(sliderWidth);
  auto rightArea = sliderArea;

  gainLabel.setBounds(leftArea.removeFromTop(25));
  gainSlider.setBounds(leftArea.reduced(10));

  mixLabel.setBounds(rightArea.removeFromTop(25));
  mixSlider.setBounds(rightArea.reduced(10));
}
