#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include "Globals.h"
#include "CellType.h"
#include "Pacman.h"

sf::Vector2i PacmanInitialLocation;

void DrawMap(sf::RenderWindow* DrawWindow, std::vector<std::vector<CellType>> MapCellData)
{
	/*
	sf::Texture WallTexture;
	sf::Sprite WallSprite{};
	WallTexture.loadFromFile("Assets/Zero.png");
	WallTexture.setSmooth(true);
	*/
	sf::RectangleShape WallShape{ sf::Vector2f{CellSize,CellSize} };
	WallShape.setFillColor(sf::Color::Blue);

	sf::CircleShape FoodShape{ CellSize / 10 };
	FoodShape.setFillColor(sf::Color::White);

	for (int i = 0; i < MapCellData.size(); i++)
	{
		for (int j = 0; j < MapCellData[i].size(); j++)
		{
			WallShape.setPosition(j * CellSize, i * CellSize);
			FoodShape.setPosition(j * CellSize + CellSize/2 - CellSize / 10, i * CellSize + CellSize / 2 - CellSize / 10);

			switch (MapCellData[i][j])
			{
			case CellType::Wall:
			{
				DrawWindow->draw(WallShape);
				break;
			}
			case CellType::Food:
			{
				DrawWindow->draw(FoodShape);
			}
			default:
				break;
			}
		}
	}
}

std::vector<std::vector<CellType>> ComputeMapCellData(std::vector<std::string> InputTextMap) {
	std::vector<std::vector<CellType>> MapCellData;

	// Allocate
	MapCellData.resize(InputTextMap.size());  // resize top level vector
	for (int i = 0; i < MapCellData.size(); i++)
	{
		MapCellData[i].resize(InputTextMap[i].size());  // resize each of the contained vectors
	}

	// Compute
	for (int i = 0; i < MapCellData.size(); i++)
	{
		for (int j = 0; j < MapCellData[i].size(); j++)
		{
			MapCellData[j][i] = CellType::Path;

			if (InputTextMap[j][i]=='=')
			{
				MapCellData[j][i] = CellType::Wall;
			}
			else if (InputTextMap[j][i] == '.')
			{
				MapCellData[j][i] = CellType::Food;
			}
			else if (InputTextMap[j][i] == 'P')
			{
				MapCellData[j][i] = CellType::PacMan;
				PacmanInitialLocation.x = i ;
				PacmanInitialLocation.y = j ;
			}

		}
	}
	return MapCellData;
}

std::vector<std::string> ReadMapFromFile(std::string FileName = "Map.txt")
{
	std::vector<std::string> InputTextMap;

	std::ifstream In(FileName);
	// Check if object is valid
	if (!In)
	{
		std::cerr << "Cannot open the File Map.txt";
		return InputTextMap;
	}
	std::string str;
	// Ignore first three lines tracker
	int IgnoreMapCommentsTracker = 3;
	// Read the next line from File untill it reaches the end.
	while (std::getline(In, str))
	{
		IgnoreMapCommentsTracker -= 1;
		if (IgnoreMapCommentsTracker < 1)
		{
			// Line contains string of length > 0 then save it in vector
			if (str.size() > 0)
				InputTextMap.push_back(str);
		}
	}
	//Close The File
	In.close();

	for (int i = 0; i < InputTextMap.size(); i++)
	{
		std::cout << "\n" << InputTextMap[i];
	}
};


int main()
{
	
	// Read map file into 
	std::vector<std::string> InputTextMap = ReadMapFromFile();
	// Fill cell data with map file
	std::vector<std::vector<CellType>> MapCellData = ComputeMapCellData(InputTextMap);

	int MapWidth = 0;
	for (int i = 0; i < MapCellData.size(); i++)
	{
		if (MapCellData[i].size() > MapWidth) {
			MapWidth = MapCellData[i].size();
		}
	}

	// Initialize window
	sf::RenderWindow MainWindow(sf::VideoMode{MapWidth * CellSize,static_cast<int>(MapCellData.size())*CellSize}, "Pacman", sf::Style::Default);

	sf::Text WinText;
	sf::Font Font;
	Font.loadFromFile("Assets/Marlboro.ttf");
	WinText.setFont(Font);
	WinText.setString("Victory");
	WinText.setCharacterSize(64);
	WinText.setFillColor(sf::Color::Red);
	WinText.setStyle(sf::Text::Bold);
	WinText.setPosition((MapWidth / 2) * CellSize -32, (MapWidth / 2) * CellSize -32);

	// Pacman
	Pacman Pacman;
	Pacman.SetPosition(sf::Vector2i{static_cast<int>( PacmanInitialLocation.x) *32,static_cast<int>(PacmanInitialLocation.y)*32});

	// Time trackers
	unsigned FrameTimeCounter = 0;
	std::chrono::time_point<std::chrono::steady_clock> LastFrameTime = std::chrono::steady_clock::now();

	bool bWon = false;

	while (MainWindow.isOpen())
	{
		if (bWon)
		{
			sf::sleep(sf::seconds(2));
			MainWindow.close();
		}

		unsigned DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - LastFrameTime).count();
		FrameTimeCounter += DeltaTime;
		LastFrameTime += std::chrono::microseconds(DeltaTime);

		// Runs the loop at 60 fps
		while (SingleFrameDelta <= FrameTimeCounter)
		{
			//std::cout << FrameTimeCounter << "\n";
			FrameTimeCounter -= SingleFrameDelta;

			// Handle events
			sf::Event Event;
			while (MainWindow.pollEvent(Event))
			{
				switch (Event.type)
				{
				case sf::Event::Closed:
				{
					MainWindow.close();
					break;
				}
				case sf::Event::KeyPressed:
				{
					if (Event.key.code == sf::Keyboard::Escape)
					{
						MainWindow.close();
					}
					break;
				}
				case sf::Event::Resized:
				{

					break;
				}
				default:
					break;
				}
			}
			Pacman.Tick(MapCellData,MapWidth);
			MainWindow.clear(sf::Color::Color(52, 76, 76, 1));
			//Draw
			if (Pacman.IsFoodFinished(MapCellData))
			{
				MainWindow.draw(WinText);
				bWon = true;
			}
			else
			{
				DrawMap(&MainWindow, MapCellData);
				Pacman.DrawPac(false, MainWindow);
			}
			//Draw End
			MainWindow.display();

		}

	}
	return 0;
}