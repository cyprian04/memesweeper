#pragma once
#include "Graphics.h"
#include "RectI.h"
#include "Vei2.h"
#include <random>
class Memefield
{
public:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Reveal();
		bool isRevealed() const;
		void Draw(const Vei2& screenPos, Graphics& gfx );
	private:
		State state = State::Hidden;
		bool hasMeme = false;
	};
public:
	Memefield(int nMemes);
	void Draw(Graphics& gfx);
	RectI GetRect() const;
	void onRevealClick(const Vei2& screenPos);
private:
	Tile& TileAt(const Vei2 GridPos);
	const Tile& TileAt(const Vei2& GridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile field[height * width];
};
