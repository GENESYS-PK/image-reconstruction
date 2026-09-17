#include "pch.hpp"
#include "app.hpp"

void Application::init()
{
	cleanup();

	SDL_DisplayID display = SDL_GetPrimaryDisplay();
	if (display == 0)
		throw std::runtime_error(SDL_GetError());

	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
	if (mode == nullptr)
		throw std::runtime_error(SDL_GetError());

	SDL_Rect usableBounds;
	if (!SDL_GetDisplayUsableBounds(display, &usableBounds))
		throw std::runtime_error(SDL_GetError());

	m_xres = static_cast<int>(usableBounds.w * 0.95);
	m_yres = static_cast<int>(usableBounds.h * 0.9);

	if (!SDL_CreateWindowAndRenderer("Rekonstrukcja obrazka", m_xres, m_yres, 0, &m_window, &m_renderer))
		throw std::runtime_error(SDL_GetError());

	m_context = ImGui::CreateContext();
	m_io = &ImGui::GetIO();

	ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
	ImGui_ImplSDLRenderer3_Init(m_renderer);

	m_frameDelay = std::chrono::duration_cast<clk_t::duration>(std::chrono::duration<double>(1.0 / mode->refresh_rate));
	m_nextRefresh = clk_t::now() + m_frameDelay;
}

void Application::cleanup()
{
	if (m_renderer)
	{
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		SDL_DestroyRenderer(std::exchange(m_renderer, nullptr));
	}

	if (m_window)
		SDL_DestroyWindow(std::exchange(m_window, nullptr));
}

void Application::handle_events()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL3_ProcessEvent(&e);
		if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE))
			m_running = false;
	}
}

void Application::draw()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();

	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::Render();

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(m_renderer);
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
	SDL_RenderPresent(m_renderer);

	if (auto crr = clk_t::now(); crr < m_nextRefresh)
		std::this_thread::sleep_for(m_nextRefresh - crr);
	else if (crr > m_nextRefresh + m_frameDelay * 5)
		m_nextRefresh = crr;

	m_nextRefresh += m_frameDelay;
}

Application::Application()
{
	if (s_instances++ == 0)
		SDL_Init(SDL_INIT_VIDEO);
}

Application::~Application()
{
	cleanup();
	if (--s_instances == 0)
		SDL_Quit();
}

void Application::run()
{
	init();
	while (m_running)
	{
		handle_events();
		draw();
	}

	cleanup();
}
