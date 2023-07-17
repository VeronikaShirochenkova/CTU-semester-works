using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using GameLauncher.Commands;
using GameLauncher.Models;
using GameLauncher.Services;

namespace GameLauncher.ViewModels
{
    public class LoginViewModel : ViewModelBase
    {
        
        // Login
        private string _login;
        public string Login
        {
            get { return _login; }  
            set { SetProperty(ref _login, value); }
        }

        // password
        public string _password;
        public string Password
        {
            get { return _password; }
            set {SetProperty(ref _password, value); }
        }

        // authorization results
        private bool _loginResult;
        public bool LoginResult
        {
            get { return _loginResult; }
            set { SetProperty(ref _loginResult, value); }
        }

        // Navigation command
        public ICommand LogInCommand { get; }
        public ICommand CloseCommand { get; }

        // Other commands
        public ICommand CheckCommand { get; }

        public ICommand MinimizeCommand { get; }
        

        // Constructor
        public LoginViewModel(NavigationService NavigationService1, NavigationService NavigationService2, Data data)
        {
            CheckCommand = new LogInCommand(this, data);
            LogInCommand = new NavigateCommand(NavigationService1);
            CloseCommand = new NavigateCommand(NavigationService2);
            MinimizeCommand = new MinimizeCommand(NavigationService1);
            LoginResult = false;
            Login = "";
            Password = "";
        }

        // try log in in app
        public void TryLogIn()
        {
            
            if (LoginResult)
            {
                Trace.WriteLine("here!");
                LogInCommand.Execute(this);
            }
        }
    }
}
