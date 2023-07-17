using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameLauncher.Models
{
    public class Data
    {
        private IDictionary<string, UserProfile> _users;
        public string CurrUser;

        private List<Game> _games;
        public List<Game> Games { get { return _games; } }

        
        // Constructor
        public Data()
        {
            _users = new Dictionary<string, UserProfile>();
            AddUsers();
            _games = new List<Game>();
            AddGames();
            CurrUser = "";
        }

        // Add user profiles in app data
        private void AddUsers()
        {
           
            UserProfile profile1 = new UserProfile("Nika", "Hi everyone!", 500, "shirover", "12345", "/Images/Avatars/NierAutomata.png");
            _users.Add("shirover12345", profile1);

            UserProfile profile2 = new UserProfile("Kirill", "Hello!", 100, "babaechi", "56789", "/Images/Avatars/HendryRoesly.png");
            _users.Add("babaechi56789", profile2);

            UserProfile profile3 = new UserProfile("Fedor", "I'm tired...", 200, "timusfed", "45678", "/Images/Avatars/DiVa.png");
            _users.Add("timusfed45678", profile3);

            UserProfile profile4 = new UserProfile("Dasha", "Who want to play? :)", 150, "antondar", "11111", "/Images/Avatars/Girl1.png");
            _users.Add("antondar11111", profile4);

            List<UserProfile> friends1 = new List<UserProfile>() { profile2, profile3, profile4 };
            profile1.AddFriends(friends1);
            List<UserProfile> friends2 = new List<UserProfile>() { profile1, profile3, profile4 };
            profile2.AddFriends(friends2);
            List<UserProfile> friends3 = new List<UserProfile>() { profile1, profile2, profile4 };
            profile3.AddFriends(friends3);
            List<UserProfile> friends4 = new List<UserProfile>() { profile1, profile2, profile3 };
            profile4.AddFriends(friends4);

        }

        // Add games in app data
        private void AddGames()
        {
            _games.Add(new Game("The Witcher",
                                "RPG",
                                10,
                                "Become The Witcher, Geralt of Rivia, a legendary monster slayer caught in a web of intrigue woven by forces vying for control of the world. Make difficult decisions and live with the consequences in a game that will immerse you in an extraordinary tale like no other.",
                                "16.09.2008",
                                "CD PROJECT RED",
                                "CD PROJECT RED",
                                "/Images/GameCards/Witcher.png"));

            _games.Add(new Game("The Witcher 2",
                                "RPG",
                                20,
                                "A time of untold chaos has come. Mighty forces clash behind the scenes in a struggle for power and influence. The Northern Kingdoms mobilize for war. But armies on the march are not enough to stop a bloody conspiracy...",
                                "17.05.2011",
                                "CD PROJECT RED",
                                "CD PROJECT RED",
                                "/Images/GameCards/Witcher2.png"));

            _games.Add(new Game("The Witcher 3",
                                "RPG",
                                20,
                                "You are Geralt of Rivia, mercenary monster slayer. Before you stands a war-torn, monster-infested continent you can explore at will. Your current contract? Tracking down Ciri — the Child of Prophecy, a living weapon that can alter the shape of the world.",
                                "18.05.2015",
                                "CD PROJECT RED",
                                "CD PROJECT RED",
                                "/Images/GameCards/Witcher3.png"));

            _games.Add(new Game("Dark Souls",
                                "RPG",
                                40,
                                "Dark Souls will be the most deeply challenging game you play this year. Can you live through a million deaths and earn your legacy?",
                                "24.08.2012",
                                "Bandai Namco Entertainment",
                                "FromSoftware",
                                "/Images/GameCards/DS1.png"));

            _games.Add(new Game("Dark Souls II",
                                "RPG, Souls-like",
                                40,
                                "Developed by FROM SOFTWARE, DARK SOULS II is the highly anticipated sequel to the gruelling 2011 breakout hit Dark Souls. The unique old-school action RPG experience captivated imaginations of gamers worldwide with incredible challenge and intense emotional reward.",
                                "25.04.2014",
                                "Bandai Namco Entertainment",
                                "FromSoftware",
                                "/Images/GameCards/DS2.png"));

            _games.Add(new Game("Dark Souls III",
                                "RPG",
                                60,
                                "Dark Souls continues to push the boundaries with the latest, ambitious chapter in the critically-acclaimed and genre-defining series. Prepare yourself and Embrace The Darkness!",
                                "11.04.2016",
                                "Bandai Namco Entertainment",
                                "FromSoftware",
                                "/Images/GameCards/DS3.png"));

            _games.Add(new Game("Metro 2033 Redux",
                                "Shooter",
                                20,
                                "In 2013 the world was devastated by an apocalyptic event, annihilating almost all mankind and turning the Earth's surface into a poisonous wasteland. A handful of survivors took refuge in the depths of the Moscow underground, and human civilization entered a new Dark Age. The year is 2033.",
                                "28.08.2014",
                                "Deep Silver",
                                "4A Games",
                                "/Images/GameCards/Metro2033.png"));

            _games.Add(new Game("Metro: Last Light Redux",
                                "Shooter",
                                20,
                                "It is the year 2034. Beneath the ruins of post-apocalyptic Moscow, in the tunnels of the Metro, the remnants of mankind are besieged by deadly threats from outside – and within. Mutants stalk the catacombs beneath the desolate surface, and hunt amidst the poisoned skies above.",
                                "28.08.2014",
                                "Deep Silver",
                                "4A Games",
                                "/Images/GameCards/MetroLastLight.png"));

            _games.Add(new Game("Metro Exodus",
                                "Shooter",
                                30,
                                "Flee the shattered ruins of the Moscow Metro and embark on an epic, continent-spanning journey across the post-apocalyptic Russian wilderness. Explore vast, non-linear levels, lose yourself in an immersive, sandbox survival experience, and follow a thrilling story-line that spans an entire year in the...",
                                "14.02.2019",
                                "Deep Silver",
                                "4A Games",
                                "/Images/GameCards/MetroExodus.png"));

        }

        // Check if login and password are correct
        public bool CheckData(string login, string password)
        {
            string key = login + password;
            if (_users.ContainsKey(key))
            {
                CurrUser = key;
                return true;
            }
            return false;
        }

        // return reference on current user
        public UserProfile getCurrentUser()
        {
            return _users[CurrUser];
        }

    }
}
