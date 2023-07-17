using GameLauncher.ViewModels;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace GameLauncher.Commands
{
    public class RemoveGameCommand : CommandBase
    {
        private LibraryViewModel _libVMRef;
        private GameCardViewModel _gameCard;
        public RemoveGameCommand(GameCardViewModel card, LibraryViewModel refLib)
        {
            _gameCard = card;
            _libVMRef = refLib;
        }
        public override void Execute(object? parameter)
        {
            _libVMRef.removeGame(_gameCard.GameName);
            _libVMRef.LibraryGames.Remove(_gameCard);
        }

    }
}
