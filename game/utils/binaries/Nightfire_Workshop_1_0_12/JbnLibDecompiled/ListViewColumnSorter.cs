// Decompiled with JetBrains decompiler
// Type: ListViewColumnSorter
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections;
using System.Windows.Forms;

public class ListViewColumnSorter : IComparer
{
  private int ColumnToSort;
  private SortOrder OrderOfSort;
  private CaseInsensitiveComparer ObjectCompare;

  public ListViewColumnSorter()
  {
    this.ColumnToSort = 0;
    this.OrderOfSort = SortOrder.None;
    this.ObjectCompare = new CaseInsensitiveComparer();
  }

  public int Compare(object x, object y)
  {
    int num = this.ObjectCompare.Compare((object) ((ListViewItem) x).SubItems[this.ColumnToSort].Text, (object) ((ListViewItem) y).SubItems[this.ColumnToSort].Text);
    if (this.OrderOfSort == SortOrder.Ascending)
      return num;
    return this.OrderOfSort == SortOrder.Descending ? -num : 0;
  }

  public int SortColumn
  {
    set => this.ColumnToSort = value;
    get => this.ColumnToSort;
  }

  public SortOrder Order
  {
    set => this.OrderOfSort = value;
    get => this.OrderOfSort;
  }
}
