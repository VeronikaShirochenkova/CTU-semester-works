using GameLauncher.ViewModels;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameLauncher.Commands
{
    public class InstallCommand : CommandBase
    {
        private readonly GameCardViewModel _card;

        public InstallCommand(GameCardViewModel card)
        {
            _card = card;
        }

        public override void Execute(object? parameter)
        {
            _card.IsInstalled = !_card.IsInstalled;

            //=========== SYSTEM PRINT ===================
            if (_card.IsInstalled)
            {
                Trace.WriteLine("Game: \"" + _card.GameName + "\" was installed!");
            }
            else
            {
                Trace.WriteLine("Game: \"" + _card.GameName + "\" was uninstalled!");
            }
            //============================================
        }
    }
}
