#pragma once
#include<vector>
#include <SFML/Graphics.hpp>
#include "CellType.h"
#include "CollisionType.h"

class Pacman
{
	
public:
	Pacman();

	void Tick(std::vector<std::vector<CellType>>& MapCellData, int MapWidth);

	void DrawPac(bool bVictory, sf::RenderWindow& DrawWindow);

	CollisionType CheckFutureCollision(bool bIsEnergized, sf::Vector2i PacPositionToCheck, std::vector<std::vector<CellType>>& MapCellData, int MapWidth);

	bool IsFoodFinished(std::vector<std::vector<CellType>>& MapCellData);

protected:
	// Properties

	bool bFoodFinished = false;
	int PacmanSpeed = 2;
	bool bDead = false;
	char MovementDirection;
	bool bEnergized = false;
	sf::Vector2i Position{ 0,0 };

	int WIndex;
	int HIndex;

public:
	__forceinline void SetPosition(sf::Vector2i NewPosition) { Position = NewPosition;};
	__forceinline void SetDead(bool bIsDead) { bDead = bIsDead; };

	__forceinline bool GetDead() { return bDead; };
	__forceinline char GetMovementDirection() { return MovementDirection; };
	__forceinline sf::Vector2i GetPosition() { return Position;};
};

