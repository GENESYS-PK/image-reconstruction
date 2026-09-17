#pragma once
#include "pch.hpp"

using clk_t = std::chrono::steady_clock;

class Application
{
private:
	int m_xres = 0;
	int m_yres = 0;
	bool m_running = true;

	clk_t::duration m_frameDelay;
	clk_t::time_point m_nextRefresh;

	ImGuiIO* m_io = nullptr;
	ImGuiContext* m_context = nullptr;
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	inline static int s_instances = 0;

	void init();
	void cleanup();
	void handle_events();
	void draw();

public:
	Application();
	~Application();

	void run();
};
