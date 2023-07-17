using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameLauncher.Services;
using GameLauncher.Stores;
using GameLauncher.ViewModels;

namespace GameLauncher.Commands
{
    public class NavigateCommand : CommandBase
    {
        private readonly NavigationService _navigationService;

        public NavigateCommand(NavigationService navigationService)
        {
            _navigationService = navigationService;
        }

        public override void Execute(object? parameter)
        {         
            _navigationService.Navigate();
        }

    }
}
