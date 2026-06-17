#pragma once
#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>

#include <iostream>

class MyProcessor
{
public:
  halp_meta(name, "My Processor")
  halp_meta(c_name, "my_processor")

  // CHANGE THIS !!
  // - On linux: uuidgen | xargs printf | xclip -selection clipboard
  //   will copy one on the clipboard
  // - uuidgen exists on Mac and Windows too
  halp_meta(uuid, "00000000-0000-0000-0000-000000000000")
  // halp_meta(channels, 2)

  struct
  {
    halp::fixed_audio_bus<"Input", float, 1> audio;
    struct : halp::hslider_f32<"Gain", halp::range{.min = 0., .max = 1., .init = 0.5}>
    {
      void update(MyProcessor& m) { std::cerr << "okie " << value << "\n"; }
    } gain;
  } inputs;

  struct
  {
    halp::fixed_audio_bus<"Input", float, 1> audio;
  } outputs;

  void operator()(int N);

  // Defined in UI.hpp
  struct ui;
};

// Defined inline here, not in a separate .cpp: Avendish back-ends compile this header
// directly and several (e.g. the GDExtension / DLL-style modules) do not link the
// MyProcessor library, so a .cpp-defined operator() is an unresolved external at link
// time on strict linkers (macOS/Windows). Keep the processing logic in the header.
inline void MyProcessor::operator()(int N)
{
  for(int i = 0; i < this->inputs.audio.channels(); i++)
  {
    auto* in = this->inputs.audio[i];
    auto* out = this->outputs.audio[i];
    for(int j = 0; j < N; j++)
    {
      out[j] = in[j] * inputs.gain;
    }
  }
}

// Pull in the UI so MyProcessor::ui is complete wherever this class header is
// included -- the score plugin generator includes the class header, not the
// MAIN_FILE, and needs the full ui type to build the process's layer.
#include "UI.hpp"
