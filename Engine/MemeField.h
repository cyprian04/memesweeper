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
		void Flagged();
		bool isFlagged() const;
		void Draw(const Vei2& screenPos, bool GameOver, Graphics& gfx );
		void SetNeighborsMemeCount(int nMemes);
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborsMemes = -1;
	};
public:
	Memefield(int nMemes);
	void Draw(Graphics& gfx);
	RectI GetRect() const;
	void onRevealClick(const Vei2& screenPos);
	void onFlagClick(const Vei2& screenPos);
	int GetDestroyedMemes() const;
private:
	Tile& TileAt(const Vei2 GridPos);
	const Tile& TileAt(const Vei2& GridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	int CountNeighborsMeme(const Vei2 GridPos);
private:
	bool isGameOver = false;
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile field[height * width];
	int xV;
	int yV;
	Vei2 TopLeft;
	int DestroyedMemes = 0;
};
