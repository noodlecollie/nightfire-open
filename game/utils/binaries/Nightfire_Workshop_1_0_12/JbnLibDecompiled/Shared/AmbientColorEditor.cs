// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.AmbientColorEditor
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace JbnLib.Shared
{
  public class AmbientColorEditor : UITypeEditor
  {
    private IWindowsFormsEditorService service;

    public override object EditValue(
      ITypeDescriptorContext context,
      IServiceProvider provider,
      object value)
    {
      if (provider != null)
        this.service = (IWindowsFormsEditorService) provider.GetService(typeof (IWindowsFormsEditorService));
      if (this.service != null)
      {
        AmbientColor ambientColor = (AmbientColor) value;
        ColorDialog colorDialog = new ColorDialog();
        colorDialog.Color = Color.FromArgb(ambientColor.GetARGB());
        int num = (int) colorDialog.ShowDialog();
        value = (object) new AmbientColor(colorDialog.Color.ToArgb());
      }
      return value;
    }

    public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context) => UITypeEditorEditStyle.Modal;
  }
}
