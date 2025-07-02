#pragma once

namespace MyImGui
{
	/* Interface */
	bool OnFrame();

	namespace Details
	{
		bool LoadFonts();
	}
};

class MainMenu
{
public:
	static inline bool m_MainMenu = false;
	static bool Render();
};