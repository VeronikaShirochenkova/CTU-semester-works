using GameLauncher.Models;
using GameLauncher.Services;
using GameLauncher.Stores;
using GameLauncher.ViewModels;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace GameLauncher
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private UserProfile _user;
        private readonly NavigationStore _navigationStore;
        private readonly Data _data;
        public MainWindow MainWindow_;
        
        // Constructor
        public App()
        {
            _data = new Data();
            _navigationStore = new NavigationStore();           
        }
        
        // Startup
        protected override void OnStartup(StartupEventArgs e)
        {
            _navigationStore.CurrentViewModel = CreateLoginViewModel();
            
            MainWindow_ = new MainWindow()
            {
                DataContext = new MainViewModel(_navigationStore)
            };
            MainWindow_.Show();

            _navigationStore.CurrentViewModel = CreateLoginViewModel();
           
            base.OnStartup(e);

            Trace.WriteLine("Try something from the list: \n");
            Trace.WriteLine("LOGIN: shirover\nPASSWORD: 12345\n");
            Trace.WriteLine("LOGIN: babaechi\nPASSWORD: 56789\n");
            Trace.WriteLine("LOGIN: timusfed\nPASSWORD: 45678\n");
            Trace.WriteLine("LOGIN: antondar\nPASSWORD: 11111\n");
        }

        // Navigation functions
        private LoginViewModel CreateLoginViewModel()
        {
            return new LoginViewModel(new NavigationService(_navigationStore, CreateProfileViewModel, MainWindow_), 
                                      new NavigationService(_navigationStore, CloseApplication, MainWindow_),
                                      _data);
        }

        private ProfileViewModel CreateProfileViewModel()
        {
            if (_navigationStore.CurrentViewModel.GetType() == typeof(LoginViewModel))
            {
                _user = _data.getCurrentUser();
            }
            return new ProfileViewModel(new NavigationService(_navigationStore,  CreateLibraryViewModel, MainWindow_),
                                        new NavigationService(_navigationStore, CreateStoreViewModel, MainWindow_),
                                        new NavigationService(_navigationStore, CreateLoginViewModel, MainWindow_),
                                        new NavigationService(_navigationStore, CloseApplication, MainWindow_),
                                        _user);
        }

        private LibraryViewModel CreateLibraryViewModel()
        {
            return new LibraryViewModel(new NavigationService(_navigationStore, CreateProfileViewModel, MainWindow_),
                                        new NavigationService(_navigationStore, CreateGameViewModel, MainWindow_),
                                        new NavigationService(_navigationStore, CreateStoreViewModel, MainWindow_),
                                        new NavigationService(_navigationStore, CreateLoginViewModel, MainWindow_),
                                        new NavigationService(_navigationStore, CloseApplication, MainWindow_),
                                        _user);
        }

        private EshopViewModel CreateStoreViewModel()
        {
            return new EshopViewModel(new NavigationService(_navigationStore, CreateProfileViewModel, MainWindow_),
                                      new NavigationService(_navigationStore, CreateLibraryViewModel, MainWindow_),
                                      new NavigationService(_navigationStore, CreateGameViewModel, MainWindow_),
                                      new NavigationService(_navigationStore, CreateLoginViewModel, MainWindow_),
                                      new NavigationService(_navigationStore, CloseApplication, MainWindow_),
                                      _data,
                                      _user);
        }

        private GameCardViewModel CreateGameViewModel()
        {
            if (_navigationStore.CurrentViewModel.GetType() == typeof(EshopViewModel))
            {
                return new GameCardViewModel(new NavigationService(_navigationStore, CreateStoreViewModel, MainWindow_),
                                             new NavigationService(_navigationStore, CloseApplication, MainWindow_), _user);
            }
                
            return new GameCardViewModel(new NavigationService(_navigationStore, CreateLibraryViewModel, MainWindow_),
                                         new NavigationService(_navigationStore, CloseApplication, MainWindow_), _user);
        }

        private ViewModelBase CloseApplication()
        {
            MainWindow.Close();
            return new ViewModelBase();
        }

    }
}
