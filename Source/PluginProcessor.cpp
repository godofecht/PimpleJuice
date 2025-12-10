#include "PluginProcessor.h"
#include "PluginEditor.h"

PimpleJuiceAudioProcessor::PimpleJuiceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
#endif
      apvts(*this, nullptr, "Parameters", createParameterLayout()) {
}

PimpleJuiceAudioProcessor::~PimpleJuiceAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout
PimpleJuiceAudioProcessor::createParameterLayout() {
  std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

  params.push_back(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"gain", 1}, "Gain",
      juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f));

  params.push_back(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"mix", 1}, "Mix",
      juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));

  return {params.begin(), params.end()};
}

const juce::String PimpleJuiceAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool PimpleJuiceAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool PimpleJuiceAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool PimpleJuiceAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double PimpleJuiceAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int PimpleJuiceAudioProcessor::getNumPrograms() { return 1; }

int PimpleJuiceAudioProcessor::getCurrentProgram() { return 0; }

void PimpleJuiceAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String PimpleJuiceAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void PimpleJuiceAudioProcessor::changeProgramName(int index,
                                                  const juce::String &newName) {
  juce::ignoreUnused(index, newName);
}

void PimpleJuiceAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void PimpleJuiceAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PimpleJuiceAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void PimpleJuiceAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;

  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  float gain = apvts.getRawParameterValue("gain")->load();
  float mix = apvts.getRawParameterValue("mix")->load();

  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto *channelData = buffer.getWritePointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      float dry = channelData[sample];
      float wet = dry * gain;
      channelData[sample] = dry * (1.0f - mix) + wet * mix;
    }
  }
}

bool PimpleJuiceAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *PimpleJuiceAudioProcessor::createEditor() {
  return new PimpleJuiceAudioProcessorEditor(*this);
}

void PimpleJuiceAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
  auto state = apvts.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void PimpleJuiceAudioProcessor::setStateInformation(const void *data,
                                                    int sizeInBytes) {
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));
  if (xmlState.get() != nullptr)
    if (xmlState->hasTagName(apvts.state.getType()))
      apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new PimpleJuiceAudioProcessor();
}
