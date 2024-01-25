#pragma once
#include <shared_mutex>
#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>
#include "utils/KeyManager.hpp"
#include "utils/ProccessManager.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

namespace ClickMapper {
	static HWND g_procSelected;
	
	class MainLayer : public Walnut::Layer {
	private:
		static constexpr char* Keys[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Left Mouse Button", "Right Mouse Button", "Middle Mouse Button" };;

		int m_procSelected;
		int m_selectedOriginalKey;
		int m_selectedReplaceKey;
		bool m_checkHeldDownState;
		HWND m_HookedProc;
		std::vector<int> m_KeyTranslation{0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x58, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x01, 0x02, 0x04 };
		std::thread* m_keyThread;
		std::mutex m_keyMutex;

		std::unique_ptr<KeyManager> m_keyManager;
	public:
		virtual void OnAttach() override;
		virtual void OnUIRender() override;
		void ListenForKeys();
	};
}


Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
	Walnut::ApplicationSpecification spec;
	spec.Height = 500;
	spec.Width = 800;
	spec.Name = "Ehbw | Button Mapper";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ClickMapper::MainLayer>();
	return app;
}
