// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.AmbientColorConverter
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.ComponentModel;
using System.Globalization;

namespace JbnLib.Shared
{
  public class AmbientColorConverter : TypeConverter
  {
    public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType) => sourceType == typeof (string) || base.CanConvertFrom(context, sourceType);

    public override object ConvertFrom(
      ITypeDescriptorContext context,
      CultureInfo culture,
      object value)
    {
      return value.GetType() == typeof (string) ? (object) new AmbientColor((string) value) : base.ConvertFrom(context, culture, value);
    }

    public override object ConvertTo(
      ITypeDescriptorContext context,
      CultureInfo culture,
      object value,
      Type destType)
    {
      return destType == typeof (string) && (object) (value as AmbientColor) != null ? (object) ((AmbientColor) value).ToString() : base.ConvertTo(context, culture, value, destType);
    }
  }
}
