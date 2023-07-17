using GameLauncher.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using GameLauncher.Commands;
using GameLauncher.Stores;
using GameLauncher.Services;
using System.Diagnostics;

namespace GameLauncher.ViewModels
{
    public class LibraryViewModel : ViewModelBase
    {
        // Library games collection
        public ObservableCollection<GameCardViewModel> LibraryGames { get; set; } = new ObservableCollection<GameCardViewModel>();

        // Navigation commands
        public ICommand LibToProfileCommand { get; }
        public ICommand LibToGameCommand { get; }
        public ICommand LibToStoreCommand { get; }
        public ICommand LogOutCommand { get; }

        // Other Commands
        public ICommand CloseCommand { get; }
        public ICommand MinimizeCommand { get; }

        // User reference
        private UserProfile _user;
        public UserProfile User { get { return _user; } }
        
        // User selected game reference
        public Game selectedShopGame => _user.SelectedGame;
        private void OnSelectedGameChanged()
        {
            OnPropertyChanged(nameof(selectedShopGame));
            LibToGameCommand.Execute(selectedShopGame);
        }


        // Constructor
        public LibraryViewModel(NavigationService NavigationCommand1, 
                                NavigationService NavigationCommand2,
                                NavigationService NavigationCommand3,
                                NavigationService NavigationCommand4,
                                NavigationService NavigationCommand5,
                                UserProfile user)
        {
            _user = user;

            LibraryGames = new ObservableCollection<GameCardViewModel>();
            addGames();

            LibToProfileCommand = new NavigateCommand(NavigationCommand1);
            LibToGameCommand = new NavigateCommand(NavigationCommand2);
            LibToStoreCommand = new NavigateCommand(NavigationCommand3);
            LogOutCommand = new NavigateCommand(NavigationCommand4);

            CloseCommand = new NavigateCommand(NavigationCommand5);
            MinimizeCommand = new MinimizeCommand(NavigationCommand1);

            _user.selectedGameChanged = OnSelectedGameChanged;

        }

        // remove game from user library
        public void removeGame(string name)
        {
             
            foreach (var game in _user.UserGames)
            {
                if (game.GameName == name)
                {
                    _user.UserGames.Remove(game);
                    break;
                }
            }
        }

        // add game to user library
        private void addGames()
        {
            foreach (var game in _user.UserGames)
            {
                LibraryGames.Add(new GameCardViewModel(game, this, _user));
            }
        }

    }
}
