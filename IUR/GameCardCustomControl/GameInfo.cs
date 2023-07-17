using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Controls.Primitives;
using System.Printing.IndexedProperties;

namespace GameCardCustomControl
{
    /// <summary>
    /// Follow steps 1a or 1b and then 2 to use this custom control in a XAML file.
    ///
    /// Step 1a) Using this custom control in a XAML file that exists in the current project.
    /// Add this XmlNamespace attribute to the root element of the markup file where it is 
    /// to be used:
    ///
    ///     xmlns:MyNamespace="clr-namespace:GameCardCustomControl"
    ///
    ///
    /// Step 1b) Using this custom control in a XAML file that exists in a different project.
    /// Add this XmlNamespace attribute to the root element of the markup file where it is 
    /// to be used:
    ///
    ///     xmlns:MyNamespace="clr-namespace:GameCardCustomControl;assembly=GameCardCustomControl"
    ///
    /// You will also need to add a project reference from the project where the XAML file lives
    /// to this project and Rebuild to avoid compilation errors:
    ///
    ///     Right click on the target project in the Solution Explorer and
    ///     "Add Reference"->"Projects"->[Browse to and select this project]
    ///
    ///
    /// Step 2)
    /// Go ahead and use your control in the XAML file.
    ///
    ///     <MyNamespace:GameInfo/>
    ///
    /// </summary>
    public class GameInfo : ToggleButton
    {
        static GameInfo()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(GameInfo), new FrameworkPropertyMetadata(typeof(GameInfo)));
        }

        public string NameGame
        {
            get => (string)GetValue(NameGameProperty);
            set => SetValue(NameGameProperty, value);
        }

        public string Genre
        {
            get => (string)GetValue(GenreProperty);
            set => SetValue(GenreProperty, value);
        }
        public int Price 
        {
            get => (int)GetValue(PriceProperty);
            set => SetValue(PriceProperty, value);
        }
        public string Description 
        {
            get => (string)GetValue(DescriptionProperty);
            set => SetValue(DescriptionProperty, value);
        }
        public string ReleaseDate 
        {
            get => (string)GetValue(ReleaseDateProperty);
            set => SetValue(ReleaseDateProperty, value);
        }

        public string Publisher
        {
            get => (string)GetValue(PublisherProperty);
            set => SetValue(PublisherProperty, value);
        }

        public string Developer
        {
            get => (string)GetValue(DeveloperProperty);
            set => SetValue(DeveloperProperty, value);
        }

        public static readonly DependencyProperty NameGameProperty = DependencyProperty.Register("NameGame", typeof(string), typeof(GameInfo));
        public static readonly DependencyProperty GenreProperty = DependencyProperty.Register("Genre", typeof(string), typeof(GameInfo));
        public static readonly DependencyProperty PriceProperty = DependencyProperty.Register("Price", typeof(int), typeof(GameInfo));
        public static readonly DependencyProperty DescriptionProperty = DependencyProperty.Register("Description", typeof(string), typeof(GameInfo));
        public static readonly DependencyProperty ReleaseDateProperty = DependencyProperty.Register("ReleaseDate", typeof(string), typeof(GameInfo));
        public static readonly DependencyProperty PublisherProperty = DependencyProperty.Register("Publisher", typeof(string), typeof(GameInfo));
        public static readonly DependencyProperty DeveloperProperty = DependencyProperty.Register("Developer", typeof(string), typeof(GameInfo));
    }
}
