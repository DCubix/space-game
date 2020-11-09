#include "engine.h"

static double time() {
	return double(SDL_GetTicks()) / 1000.0;
}

ErrorCode GameWindow::create(const str& title, u32 width, u32 height, u32 pixelSize) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return ERR_InitializationFailed;
	}

	m_window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width * pixelSize, height * pixelSize,
		SDL_WINDOW_SHOWN
	);
	if (m_window == nullptr) {
		SDL_Quit();
		return ERR_WindowCreation;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (m_renderer == nullptr) {
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return ERR_RendererCreation;
	}

	m_screen = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);

	m_width = width;
	m_height = height;
	m_pixelSize = pixelSize;
	m_title = title;

	m_sprites.reserve(10000);

	return loop();
}

void GameWindow::title(const str& title) {
	m_title = title;
	SDL_SetWindowTitle(m_window, title.c_str());
}

void GameWindow::sprite(u32 index, i32 x, i32 y) {
	if (!m_spriteSheet) return;
	int tx = (index % SpriteSheetColumnCount) * TileWidth;
	int ty = ::floor(index / SpriteSheetColumnCount) * TileHeight;

	Sprite spr;
	spr.src = { tx, ty, TileWidth, TileHeight };
	spr.dest = { x, y, TileWidth, TileHeight };

	m_sprites.push_back(spr);
}

void GameWindow::patch(u32 ix, u32 iy, u32 iw, u32 ih, i32 x, i32 y) {
	if (!m_spriteSheet) return;

	const u32 index = util::spi(ix, iy);
	int tx = (index % SpriteSheetColumnCount) * TileWidth;
	int ty = ::floor(index / SpriteSheetColumnCount) * TileHeight;

	Sprite spr;
	spr.src = { tx, ty, int(TileWidth * iw), int(TileHeight * ih) };
	spr.dest = { x, y, int(TileWidth * iw), int(TileHeight * ih) };

	m_sprites.push_back(spr);
}

ErrorCode GameWindow::loop() {
	bool running = true;
	const double timeStep = 1.0 / 60.0;
	double lastTime = time();
	double accum = 0.0;

	onCreate();

	SDL_Event evt;
	while (running) {
		bool canRender = false;
		double currentTime = time();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
		accum += delta;

		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) running = false;
		}

		while (accum >= timeStep) {
			// update animations
			for (auto& anim : m_animations) {
				if (anim.time >= 1.0f / anim.fps) {
					anim.time = 0.0f;
					if (anim.mode != AnimationMode::PingPong) {
						if (anim.index++ >= anim.frames.size()-1) {
							if (anim.mode == AnimationMode::Loop) anim.index = 0;
							else anim.index = anim.frames.size() - 1;
						}
					} else {
						if (!anim.reverse) {
							if (anim.index++ >= anim.frames.size()-1) {
								anim.reverse = true;
							}
						} else {
							if (anim.index-- <= 0) {
								anim.reverse = false;
							}
						}
					}
				}
				anim.time += float(timeStep);
			}

			onUpdate(float(timeStep));
			accum -= timeStep;
			canRender = true;
		}

		if (canRender) {
			SDL_SetRenderTarget(m_renderer, m_screen);
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
			SDL_RenderClear(m_renderer);

			if (m_spriteSheet) {
				for (auto& spr : m_sprites) {
					SDL_RenderCopy(m_renderer, m_spriteSheet, &spr.src, &spr.dest);
				}
			}
			m_sprites.clear();

			SDL_SetRenderTarget(m_renderer, nullptr);

			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
			SDL_RenderClear(m_renderer);

			SDL_Rect rec = { 0, 0, int(m_width * m_pixelSize), int(m_height * m_pixelSize) };
			SDL_RenderCopy(m_renderer, m_screen, nullptr, &rec);
			SDL_RenderPresent(m_renderer);
		}
	}

	onDestroy();

	if (m_spriteSheet) {
		SDL_DestroyTexture(m_spriteSheet);
	}
	SDL_DestroyTexture(m_screen);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

	return ERR_NoError;
}

void GameWindow::loadSpriteSheet(const str& fileName) {
	if (m_spriteSheet) {
		SDL_DestroyTexture(m_spriteSheet);
	}

	SDL_Surface* surf = SDL_LoadBMP(fileName.c_str());
	SDL_SetColorKey(surf, 1, SDL_MapRGB(surf->format, 255, 0, 255));
	m_spriteSheet = SDL_CreateTextureFromSurface(m_renderer, surf);
	SDL_FreeSurface(surf);
}

AnimationID GameWindow::createAnimation(const std::initializer_list<u32>& frames) {
	Animation anim{};
	anim.mode = AnimationMode::Loop;
	anim.frames = std::vector<u32>(frames);
	m_animations.push_back(anim);
	return m_animations.size()-1;
}

u32 GameWindow::animation(AnimationID id, u32 fps, AnimationMode mode) {
	Animation& anim = m_animations[id];
	anim.fps = fps;
	anim.mode = mode;
	return anim.frames[anim.index];
}

void GameWindow::restartAnimation(AnimationID id) {
	m_animations[id].index = 0;
	m_animations[id].time = 0.0f;
	m_animations[id].reverse = false;
}