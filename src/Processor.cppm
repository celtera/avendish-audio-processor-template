/* SPDX-License-Identifier: GPL-3.0-or-later */

// C++20 module version of the audio processor.
//
// The whole object lives in one module: `import halp;` brings in the halp
// helpers, and the processor (model + UI) is `export`ed so every Avendish
// back-end's generated prototype can `import my_processor;` instead of
// #including a header. Build requirements: a generator with module support
// (Ninja) and -DCMAKE_CXX_SCAN_FOR_MODULES=ON.
//
// Metadata is written as `static consteval` members rather than the halp_meta
// macro: a module's `import` does not carry macros across, so halp_meta is not
// visible here. consteval is exactly what halp_meta expands to, so nothing is
// lost. (On a C++26-reflection compiler this becomes `[[=halp::meta(...)]]`.)

module;
#include <iostream>

export module my_processor;

import halp;

export class MyProcessor
{
public:
  static consteval auto name() { return "My Processor"; }
  static consteval auto c_name() { return "my_processor"; }

  // CHANGE THIS !!
  // - On linux: uuidgen | xargs printf | xclip -selection clipboard
  //   will copy one on the clipboard
  // - uuidgen exists on Mac and Windows too
  static consteval auto uuid() { return "00000000-0000-0000-0000-000000000000"; }

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

  void operator()(int N)
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

  // The optional UI. Defined inline in the module purview so the type is
  // complete for every back-end that builds a UI from it.
  struct ui
  {
    static consteval auto layout() { return halp::layouts::vbox; }
    static consteval auto background() { return halp::colors::mid; }

    halp::label header{"Hello !"};

    struct
    {
      static consteval auto layout() { return halp::layouts::hbox; }
      static consteval auto background() { return halp::colors::dark; }
      // halp::item<&ins::gain> widget;
    } widgets;
  };
};
