// The MyProcessor implementation lives entirely in Model.hpp (inline), because the
// Avendish back-ends compile the header directly and several do not link this library.
// This translation unit only exists so the MyProcessor CMake target has a source to
// compile; put any non-inline / heavy helper code here if you ever need it.
#include "Model.hpp"
