using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameLauncher.ViewModels;

namespace GameLauncher.Commands
{
    public class EditProfileCommand : CommandBase
    {
        private readonly ProfileViewModel _profile;

        public override void Execute(object? parameter)
        {
            _profile.ProfileEditable = !_profile.ProfileEditable;
            _profile.Content = _profile.ProfileEditable ? "Save" : "Edit...";
        }

        public EditProfileCommand(ProfileViewModel profileViewModel)
        {
            _profile = profileViewModel;
        }
    }
}
