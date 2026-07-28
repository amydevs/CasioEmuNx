#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
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

const GLuint WIDTH = 1280, HEIGHT = 720;

SDL_Window *window;
SDL_GLContext context;

std::vector<std::pair<std::string, std::string>> getModels() {
	std::vector<std::pair<std::string, std::string>> models;
	
	// Try different possible paths
	std::vector<std::string> possible_paths = {
		"../models",
		"../../models",
        "../share/casioemu/models",
		"models"
	};
	
	for (const auto& models_path : possible_paths) {
		DIR* dir = opendir(models_path.c_str());
		if (dir != NULL) {
			struct dirent* entry;
			while ((entry = readdir(dir)) != NULL) {
				if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
                    std::filesystem::path absolute_path = std::filesystem::canonical(models_path + "/" + entry->d_name);
					models.push_back({ absolute_path.string(), entry->d_name });
				}
			}
			closedir(dir);
			if (!models.empty()) {
				std::sort(models.begin(), models.end(), 
					[](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) { return a.second < b.second; });
				break;
			}
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
                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED
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

    if ( init() ) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();

        io.Fonts->AddFontDefault();
        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        std::vector<std::pair<std::string, std::string>> models = getModels();

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
                    default:
                        break;
                }
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);

            ImGui::NewFrame();

            // Main window covering the entire screen
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
                if (ImGui::Button(models[i].second.c_str(), ImVec2(button_width, button_height))) {
#ifdef __SWITCH__
                    std::string absolute_nro_path = std::filesystem::canonical("./casioemu.nro").string();
                    std::string args = absolute_nro_path + " " + models[i].first;
                    envSetNextLoad(absolute_nro_path.c_str(), args.c_str());
                    exit = 1;
#else
                    std::string cmd = "../emulator/build/emulator model=" + models[i].first;
                    system(cmd.c_str());
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
    return 0;
}


