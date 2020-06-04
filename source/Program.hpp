#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <memory>
#include "GameSettings.hpp"
#include "ConsoleGUI.hpp"
#include "Game.hpp"

class Program
{
private:
    std::unique_ptr<GUInterface> gui = std::make_unique<ConsoleGUI>();
    GameSettings settings{};
    bool exit{false};
public:
    explicit Program() = default;
    Program(const Program &) = delete;
    Program & operator=(const Program &) = delete;
	Program(Program &&) = delete;
	Program & operator=(Program &&) = delete;
	~Program() = default;

    void Run();
};

#endif // PROGRAM_HPP