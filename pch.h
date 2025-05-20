#pragma once
#include <string>
#include <iostream>
#include <print>
#include <cstdint>
#include <vector>
#include <array>

#include <Windows.h>

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

#include "d3d11.h"

#include "ImGui/imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "vmmdll.h"

#include "Offsets.h"