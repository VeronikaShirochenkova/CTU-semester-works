using GameLauncher.Stores;
using GameLauncher.ViewModels;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace GameLauncher.Services
{
    public class NavigationService
    {
        private readonly NavigationStore _navigationStore;
        public MainWindow _mainWindow;
        public NavigationStore NavigationStore
        {
            get { return _navigationStore; }
        }
        private readonly Func<ViewModelBase> _createViewModel;

        public NavigationService(NavigationStore navigationStore, Func<ViewModelBase> createViewModel, MainWindow mainWindow)
        {
            _navigationStore = navigationStore;
            _createViewModel = createViewModel;
            _mainWindow = mainWindow;
        }
        public void Navigate()
        {
            _navigationStore.CurrentViewModel = _createViewModel();
            Trace.WriteLine("Current: " + _navigationStore.CurrentViewModel.ToString());

        }

        public void Minimize()
        {
            _mainWindow.WindowState = WindowState.Minimized;
        }
    }
}
