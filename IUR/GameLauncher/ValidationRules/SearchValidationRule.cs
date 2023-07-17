using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace GameLauncher.ValidationRules
{
    public class SearchValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {

            if (((string)value).Length < 4)
            {
                return new ValidationResult(false, "Password must be at least four characters");
            }

            return ValidationResult.ValidResult;
        }
    }
}
