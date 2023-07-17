using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    public class ProfileViewModel : ViewModelBase
    {
        // User reference
        private UserProfile _user;
        public UserProfile User 
        { 
            get { return _user; } 
            set { _user = value; } 
        }

        public ObservableCollection<ProfileViewModel> Friends { get; set; }

        // Edit button property
        private bool _profileEditable;
        public bool ProfileEditable
        {
            get { return _profileEditable; }
            set
            {
                _profileEditable = value;
                OnPropertyChanged(nameof(ProfileEditable));
            }
        }

        private string _content;
        public string Content
        {
            get { return _content; }
            set { 
                _content = value; 
                OnPropertyChanged(nameof(Content)); 
            }
        }


        // Navigation command
        public ICommand ProfileToLibCommand { get; }
        public ICommand ProfileToStoreCommand { get; }
        public ICommand LogOutCommand { get; }


        // Other commands
        public ICommand EditCommand { get; }

        public ICommand CloseCommand { get; }
        public ICommand MinimizeCommand { get; }


        // Constructor
        public ProfileViewModel(NavigationService NavigationService1, 
                                NavigationService NavigationService2, 
                                NavigationService NavigationService3,
                                NavigationService NavigationService4,
                                UserProfile user)
        {
            User = user;
            Friends = new ObservableCollection<ProfileViewModel>();
            AddFriends(user.UserFriends);
            ProfileEditable = false;
            Content = "Edit...";

            EditCommand = new EditProfileCommand(this);

            ProfileToLibCommand = new NavigateCommand(NavigationService1);
            ProfileToStoreCommand = new NavigateCommand(NavigationService2);
            LogOutCommand = new NavigateCommand(NavigationService3);
            CloseCommand = new NavigateCommand(NavigationService4);
            MinimizeCommand = new MinimizeCommand(NavigationService1);
            
        }

        // Constructor for "Friend" object
        public ProfileViewModel(UserProfile user)
        {
            User = user;

        }

        // Add user friends
        public void AddFriends(ObservableCollection<UserProfile> fr)
        {
            foreach (UserProfile f in fr)
            {
                Friends.Add(new ProfileViewModel(f));
            }
        }

    }
}
