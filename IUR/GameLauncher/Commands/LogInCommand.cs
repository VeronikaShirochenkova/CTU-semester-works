using GameLauncher.Models;
using GameLauncher.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameLauncher.Commands
{
    public class LogInCommand : CommandBase
    {
        private readonly LoginViewModel _login;
        private readonly Data _data;
        public LogInCommand(LoginViewModel login, Data data)
        {
            _login = login;
            _data = data;
        }
        public override void Execute(object? parameter)
        {
            _login.LoginResult = _data.CheckData(_login.Login, _login.Password);
            _login.TryLogIn();
        }
    }
}
