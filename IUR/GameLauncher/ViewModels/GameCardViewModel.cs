using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameLauncher.Models;
using GameLauncher.Commands;
using GameLauncher.Services;
using System.Windows.Input;

namespace GameLauncher.ViewModels
{
    public class GameCardViewModel : ViewModelBase
    {
        // Game info
        private Game _game;
        private string _gameName;
        private string _genre;
        private int _price;
        private string _description;
        private string _releaseDate;
        private string _developer;
        private string _publisher;
        private string _gameImage;


        // References
        private LibraryViewModel _libraryRef;
        private EshopViewModel _storeRef;


        // User info
        private UserProfile _user;
        public UserProfile User 
        { 
            get
            {
                return _user;
            }
            set
            {
                _user = value;
                OnPropertyChanged(nameof(User));
            }
        }
       
        private int _balance;
        public int Balance
        {
            get { return _balance; }
            set
            {
                _balance = value;
                OnPropertyChanged(nameof(Balance));
            }
        }


        // Install/Uninstall button property
        private bool _isInstalled;
        public bool IsInstalled
        {
            get { return _isInstalled; }
            set
            {
                _isInstalled = value;
                _game.InstallGame(value);
                InstallButtonContent = value ? "Uninstall" : "Install";
                OnPropertyChanged(nameof(IsInstalled));
            }
        }

        private string _installButtonContent;
        public string InstallButtonContent
        {
            get { return _installButtonContent; }
            set { _installButtonContent = value; OnPropertyChanged(nameof(InstallButtonContent)); }
        }


        // Navigation commands
        public ICommand ToGamePageCommand { get; }
        public ICommand GameToShop { get;  }

        // Other commands
        public ICommand InstallCommand { get; }

        private RemoveGameCommand _removeGameCommand;
        public RemoveGameCommand RemoveGameCommand
        {
            get { return _removeGameCommand ?? (_removeGameCommand = new RemoveGameCommand(this, _libraryRef)); }
        }

        private BuyGameCommand _buyGameCommand;
        public BuyGameCommand BuyGameCommand
        {
            get { return _buyGameCommand ?? (_buyGameCommand = new BuyGameCommand(this, User)); }
        }
        public ICommand CloseCommand { get; }
        public ICommand MinimizeCommand { get; }


        // Constructor for Library Game Card
        public GameCardViewModel(Game game, LibraryViewModel libraryViewModelRef, UserProfile user)
        {
            _game = game;
            GameName = game.GameName;
            Genre = game.GameGenre;
            Price = game.GamePrice;
            Description = game.GameDescription;
            ReleaseDate = game.GameReleaseDate;
            Developer = game.GameDeveloper;
            Publisher = game.GamePublisher;
            GameImage = game.GameImage;

            _libraryRef = libraryViewModelRef;
            User = user;
            Balance = user.Money;
            IsInstalled = game.GameInstalled;
            InstallButtonContent = IsInstalled ? "Uninstall" : "Install";

            ToGamePageCommand = new SelectGameCommand(this);
            InstallCommand = new InstallCommand(this);
        }

        // Constructor for Store Game Card
        public GameCardViewModel(Game game, EshopViewModel eshopViewModelRef, UserProfile user)
        {
            GameName = game.GameName;
            Genre = game.GameGenre;
            Price = game.GamePrice;
            Description = game.GameDescription;
            ReleaseDate = game.GameReleaseDate;
            Developer = game.GameDeveloper;
            Publisher = game.GamePublisher;
            GameImage = game.GameImage;

            _storeRef = eshopViewModelRef;
            User = user;
            Balance = user.Money;

            ToGamePageCommand = new SelectGameCommand(this);
        }

        // Constructor for Game Page
        public GameCardViewModel(NavigationService navigationCommand1, NavigationService navigationCommand2, UserProfile user)
        {
            Game game = user.SelectedGame;
            GameName = game.GameName;
            Genre = game.GameGenre;
            Price = game.GamePrice;
            Description = game.GameDescription;
            ReleaseDate = game.GameReleaseDate;
            Developer = game.GameDeveloper;
            Publisher = game.GamePublisher;
            GameImage = game.GameImage;
            User = user;
            Balance= user.Money;

            GameToShop = new NavigateCommand(navigationCommand1);
            CloseCommand = new NavigateCommand(navigationCommand2);
            MinimizeCommand = new MinimizeCommand(navigationCommand1);
            
        }


        // set selected game value to user
        public void selectGame()
        {
            User.SelectedGame = this.GameCardToGame();
        }

        // Convert game card to game object
        public Game GameCardToGame()
        {
            return new Game(this.GameName, this.Genre, this.Price, this.Description, this.ReleaseDate, this.Publisher, this.Developer, this.GameImage);
        }

        public string GameName
        {
            get => _gameName;
            set => SetProperty(ref _gameName, value);
        }
        public string Genre
        {
            get => _genre;
            set => SetProperty(ref _genre, value);
        }
        public int Price
        {
            get => _price;
            set => SetProperty(ref _price, value);
        }
        public string Description
        {
            get => _description;
            set => SetProperty(ref _description, value);
        }
        public string ReleaseDate
        {
            get => _releaseDate;
            set => SetProperty(ref _releaseDate, value);
        }

        public string Publisher
        {
            get => _publisher;
            set => SetProperty(ref _publisher, value);
        }

        public string Developer
        {
            get => _developer;
            set => SetProperty(ref _developer, value);
        }

        public string GameImage
        {
            get => _gameImage;
            set => SetProperty(ref _gameImage, value);
        }

    }
}
