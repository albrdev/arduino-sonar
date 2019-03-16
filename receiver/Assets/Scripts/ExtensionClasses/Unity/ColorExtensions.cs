using UnityEngine;

namespace Assets.Scripts.ExtensionClasses
{
    public static class ColorExtensions
    {
        public static Color SetRed(this Color self, float value)
        {
            return new Color(value, self.g, self.b, self.a);
        }

        public static Color SetGreen(this Color self, float value)
        {
            return new Color(self.r, value, self.b, self.a);
        }

        public static Color SetBlue(this Color self, float value)
        {
            return new Color(self.r, self.g, value, self.a);
        }

        public static Color SetAlpha(this Color self, float value)
        {
            return new Color(self.r, self.g, self.b, value);
        }
    }
}
