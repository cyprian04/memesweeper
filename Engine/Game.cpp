/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	field(gfx.GetRect().GetCenter(),10)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (field.GetGameIs() == Memefield::GameState::played)
		{
			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				const Vei2 MousePos = e.GetPos();
				if (field.GetRect().Contains(MousePos))
				{
					field.onRevealClick(MousePos);
				}
			}
			else if (e.GetType() == Mouse::Event::Type::RPress)
			{
				const Vei2 MousePos = e.GetPos();
				if (field.GetRect().Contains(MousePos))
				{
					field.onFlagClick(MousePos);
				}
			}
		}
	}
}

void Game::ComposeFrame()
{
	field.Draw(gfx);
	if (field.GetGameIs() == Memefield::GameState::Win)
	{
		SpriteCodex::DrawWin(Vei2(gfx.GetRect().GetCenter()), gfx);
	}
}
