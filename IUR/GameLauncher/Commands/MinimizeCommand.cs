using GameLauncher.Services;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameLauncher.Commands
{
    public class MinimizeCommand : CommandBase
    {
        private readonly NavigationService _navigationService;

        public MinimizeCommand(NavigationService navigationService)
        {
            _navigationService = navigationService;
        }

        public override void Execute(object? parameter)
        {
            _navigationService.Minimize();
        }

    }
}
