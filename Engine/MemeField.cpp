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

void Memefield::Tile::Draw(const Vei2& screenPos, Graphics& gfx)
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
		if (!hasMeme)
		{
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		else
		{
			SpriteCodex::DrawTile0(screenPos, gfx);
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		break;
	}
}

Memefield::Memefield(int nMemes)
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
}

void Memefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 GridPos = {0,0}; GridPos.y < height; GridPos.y ++)
	{
		for (GridPos.x = 0; GridPos.x < height; GridPos.x++)
		{
			TileAt(GridPos).Draw(GridPos * SpriteCodex::tileSize, gfx);
		}
	}
}

RectI Memefield::GetRect() const
{
	return RectI(0,width * SpriteCodex::tileSize,0,height * SpriteCodex::tileSize);
}

void Memefield::onRevealClick(const Vei2& screenPos)
{
	const Vei2 gridPos = ScreenToGrid(screenPos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);
	Tile& tile = TileAt(gridPos);
	if (!tile.isRevealed())
	{
		tile.Reveal();
	}
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
	return screenPos / SpriteCodex::tileSize;
}
