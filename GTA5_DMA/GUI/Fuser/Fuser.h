#pragma once

class Fuser
{
public:
	static bool OnFrame();
    static inline std::vector<int> iMonitors;
	static inline std::vector<RECT> rcMonitors;
    static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
    {
        rcMonitors.push_back(*lprcMonitor);

        iMonitors.push_back(rcMonitors.size());

        return TRUE;
    }
    static bool GetDisplayInfo()
    {
        iMonitors.clear();
        rcMonitors.clear();

        EnumDisplayMonitors(0, 0, MonitorEnum, 0);

        return 1;
    }

public:
	static inline bool m_Fuser = false;
    static inline int m_MonitorIndex = 1;
    static inline int m_XPosition = 0;
    static inline int m_YPosition = 0;
    static inline int m_Width = 1920;
    static inline int m_Height = 1080;
};