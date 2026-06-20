/* SPDX-License-Identifier: GPL-3.0-or-later */

// In the module build the processor lives entirely in Processor.cppm, compiled
// per back-end as a C++20 module. This translation unit only exists so the
// addon's base static library has a source to compile; the back-ends import the
// module rather than linking object code from here. Put any non-module helper
// code (plain C++ the module does not need to own) here if you ever need it.
