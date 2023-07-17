using GameLauncher.Models;
using GameLauncher.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameLauncher.Commands
{
    public class SelectGameCommand : CommandBase
    {
        private readonly GameCardViewModel _card;
        public SelectGameCommand(GameCardViewModel card)
        {
            _card = card;
        }
        public override void Execute(object? parameter)
        {
            _card.selectGame();
        }
    }
}
