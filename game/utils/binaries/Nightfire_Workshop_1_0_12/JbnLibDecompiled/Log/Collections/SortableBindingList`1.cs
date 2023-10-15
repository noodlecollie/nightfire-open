// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Collections.SortableBindingList`1
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace JbnLib.Log.Collections
{
  public class SortableBindingList<T> : BindingList<T>
  {
    private bool _isSorted;
    private ListSortDirection _sortDirection;
    private PropertyDescriptor _sortProperty;

    protected override bool SupportsSortingCore => true;

    protected override void ApplySortCore(PropertyDescriptor property, ListSortDirection direction)
    {
      if (this.Items is List<T> items)
      {
        PropertyComparer<T> propertyComparer = new PropertyComparer<T>(property, direction);
        items.Sort((IComparer<T>) propertyComparer);
        this._isSorted = true;
        this._sortDirection = direction;
        this._sortProperty = property;
      }
      else
        this._isSorted = false;
      this.OnListChanged(new ListChangedEventArgs(ListChangedType.Reset, -1));
    }

    protected override bool IsSortedCore => this._isSorted;

    protected override ListSortDirection SortDirectionCore => this._sortDirection;

    protected override PropertyDescriptor SortPropertyCore => this._sortProperty;

    protected override void RemoveSortCore() => this._isSorted = false;

    public void Save(string filename)
    {
      using (FileStream serializationStream = new FileStream(filename, FileMode.Create))
        new BinaryFormatter().Serialize((Stream) serializationStream, (object) (List<T>) this.Items);
    }

    public void Load(string filename)
    {
      this.ClearItems();
      if (File.Exists(filename))
      {
        using (FileStream serializationStream = new FileStream(filename, FileMode.Open))
          ((List<T>) this.Items).AddRange((IEnumerable<T>) new BinaryFormatter().Deserialize((Stream) serializationStream));
      }
      this.OnListChanged(new ListChangedEventArgs(ListChangedType.Reset, -1));
    }
  }
}
