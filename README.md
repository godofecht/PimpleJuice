# PimpleJuice 🍊

A better sampler plugin built with JUCE.

## Features

- 🎛️ Clean, modern UI with rotary controls
- 🎚️ Gain and Mix parameters with full automation support
- 💾 Parameter state persistence
- 🔌 VST3, AU, and Standalone formats

## Building

### Prerequisites

- CMake 3.24+
- C++20 compatible compiler
- JUCE framework

### Setup

1. Clone the repository:
```bash
git clone git@github.com:godofecht/PimpleJuice.git
cd PimpleJuice
```

2. Clone JUCE:
```bash
git clone https://github.com/juce-framework/JUCE.git
```

3. Build:
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Plugin Locations (macOS)

After building, plugins will be available in:
- **VST3**: `build/PimpleJuice_artefacts/Release/VST3/`
- **AU**: `build/PimpleJuice_artefacts/Release/AU/`
- **Standalone**: `build/PimpleJuice_artefacts/Release/Standalone/`

## License

MIT License
