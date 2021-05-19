/******************************************************************************/
/*!
\file   main.cpp
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/

#include "os_win32.h"
#include "render_dx11.h"

#include <imgui.h>
#include "../external/implot/implot.h"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <iostream>

#include "ReverbHandler.h"


//TODO
// CUSTOM G R SLIDERS


namespace dx = DirectX;

namespace
{
int screen_width = 1280, screen_height = 720;
}

int main()
{
  CS350::OS_Win32 os{ TEXT("Moorer Reverb"), screen_width, screen_height };
  CS350::Render_DX11 render{ os };
  os.Show();

  float3 clear_color{ 0.15f, 0.15f, 0.15f };

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

  io.ConfigViewportsNoTaskBarIcon = true;

  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  ImGui_ImplWin32_Init(os.GetWindowHandle());
  ImGui_ImplDX11_Init(render.GetD3D11Device(), render.GetD3D11Context());

  double current_time = os.GetTime();
  bool show_imgui_demo = false;

  ReverbHandler reverb;

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.0f);
  ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 1.f, 0.717f, 0.5f));

  while (!os.ShouldClose())
  {
    float dt;
    {
      double previous_time = current_time;
      current_time = os.GetTime();
      dt = static_cast<float>(current_time - previous_time);
      if (dt > .25f)
      {
        dt = (1.0f / 60.0f);
      }
    }

    // message loop
    os.HandleMessages();

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // program loop

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
    ImGui::SetNextWindowSize({ (float)os.GetWidth(), (float)os.GetHeight() });
    ImGui::Begin("root_window", nullptr, ImGuiWindowFlags_NoDecoration
                 | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking);
    ImGui::DockSpace(ImGui::GetID("root_window"), {}, ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    // if (ImGui::BeginMainMenuBar())
    // {
    //   if (ImGui::BeginMenu("File"))
    //   {
    //     ImGui::EndMenu();
    //   }
    //   if (ImGui::BeginMenu("View"))
    //   {
        
    //     ImGui::EndMenu();
    //   }
    //   ImGui::EndMainMenuBar();
    // }

    reverb.update();

    if (show_imgui_demo)
    {
      ImGui::ShowDemoWindow(&show_imgui_demo);
      //ImPlot::ShowDemoWindow(&show_imgui_demo);
    }

    ImGui::GetOverlayDrawList()->AddCircleFilled(io.MousePos, 20.0f, ImGui::GetColorU32({ 1, 0, 1, 0.5f }));

    render.SetClearColor(clear_color);
    render.ClearDefaultFramebuffer();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    render.Present();
  }

  return 0;
}

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
  return main();
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  return main();
}
