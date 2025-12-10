#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
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

//==============================================================================
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

int PimpleJuiceAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

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

//==============================================================================
void PimpleJuiceAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void PimpleJuiceAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PimpleJuiceAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
    return false;
  }

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
    return false;
  }
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

#if !JucePlugin_IsSynth
  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }
#endif

  float gain = apvts.getRawParameterValue("gain")->load();
  // float mix = apvts.getRawParameterValue ("mix")->load(); // Mix is less
  // relevant for a pure synth, but keeping for now if we add FX later

  for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
    auto *channelData = buffer.getWritePointer(channel);

    // For a synth, we would render voices here.
    // For now, let's just silence the output so it doesn't blast garbage.
    // Once we add a synth engine, we will render audio here.
    if (totalNumInputChannels == 0) {
      buffer.clear(channel, 0, buffer.getNumSamples());
    } else {
      // If we somehow have inputs (e.g. effect synth), pass them through
      // modified But usually synths ignore input.
      for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        channelData[sample] *= gain;
      }
    }
  }
}

//==============================================================================
bool PimpleJuiceAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *PimpleJuiceAudioProcessor::createEditor() {
  return new PimpleJuiceAudioProcessorEditor(*this);
}

//==============================================================================
void PimpleJuiceAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries
  auto state = apvts.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void PimpleJuiceAudioProcessor::setStateInformation(const void *data,
                                                    int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));
  if (xmlState.get() != nullptr) {
    if (xmlState->hasTagName(apvts.state.getType())) {
      apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
  }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new PimpleJuiceAudioProcessor();
}
