// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.ScoreConverter
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.ComponentModel;
using System.Globalization;

namespace JbnLib.Log.Types
{
  public class ScoreConverter : ExpandableObjectConverter
  {
    public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType) => destinationType == typeof (Score) || base.CanConvertTo(context, destinationType);

    public override object ConvertTo(
      ITypeDescriptorContext context,
      CultureInfo culture,
      object value,
      Type destinationType)
    {
      return destinationType == typeof (string) && value is Score ? (object) ((Score) value).Name : base.ConvertTo(context, culture, value, destinationType);
    }
  }
}
