#include "Program.hpp"


void Program::Run(){
    while(exit != true){
        MainMenuChoice choice = gui->MainMenu();
        switch(choice){
            case MainMenuChoice::start_game:
            {
                bool restart = true;
                while(restart){
                    std::unique_ptr<Game> game = std::make_unique<Game>(settings, *gui);
                    gui->SetGameBoardSize(settings.GetWidth(), settings.GetHeight());
                    restart = game->Play();
                }
                break;
            }
            case MainMenuChoice::settings:
            {
                settings = gui->SettingsMenu();
                break;
            }
            case MainMenuChoice::help:
            {
                gui->HelpMenu();
                break;
            }
            case MainMenuChoice::quit:
            {
                exit = true;
                break;
            }
        }
    }
}