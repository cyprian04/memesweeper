#include "MemeField.h"
#include "SpriteCodex.h"
#include <assert.h>
void Memefield::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;
}

bool Memefield::Tile::HasMeme() const
{
	return hasMeme;
}

void Memefield::Tile::Reveal()
{
	assert(state == State::Hidden);
	state = State::Revealed;
}

bool Memefield::Tile::isRevealed() const
{
	return state == State::Revealed;
}

void Memefield::Tile::Flagged()
{
	assert(!isRevealed());
	if (state == State::Hidden)
	{
		state = State::Flagged;
	}
	else
	{
		state = State::Hidden;
	}
}

bool Memefield::Tile::isFlagged() const
{
	return state == State::Flagged;
}

void Memefield::Tile::Draw(const Vei2& screenPos, GameState GameIs, Graphics& gfx) const
{
	if (GameIs != GameState::Lost)
	{
		switch (state)
		{
		case State::Hidden:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(screenPos, nNeighborsMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			break;
		}
	}
	else
	{
		switch (state)
		{
		case State::Hidden:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);

			}
			SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::Flagged:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(screenPos, nNeighborsMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
			break;
		}
	}
}

void Memefield::Tile::SetNeighborsMemeCount(int nMemes)
{
	assert(nNeighborsMemes == -1);
	nNeighborsMemes = nMemes;
}


Memefield::Memefield(const Vei2 center, int nMemes)
	:
	TopLeft(center - Vei2(width * SpriteCodex::tileSize, height* SpriteCodex::tileSize) / 2)
{
	assert(nMemes > 0 && nMemes < width* height);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xPos(0, width - 1);
	std::uniform_int_distribution<int> yPos(0, width - 1);

	for (int Nspawn = 0; Nspawn < nMemes; Nspawn++)
	{
		Vei2 SpawnPos;
		do
		{
			SpawnPos = { xPos(rng), yPos(rng) };

		} while (TileAt(SpawnPos).HasMeme());

		TileAt(SpawnPos).SpawnMeme();
	}

	for (Vei2 GridPos = { 0,0 }; GridPos.y < height; GridPos.y++)
	{
		for (GridPos.x = 0; GridPos.x < width; GridPos.x++)
		{
			TileAt(GridPos).SetNeighborsMemeCount(CountNeighborsMeme(GridPos));
		}
	}
}

void Memefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);

	for (Vei2 GridPos = { 0,0}; GridPos.y < height; GridPos.y++)
	{
		for (GridPos.x = 0; GridPos.x < width; GridPos.x++)
		{
			TileAt(GridPos).Draw(TopLeft + GridPos* SpriteCodex::tileSize, GameIs, gfx);
		}
	}
}

RectI Memefield::GetRect() const 
{
	return RectI(TopLeft, width* SpriteCodex::tileSize, height * SpriteCodex::tileSize);
}

void Memefield::onRevealClick(const Vei2& screenPos)
{
	if (GameIs == GameState::played)
	{
		const Vei2 gridPos = ScreenToGrid(screenPos);
		assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y >= 0 && gridPos.y < height);
		Tile& tile = TileAt(gridPos);
		if (!tile.isRevealed() && !tile.isFlagged())
		{
			tile.Reveal();
			if (tile.HasMeme())
			{
				GameIs = GameState::Lost;
				snd.Play();
			}
			else if(GameIsWon())
			{
				GameIs = GameState::Win;
			}
		}
	}
}

void Memefield::onFlagClick(const Vei2& screenPos)
{
	if (GameIs == GameState::played)
	{
		const Vei2 gridPos = ScreenToGrid(screenPos);
		assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y >= 0 && gridPos.y < height);
		Tile& tile = TileAt(gridPos);
		if (!tile.isRevealed())
		{
			tile.Flagged();
		}
	}
}

Memefield::GameState Memefield::GetGameIs() const
{
	return GameIs;
}

Memefield::Tile& Memefield::TileAt(const Vei2 GridPos)
{
	return field[GridPos.x + GridPos.y * width];
}

const Memefield::Tile& Memefield::TileAt(const Vei2& GridPos) const
{
	return field[GridPos.x + GridPos.y * width];
}

Vei2 Memefield::ScreenToGrid(const Vei2& screenPos)
{
	return (screenPos - TopLeft) / SpriteCodex::tileSize;
}

int Memefield::CountNeighborsMeme(const Vei2 GridPos)
{
	const int xStart = std::max(0, GridPos.x - 1);
	const int yStart = std::max(0, GridPos.y - 1);
	const int xEnd = std::min(width - 1, GridPos.x + 1);
	const int yEnd = std::min(height - 1, GridPos.y + 1);

	int count = 0;
	for (Vei2 gridPos= {xStart, yStart}; gridPos.y <= yEnd; gridPos.y++)
	{
		for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
		{
			if (TileAt(gridPos).HasMeme())
			{
				count++;
			}
		}
	}
	return count;
}

bool Memefield::GameIsWon() const
{
	for (const Tile& t: field)
	{
		if (!t.isRevealed() && !t.HasMeme())
		{
			return false;
		}
	}
	return true;
}
