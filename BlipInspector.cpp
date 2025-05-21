#include "pch.h"

#include "BlipInspector.h"

bool BlipInspector::OnFrame()
{
	if (!m_BlipInspector) return 1;

	std::scoped_lock BlipInspectorLock(BlipInspector::m_BlipInspectorMutex);

	ImGui::Begin("Blip Inspector", &m_BlipInspector);

	for (auto& BlipInfo : m_Blips)
	{
		if (BlipInfo.ID == 0) continue;
		ImGui::Text(std::format("ID: {0:d}, Distance: {1:.2f}", BlipInfo.ID, BlipInfo.WorldPosition.Distance(m_LastPlayerPosition)).c_str());
	}

	ImGui::End();

	return 1;
}