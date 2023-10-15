// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.VariableConverter
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.ComponentModel;
using System.Globalization;

namespace JbnLib.Log.Types
{
  public class VariableConverter : ExpandableObjectConverter
  {
    public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType) => destinationType == typeof (Variable) || base.CanConvertTo(context, destinationType);

    public override object ConvertTo(
      ITypeDescriptorContext context,
      CultureInfo culture,
      object value,
      Type destinationType)
    {
      if (destinationType != typeof (string) || !(value is Variable))
        return base.ConvertTo(context, culture, value, destinationType);
      Variable variable = (Variable) value;
      if (variable.Name == null)
        variable.Name = "*Nobody*";
      if (variable.Value == null)
        variable.Value = "";
      return (object) (variable.Name + ", " + variable.Value);
    }
  }
}
