using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameLauncher.Models
{
    public class Game
    {
        public string GameName { get; }
        public string GameGenre { get; }
        public int GamePrice { get; }
        public string GameDescription { get; }
        public string GameReleaseDate { get; }
        public string GamePublisher { get; }
        public string GameDeveloper { get; }

        public string GameImage { get; }
        public bool GameInstalled { get; set; }

        public Game(string name, string genre, int price, string description, string releaseDate, string gamePublisher, string gameDeveloper, string gameImage)
        {
            GameName = name;
            GameGenre = genre;
            GamePrice = price;
            GameDescription = description;
            GameReleaseDate = releaseDate;
            GamePublisher = gamePublisher;
            GameDeveloper = gameDeveloper;
            GameImage = gameImage;
            GameInstalled = false;
        }

        // change install/uninstall game status
        public void InstallGame(bool value)
        {
            GameInstalled = value;
        }
    }
}
