#include "../Data/HardwareId.hpp"
#include "CodeViewer.hpp"
#include "SDL_timer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include "ui.hpp"
#include <SDL.h>
#include <cstddef>
#include <iostream>

#include "hex.hpp"

char *n_ram_buffer = nullptr;
CodeViewer *code_viewer = nullptr;
static SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
static SDL_Window *window;
static SDL_Renderer *renderer;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void debugger_gui_loop() {
    if (!m_emu->Running())
        return;

    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    static MemoryEditor mem_edit;
    if (n_ram_buffer != nullptr) {
        size_t base = m_emu->hardware_id == casioemu::HW_ES_PLUS ? 0x8000 : 0xD000;
        size_t size = m_emu->hardware_id == casioemu::HW_ES_PLUS ? 0x0E00 : 0x2000;
        mem_edit.DrawWindow("RAM Editor", n_ram_buffer, size, base);
    }
    code_viewer->DrawWindow();

    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

int init_debugger_window() {
    window = SDL_CreateWindow("CasioEmuX Debugger", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 600, window_flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.WantCaptureKeyboard = true;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.FontGlobalScale = 1.0f;

    // Setup Platform/Renderer backends
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    code_viewer = new CodeViewer(m_emu->GetModelFilePath("_disas.txt"));

    return 0;
}
