#include "Pacman.h"
#include "Globals.h"
#include <iostream>

Pacman::Pacman()
{
	MovementDirection = '0';
}

void Pacman::Tick(std::vector<std::vector<CellType>>& MapCellData, int MapWidth)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (CheckFutureCollision(false, sf::Vector2i{ Position.x,Position.y - PacmanSpeed }, MapCellData, MapWidth) == CollisionType::WallCollision)
		{
			if (MovementDirection == 'W')
			{
				MovementDirection = '0';
			}
		}
		else
		{
			MovementDirection = 'W';
		}
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (CheckFutureCollision(false, sf::Vector2i{ Position.x,Position.y + PacmanSpeed }, MapCellData, MapWidth) == CollisionType::WallCollision)
		{
			if (MovementDirection == 'S')
			{
				MovementDirection = '0';
			}
		}
		else
		{
			MovementDirection = 'S';
		}
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (CheckFutureCollision(false, sf::Vector2i{ Position.x - PacmanSpeed,Position.y }, MapCellData, MapWidth) == CollisionType::WallCollision)
		{
			if (MovementDirection == 'A')
			{
				MovementDirection = '0';
			}
		}
		else
		{
			MovementDirection = 'A';
		}
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (CheckFutureCollision(false, sf::Vector2i{ Position.x + PacmanSpeed,Position.y }, MapCellData, MapWidth) == CollisionType::WallCollision)
		{
			if (MovementDirection == 'D')
			{
				MovementDirection = '0';
			}
		}
		else
		{
			MovementDirection = 'D';
		}
	}


	switch (MovementDirection)
	{
	case 'W':
	{
		if (!CheckFutureCollision(false, sf::Vector2i{ Position.x,Position.y - PacmanSpeed }, MapCellData, MapWidth) == CollisionType::WallCollision)
		{
			Position.y -= PacmanSpeed;
		}
		break;
	}
	case 'S':
	{
		if (!CheckFutureCollision(false, sf::Vector2i{ Position.x,Position.y + PacmanSpeed }, MapCellData, MapWidth) == CollisionType::WallCollision)
		{
			Position.y += PacmanSpeed;
		}
		break;
	}
	case 'A':
	{
		if (!CheckFutureCollision(false, sf::Vector2i{ Position.x - PacmanSpeed,Position.y }, MapCellData, MapWidth) == CollisionType::WallCollision)
		{
			Position.x -= PacmanSpeed;
		}
		break;
	}
	case 'D':
	{
		if (!CheckFutureCollision(false, sf::Vector2i{ Position.x + PacmanSpeed,Position.y }, MapCellData, MapWidth) == CollisionType::WallCollision)
		{
			Position.x += PacmanSpeed;
		}
		
		break;
	}
	case '0':
	{
		break;
	}
	default:
		break;
	}


	// Horizontal position wrap
	// TODO 32 is CellSize
	if (Position.x<= -32)
	{
		Position.x = 32 * MapWidth - PacmanSpeed;
	}
	//bool temp = 32 * MapWidth <= Position.x;
	else if (Position.x>=32*MapWidth)
	{
		Position.x = PacmanSpeed - 32;
	}

	// Vertical position clamp
	if (Position.y< 0)
	{
		Position.y = 32 * MapCellData.size();
	}
	else if (Position.y>= 32* MapCellData.size())
	{
		Position.y = 0;
	}


	 WIndex = Position.x / CellSize;
	 HIndex = Position.y / CellSize;

}

void Pacman::DrawPac(bool bVictory, sf::RenderWindow& DrawWindow)
{


	sf::CircleShape PacShape{ CellSize / 2 };
	PacShape.setPosition(sf::Vector2f{static_cast<float>(WIndex) * static_cast<float>(CellSize),static_cast<float> (HIndex)* static_cast<float> (CellSize)});
	if (bDead)
	{
		PacShape.setFillColor(sf::Color::Red);
		DrawWindow.draw(PacShape);
		return;
	}
	PacShape.setFillColor(sf::Color::Yellow);
	DrawWindow.draw(PacShape);
	return;
}

CollisionType Pacman::CheckFutureCollision(bool bIsEnergized, sf::Vector2i PacPositionToCheck, std::vector<std::vector<CellType>>& MapCellData, int MapWidth)
{
	// In the future, first check for enemy collision then continue the following code.

	if (PacPositionToCheck.x>=0 && PacPositionToCheck.x<=MapWidth*CellSize && PacPositionToCheck.y>=0 && PacPositionToCheck.y <= MapCellData.size() * CellSize)
	{
		//Getting the exact position.
		int FutureWidthCellIndex = PacPositionToCheck.x / CellSize;
		int FutureHeightCellIndex = PacPositionToCheck.y / CellSize;

		std::cout << FutureWidthCellIndex << "\t" << FutureHeightCellIndex << "\t\t" << Position.x / CellSize << "\t" << Position.y / CellSize << "\n";

		switch (MapCellData[FutureHeightCellIndex][FutureWidthCellIndex])
		{
		case CellType::Wall:
		{
			return CollisionType::WallCollision;
			break;
		}
		case CellType::Energy:
		{
			return CollisionType::EnergyCollision;
			break;
		}
		case CellType::Food:
		{
			MapCellData[FutureHeightCellIndex][FutureWidthCellIndex] = CellType::Path;
			return CollisionType::FoodCollision;
			break;
		}
		case CellType::Path:
		{
			return CollisionType::NoCollision;
			break;
		}
		default:
			return CollisionType::NoCollision;
			break;
		}
	}
	return NoCollision;
}


bool Pacman::IsFoodFinished(std::vector<std::vector<CellType>>& MapCellData)
{
	int FoodCount = 0;
	for (int i = 0; i < MapCellData.size(); i++)
	{
		for (int j = 0; j < MapCellData[i].size(); j++) {
			if (MapCellData[j][i]==CellType::Food)
			{
				FoodCount++;
			}
		}
	}
	if (FoodCount>0)
	{
		return false;
	}
	return true;
}
