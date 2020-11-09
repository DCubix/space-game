#ifndef ENGINE_H
#define ENGINE_H

#include "sdl2.h"
#include <cstdint>
#include <string>
#include <vector>
#include <initializer_list>

using u8	= uint8_t;
using u16	= uint16_t;
using u32	= uint32_t;
using u64	= uint64_t;
using i8	= int8_t;
using i16	= int16_t;
using i32	= int32_t;
using i64	= int64_t;
using str	= std::string;

constexpr u32 TileWidth = 8;
constexpr u32 TileHeight = 8;
constexpr u32 SpriteSheetColumnCount = 32;

using AnimationID = uint32_t;

namespace util {
	static u32 spi(u32 x, u32 y) {
		return x + y * SpriteSheetColumnCount;
	}
}

enum ErrorCode {
	ERR_NoError = 0,
	ERR_InitializationFailed,
	ERR_WindowCreation,
	ERR_RendererCreation
};

enum class AnimationMode {
	OneShot = 0,
	Loop,
	PingPong
};

class GameWindow {
public:
	GameWindow() = default;
	~GameWindow() = default;

	ErrorCode create(const str& title, u32 width, u32 height, u32 pixelSize = 2);

	virtual void onCreate() = 0;
	virtual void onUpdate(float delta) = 0;
	virtual void onDestroy() = 0;

	const str& title() const { return m_title; }
	void title(const str& title);

	const u32& width() const { return m_width; }
	const u32& height() const { return m_height; }
	const u32& pixelSize() const { return m_pixelSize; }

	void sprite(u32 index, i32 x, i32 y);
	u32 animation(AnimationID id, u32 fps = 15, AnimationMode mode = AnimationMode::Loop);
	void patch(u32 ix, u32 iy, u32 iw, u32 ih, i32 x, i32 y);

	void loadSpriteSheet(const str& fileName);
	AnimationID createAnimation(const std::initializer_list<u32>& frames);
	void restartAnimation(AnimationID id);

private:
	str m_title{ "Game Window" };
	u32 m_width{ 0 }, m_height{ 0 }, m_sheetWidth{ 0 }, m_pixelSize{ 2 };

	struct Sprite {
		SDL_Rect src, dest;
	};

	struct Animation {
		u32 fps{ 15 }, index{ 0 };
		float time{ 0.0f };
		AnimationMode mode;
		bool reverse{ false };
		std::vector<u32> frames;
	};

	// Internals
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture *m_screen, *m_spriteSheet{ nullptr };

	std::vector<Sprite> m_sprites{};
	std::vector<Animation> m_animations{};

	ErrorCode loop();
};

#endif // ENGINE_H
