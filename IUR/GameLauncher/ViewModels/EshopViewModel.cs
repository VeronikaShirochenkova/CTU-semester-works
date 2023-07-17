using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using GameLauncher.Commands;
using GameLauncher.Models;
using GameLauncher.Services;
using GameLauncher.Stores;

namespace GameLauncher.ViewModels
{
    public class EshopViewModel : ViewModelBase
    {
        // Game collections
        public ObservableCollection<GameCardViewModel> EShopGames { get; } = new ObservableCollection<GameCardViewModel>();
        private List<GameCardViewModel> _Games;


        // User reference
        private UserProfile _user;
        public UserProfile User { get { return _user; } }


        // Filter for search bar
        private string _filter;
        public string Filter
        {
            get { return _filter; }
            set 
            { 
                _filter = value;
                UpdateGameList();
                OnPropertyChanged(nameof(Filter)); 
            }
        }


        // User selected game reference
        public Game selectedShopGame => _user.SelectedGame;
        private void OnSelectedGameChanged()
        {
            OnPropertyChanged(nameof(selectedShopGame));
            StoreToGameCommand.Execute(selectedShopGame);
        }


        // Navigation commands
        public ICommand StoreToProfileCommand { get; }
        public ICommand StoreToLibraryCommand { get; }
        public ICommand StoreToGameCommand { get; }
        public ICommand LogOutCommand { get; }

        // Other commands
        public ICommand CloseCommand { get; }
        public ICommand MinimizeCommand { get; }


        public EshopViewModel(  NavigationService NavigationCommand1, 
                                NavigationService NavigationCommand2, 
                                NavigationService NavigationCommand3,
                                NavigationService NavigationCommand4,
                                NavigationService NavigationCommand5,
                                Data data,
                                UserProfile user)
        {
            _user = user;
            _Games = new List<GameCardViewModel>();
            Filter = "";

            StoreToProfileCommand = new NavigateCommand(NavigationCommand1);
            StoreToLibraryCommand = new NavigateCommand(NavigationCommand2);
            StoreToGameCommand = new NavigateCommand(NavigationCommand3);
            LogOutCommand = new NavigateCommand(NavigationCommand4);

            CloseCommand = new NavigateCommand(NavigationCommand5);
            MinimizeCommand = new MinimizeCommand(NavigationCommand1);

            
            AddGamesToShop(data.Games);      
            UpdateGameList();

            _user.selectedGameChanged = OnSelectedGameChanged;   
        }

        // call user function to buy this game
        public void ShopBuyGame(GameCardViewModel game)
        {
            _user.UserBuyGame(game.GameCardToGame());
        }

        // update searching results
        private void UpdateGameList()
        {
            if (Filter.Length == 0 || Filter == null)
            {
                EShopGames.Clear();
                foreach(GameCardViewModel gameCard in _Games)
                {
                    EShopGames.Add(gameCard);
                }
            }
            else
            {
                EShopGames.Clear();
                foreach (GameCardViewModel gameCard in _Games)
                {
                    if (gameCard.GameName.Contains(Filter))
                    {
                        EShopGames.Add(gameCard);
                    }
                    
                }
            }

        }

        // add games to store collection
        private void AddGamesToShop(List<Game> games)
        {
            foreach(var g in games)
            {
                _Games.Add(new GameCardViewModel(g, this, _user));
            }
        }

    }
}
