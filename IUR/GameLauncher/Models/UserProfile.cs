using GameLauncher.ViewModels;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace GameLauncher.Models
{
    public class UserProfile
    {
        public ObservableCollection<Game> UserGames { get; set; }
        public ObservableCollection<UserProfile> UserFriends { get; set; }

        public string Nickname { get; set; }
        public string ProfileDescription { get; set; }
        public int Money { get; set; }
        public string Avatar { get; set; }

        private string _login;
        public string Login { get { return _login; }  }

        private string _password;
        public string Password { get { return _password; } }

        
        // info for opening game page
        private Game _selectedGame;
        public Game SelectedGame 
        { 
            get
            {
                return _selectedGame;
            }
            set
            {
                _selectedGame = value;
                if (value != null)
                {
                    OnSelectedGameChanged();
                }
                

            } 
        }
        public Action selectedGameChanged;
        public void OnSelectedGameChanged()
        {
            selectedGameChanged?.Invoke();
        }


        // Constructor
        public UserProfile(string nickname, string profileDescription, int money, string login, string password, string avatar)
        {
            Nickname = nickname;
            ProfileDescription = profileDescription;
            Money = money >= 1000 ? 999 : money;
            Avatar = avatar;

            _login = login;
            _password = password;
            UserGames = new ObservableCollection<Game>();
            UserFriends = new ObservableCollection<UserProfile>();
        }

        // add user friends
        public void AddFriends(List<UserProfile> friends)
        {
            foreach(UserProfile friend in friends)
            {
                UserFriends.Add(friend);
            }
        }

        // check if user has enough money to buy game
        public bool CheckWalletBalance(int gamePrice)
        {
            if (Money - gamePrice > 0)
            {
                return true;
            }
            return false;
        }

        // check if this game already in user library
        public bool GameInLibrary(Game game)
        {
            foreach(Game g in UserGames)
            {
                if (g.GameName == game.GameName)
                {
                    return true;
                }
            }
            return false;
        }

        // add this game in user library
        public void AddGameToLibrary(Game game)
        {
            Money = Money - game.GamePrice;
            UserGames.Add(game);
        }

        // process of buying game
        public bool UserBuyGame(Game game)
        {
            // no enough money
            if (!CheckWalletBalance(game.GamePrice)) { return false; }

            // check if game already in library
            if (!GameInLibrary(game))
            {
                AddGameToLibrary(game);
                return true;
            }
            return false;
        }

        // update user data
        public void RewriteData(UserProfile user)
        {
            Nickname = user.Nickname;
            ProfileDescription = user.ProfileDescription;
            Money = user.Money;

        }
    }
}
