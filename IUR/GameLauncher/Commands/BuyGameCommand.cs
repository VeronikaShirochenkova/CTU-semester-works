using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameLauncher.Models;
using GameLauncher.ViewModels;

namespace GameLauncher.Commands
{
    public class BuyGameCommand : CommandBase
    {
        private readonly GameCardViewModel _card;
        private readonly UserProfile _user;
        
        public BuyGameCommand(GameCardViewModel card, UserProfile user)
        {
            _card = card;
            _user = user;
        }
        public override void Execute(object? parameter)
        {
            _user.UserBuyGame(_card.GameCardToGame());
            _card.Balance = _user.Money;
        }
    }
}
