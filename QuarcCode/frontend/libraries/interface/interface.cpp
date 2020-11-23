#include "interface.hpp"

#include <windows.h>
#include <shobjidl.h>

#include "../../../backend/editor/editor.h"
#include "../../../backend/filesystem/filesystem.h"

QuarcInterface::QuarcInterface()
{
	static TextEditor editor;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", true))
		{
			if (ImGui::Button("Open File                                 Ctrl+O", { 200,0 })) { qFiles.OpenFile(); }

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit", true))
		{
			if (editor.CanUndo()) { if (ImGui::Button("Undo                                         Ctrl+Z", { 200,0 }))editor.Undo(); }
			else { ImGui::InactiveButton("Undo                                         Ctrl+Z", { 200,0 }); }

			if (editor.CanRedo()) { if (ImGui::Button("Redo                                         Ctrl+Y", { 200,0 }))	editor.Redo(); }
			else { ImGui::InactiveButton("Redo                                         Ctrl+Y", { 200,0 }); }

			ImGui::Separator();

			if (editor.HasSelection()) { if (ImGui::Button("Cut                                            Ctrl+X", { 200,0 }))		editor.Cut(); }
			else { ImGui::InactiveButton("Cut                                            Ctrl+X", { 200,0 }); }

			if (editor.HasSelection()) { if (ImGui::Button("Copy                                          Ctrl+C", { 200,0 })) editor.Copy(); }
			else { ImGui::InactiveButton("Copy                                          Ctrl+C", { 200,0 }); }

			if (ImGui::Button("Paste                                         Ctrl+V", { 200,0 })) editor.Paste();

			ImGui::EndMenu();
		}
	}

	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize - ImVec2(0, 18));
	ImGui::SetNextWindowPos({ 0, 19 });
	ImGui::Begin("##maincontent", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	{
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImVec2(50, ImGui::GetIO().DisplaySize.y), ImColor(0.19f, 0.19f, 0.19f));

		ImGui::SetCursorPos({ 50, 0 });
		if (ImGui::BeginTabBar("filestabbar", ImGuiTabBarFlags_Reorderable))
		{
			for (auto iter = qFiles.files_map.begin(); iter != qFiles.files_map.end(); iter++)
			{
				ImGui::BeginTabItem(iter->filename.c_str());

				if (ImGui::IsItemClicked(0))
				{
					editor.SetReadOnly(false);

					std::ifstream t(iter->path);

					if (t.good())
					{
						std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
						editor.SetText(str);
					}

					t.close();
				}
				else if (ImGui::IsItemClicked(1))
				{
					qFiles.files_map.erase(iter);
					editor.SetText("");
				}
			}

			ImGui::EndTabBar();
		}

		ImGui::SetCursorPos({ 50, 15 });
		editor.Render("file", { ImGui::GetIO().DisplaySize.x - 50, ImGui::GetIO().DisplaySize.y - 33 });
	}
	ImGui::End();
}