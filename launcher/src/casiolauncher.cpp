#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

#include <SDL.h>
#include <glad/glad.h>
#ifdef __SWITCH__
#include <switch.h>
#endif

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#ifndef __SWITCH__
#include "whereami.h"
#endif

namespace fs = std::filesystem;

const GLuint WIDTH = 1280, HEIGHT = 720;

SDL_Window *window;
SDL_GLContext context;

const std::vector<std::string> possible_casioemu_paths = {
    "./casioemu",
    "../emulator/casioemu",
    "../emulator/build/casioemu",
};

const std::vector<std::string> possible_models_paths = {
    "../models",
    "../../models",
    "../share/casioemu/models",
    "models",
};

std::vector<fs::directory_entry> getModelsDirectoryEntries(fs::path root_path) {
	std::vector<fs::directory_entry> models;

    for (const auto& models_subpath : possible_models_paths) {
        fs::path models_path = root_path / models_subpath;
        if (!fs::exists(models_path) || !fs::is_directory(models_path)) {
            continue;
        }
        for (const auto& entry : fs::directory_iterator(models_path)) {
            if (!entry.is_directory()) {
                continue;
            }
            models.push_back(entry);
        }
    }
	
	return models;
}

static bool init() {
    bool success = true;

    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0 ){
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success =  false;
    }
    else {
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        window = SDL_CreateWindow(
                "Casio Emulator Launcher",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WIDTH, HEIGHT,
                SDL_WINDOW_OPENGL |
                SDL_WINDOW_SHOWN |
                SDL_WINDOW_RESIZABLE
        );
        if( window == NULL ){
            printf("%s: Window could not be created! SDL Error: %s", __func__, SDL_GetError());
            success =  false;
        }
        else {
            context = SDL_GL_CreateContext(window);
            if( context == NULL )
            {
                printf( "%s: OpenGL context could not be created! SDL Error: %s", __func__, SDL_GetError());
                success =  false;
            }
            else {
                gladLoadGL();
            }
        }
    }
    return success;
}
int main() {
#ifndef __SWITCH__
    char executable_path[PATH_MAX];
    wai_getExecutablePath(executable_path, sizeof(executable_path), NULL);
    fs::path executable_dir = fs::path(executable_path).parent_path();
#else
    freopen("casiolauncher.log", "w", stdout);
    setvbuf(stdout, NULL, _IOLBF, 1024);
    fs::path executable_dir = fs::path("./");
#endif
    
    if ( init() ) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();

        ImFontConfig config;
        config.SizePixels = 24.0f;

        io.Fonts->AddFontDefault(&config);
        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        std::vector<fs::directory_entry> models = getModelsDirectoryEntries(fs::path(executable_dir));

        int exit = 0;
        while (!exit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                switch (event.type) {
                    case SDL_QUIT:
                        exit = 1;
                        break;
                    case SDL_KEYUP:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            exit = 1;
                        }
                        break;
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            io.DisplaySize.x = (float) event.window.data1;
                            io.DisplaySize.y = (float) event.window.data2;
                        }
                        break;
                    default:
                        break;
                }
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);

            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);
            
            ImGui::Begin("Casio Models", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
            
            ImGui::Text("Select a model to launch:");
            ImGui::Spacing();
            
            float button_width = 300.0f;
            float button_height = 60.0f;
            float window_width = ImGui::GetContentRegionAvail().x;
            float offset_x = (window_width - button_width) * 0.5f;
            
            for (size_t i = 0; i < models.size(); ++i) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset_x);
                if (ImGui::Button(models[i].path().filename().string().c_str(), ImVec2(button_width, button_height))) {
#ifdef __SWITCH__
                    fs::path nro_path = fs::canonical("./casioemu.nro");
                    fs::path model_path = fs::canonical(models[i].path());
                    std::string args = nro_path.string() + " " + model_path.string();
                    envSetNextLoad(nro_path.c_str(), args.c_str());
                    exit = 1;
#else
                    for (const auto& casioemu_subpath : possible_casioemu_paths) {
                        fs::path casioemu_path = executable_dir / casioemu_subpath;
                        if (fs::exists(casioemu_path)) {
                            std::string cmd = fs::canonical(casioemu_path).string() + " " + models[i].path().string();
                            SDL_HideWindow(window);
                            system(cmd.c_str());
                            SDL_ShowWindow(window);
                            break;
                        }
                    }
#endif
                }
            }
            
            ImGui::End();

            ImGui::Render();
            glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);

            glClearColor(0.1f, 0.1f, 0.1f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            SDL_GL_SwapWindow(window);
            SDL_Delay(1);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(context);
    }
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
#ifdef __SWITCH__
	fclose(stdout);
#endif
    return 0;
}


