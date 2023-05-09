#pragma once
#include "Main.hpp"
#include <iostream>
#include <thread>
namespace ClickMapper {
	void MainLayer::OnAttach(){
		m_keyThread = new std::thread(&MainLayer::ListenForKeys, this);
		m_procSelected = 0;
	}

	void MainLayer::OnUIRender()
	{
		ImGui::Begin("Key Mapper");
		std::vector<std::pair<std::string, HWND>> proc = Proccess::GetProcesses();
		if (ImGui::BeginCombo("Process to Hook into", proc.at(m_procSelected).first.c_str())) {
			for (int i = 0; i < proc.size(); i++) {
				std::pair<std::string, HWND> process = proc.at(i);
				if (proc.at(i).first.empty()) {
					continue;
				}
				const bool isSelected = (i == m_procSelected);
				if (ImGui::Selectable(process.first.c_str(), isSelected)) {
					m_procSelected = i;
					m_HookedProc = process.second;
					HWND parentHwnd = (m_HookedProc);
					SetForegroundWindow(parentHwnd);
				}

				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}


		ImGui::Combo("Key to replace", &m_selectedOriginalKey, Keys, IM_ARRAYSIZE(Keys));
		ImGui::Combo("Replace with Key", &m_selectedReplaceKey, Keys, IM_ARRAYSIZE(Keys));
		//TODO: add function
		ImGui::Checkbox("Check for held down state", &m_checkHeldDownState);

		if (ImGui::Button("Create replacement")) {
			int VKKeyToReplace = m_KeyTranslation[m_selectedOriginalKey];
			int VKKeyReplaceWith = m_KeyTranslation[m_selectedReplaceKey];
			{
				std::lock_guard<std::mutex> lock(m_keyMutex);
				m_registeredKeys.insert(std::make_pair(VKKeyToReplace, VKKeyReplaceWith));
			}
		}

		if (ImGui::BeginTable("Replaced Keys", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
			ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
			ImGui::TableSetupColumn("Original Key", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Replaced With", ImGuiTableColumnFlags_None);
			{
				std::lock_guard<std::mutex> lock(m_keyMutex);
				for (auto& [oldKey, replacedKey] : m_registeredKeys) {
					ImGui::TableNextRow();
					for (int i = 0; i < 2; i++) {
						ImGui::TableSetColumnIndex(i);
						ImGui::Text("%s", VKToKeyName(i % 2 == 0 ? oldKey : replacedKey).c_str());
					}
				}
			}
			ImGui::EndTable();
		}
		ImGui::End();

	}

	void MainLayer::ListenForKeys() {
		while (true) {
			{
				//Prevent race conditions.
				std::lock_guard<std::mutex> lock(m_keyMutex);
				for (auto& [oldKey, replacementKey] : m_registeredKeys) {

					if (GetForegroundWindow() == m_HookedProc && GetKeyState(oldKey) & 0x8000 /*Check if down and not if toggled (e.g. (1 << 15))*/) {
						HandleKey(m_HookedProc, replacementKey);
						using namespace std::chrono_literals;
						std::this_thread::sleep_for(1ms);
					}
				}
			}
		}
	} 
}