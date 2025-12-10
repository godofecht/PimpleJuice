# PimpleJuice 🍊

The bubble's gotta burst.
A better alternative to PampleJuice.

In progress. Don't make fun of me.

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
