#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <chrono>
#include <thread>
#include "GameSettings.hpp"
#include "GUInterface.hpp"
#include "Fruit.hpp"
#include "Playground.hpp"
#include "Snake.hpp"


class Game
{
private:
    Playground playground;
    Snake snake;
    Fruit fruit;
    int snakeSpeed;
    int updateTime;
    int gameScore{0};
    Input userInput{Input::none};
    GUInterface & gui;

    class Timer
    {
    private:
        std::chrono::time_point<std::chrono::steady_clock> start, end;
        std::chrono::duration<int, std::milli> duration;
    public:
        explicit Timer(): start{std::chrono::steady_clock::now()}, end{std::chrono::steady_clock::now()}, duration{0} {}
        Timer(const Timer &) = default;
        Timer & operator=(const Timer &)= default;
        Timer(Timer &&)= default;
        Timer & operator=(Timer &&)= default;
        ~Timer() = default;

        void Start(){ start = std::chrono::steady_clock::now(); }
        void Stop(){ end = std::chrono::steady_clock::now(); }
        int ElapsedTime()
        {
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            return duration.count();
        }
    };

    void EatFruit();
    bool IsGameOver(const Position & nextHeadPos) const;
    void SendDataToGui() const;
public:
    explicit Game() = delete;
    Game(GameSettings & settings, GUInterface & programGui);
    Game(const Game &) = delete;
    Game & operator=(const Game &) = delete;
	Game(Game &&) = delete;
	Game & operator=(Game &&) = delete;
	~Game() = default;

    bool Play();
};

#endif // GAME_HPP