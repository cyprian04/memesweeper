#pragma once
#include "Graphics.h"
#include "RectI.h"
#include "Vei2.h"
#include "Sound.h"
#include <random>
class Memefield
{
public:
	enum class GameState
	{
		Lost,
		Win,
		played
	};
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
		void Draw(const Vei2& screenPos, GameState GameIs, Graphics& gfx ) const ;
		bool hasNoNeighbors() const;
		void SetNeighborsMemeCount(int nMemes);
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborsMemes = -1;
	};
public:
	Memefield(const Vei2 center, int nMemes);
	void Draw(Graphics& gfx);
	RectI GetRect() const;
	void onRevealClick(const Vei2& screenPos);
	void onFlagClick(const Vei2& screenPos);
	GameState GetGameIs() const;
private:
	void RevealTiles(const Vei2& GridPos);
	Tile& TileAt(const Vei2 GridPos);
	const Tile& TileAt(const Vei2& GridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	int CountNeighborsMeme(const Vei2 GridPos);
	bool GameIsWon() const;
private:
	GameState GameIs = GameState::played;
	static constexpr int width = 25;
	static constexpr int height = 25;
	Sound snd = Sound(L"spayed.wav");
	Tile field[height * width];
	Vei2 TopLeft;
};
