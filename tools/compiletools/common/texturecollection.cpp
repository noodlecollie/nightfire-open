#include "texturecollection.h"
#include "miptexwrapper.h"
#include "hlassert.h"
#include "pngtexturepath.h"
#include "log.h"

class TextureCollection::Item
{
public:
	Item(ItemType type) :
		m_Type(type)
	{
	}

	virtual ~Item()
	{
	}

	ItemType type() const
	{
		return m_Type;
	}

	virtual std::string name() const = 0;
	virtual bool isExportable() const = 0;
	virtual size_t bytesInUse() const = 0;
	virtual size_t exportBytesRequired() const = 0;

private:
	ItemType m_Type;
};

class MiptexItem : public TextureCollection::Item
{
public:
	MiptexItem() : TextureCollection::Item(TextureCollection::ItemType::Miptex)
	{
	}

	virtual ~MiptexItem()
	{
	}

	inline MiptexWrapper* miptex()
	{
		return &m_Miptex;
	}

	inline const MiptexWrapper* miptex() const
	{
		return &m_Miptex;
	}

	virtual std::string name() const override
	{
		return std::string(m_Miptex.name());
	}

	virtual bool isExportable() const override
	{
		return m_Miptex.isValid();
	}

	virtual size_t bytesInUse() const override
	{
		return m_Miptex.dataSize();
	}

	virtual size_t exportBytesRequired() const override
	{
		return m_Miptex.exportDataSize();
	}

private:
	MiptexWrapper m_Miptex;
};

class PNGTextureItem : public TextureCollection::Item
{
public:
	PNGTextureItem() : TextureCollection::Item(TextureCollection::ItemType::PngOnDisk)
	{
	}

	virtual ~PNGTextureItem()
	{
	}

	inline PNGTexturePath* texture()
	{
		return &m_Texture;
	}

	inline const PNGTexturePath* texture() const
	{
		return &m_Texture;
	}

	virtual std::string name() const override
	{
		return m_Texture.path();
	}

	virtual bool isExportable() const override
	{
		return m_Texture.hasValidPath();
	}

	virtual size_t bytesInUse() const override
	{
		return m_Texture.path().size();
	}

	virtual size_t exportBytesRequired() const override
	{
		return m_Texture.exportDataSize();
	}

private:
	PNGTexturePath m_Texture;
};

TextureCollection::TextureCollection() :
	m_Items()
{
}

TextureCollection::~TextureCollection()
{
}

TextureCollection::ItemType TextureCollection::itemType(uint32_t index) const
{
	if ( index >= m_Items.size() || !m_Items[index].get() )
	{
		return ItemType::Undefined;
	}

	return m_Items[index]->type();
}

std::string TextureCollection::itemName(uint32_t index) const
{
	if ( index >= m_Items.size() || !m_Items[index].get() )
	{
		return std::string();
	}

	return m_Items[index]->name();
}

uint32_t TextureCollection::count(ItemType itemType) const
{
	if ( itemType != ItemType::Undefined )
	{
		return countItemsOfType(itemType);
	}
	else
	{
		return m_Items.size();
	}

}

MiptexWrapper* TextureCollection::miptexAt(uint32_t index)
{
	if ( itemType(index) != ItemType::Miptex )
	{
		return NULL;
	}

	return static_cast<MiptexItem*>(m_Items[index].get())->miptex();
}

const MiptexWrapper* TextureCollection::miptexAt(uint32_t index) const
{
	if ( itemType(index) != ItemType::Miptex )
	{
		return NULL;
	}

	return static_cast<const MiptexItem*>(m_Items[index].get())->miptex();
}

PNGTexturePath* TextureCollection::pngTextureAt(uint32_t index)
{
	if ( itemType(index) != ItemType::PngOnDisk )
	{
		return NULL;
	}

	return static_cast<PNGTextureItem*>(m_Items[index].get())->texture();
}

const PNGTexturePath* TextureCollection::pngTextureAt(uint32_t index) const
{
	if ( itemType(index) != ItemType::PngOnDisk )
	{
		return NULL;
	}

	return static_cast<const PNGTextureItem*>(m_Items[index].get())->texture();
}

void TextureCollection::filter(const std::function<bool(uint32_t, TextureCollection::ItemType)>& callback, std::vector<int32_t>& map)
{
	map.clear();

	const uint32_t originalCount = count();
	if ( originalCount < 1 )
	{
		return;
	}

	map.resize(originalCount, -1);
	uint32_t currentRemappedIndex = 0;

	for ( uint32_t index = 0; index < originalCount; ++index )
	{
		const bool filterResult = callback(index, itemType(index));

		if ( filterResult )
		{
			map[index] = currentRemappedIndex++;

			Developer(DEVELOPER_LEVEL_SPAM, "Texture %u (%s) remains after filtering: true. New index: %u\n",
					  index,
					  itemName(index).c_str(),
					  currentRemappedIndex - 1);
		}
		else
		{
			Developer(DEVELOPER_LEVEL_SPAM, "Texture %u (%s) remains after filtering: false\n",
					  index,
					  itemName(index).c_str());
		}

	}

	mapItems(map, currentRemappedIndex);
}

// Assumes that there are no holes in the map
// (because this should only be called with a map we've generated ourselves).
void TextureCollection::mapItems(const std::vector<int32_t>& map, uint32_t newCount)
{
	if ( newCount < 1 )
	{
		m_Items.clear();
		return;
	}

	ItemList tempItems;
	tempItems.resize(newCount);

	for ( uint32_t oldIndex = 0; oldIndex < m_Items.size(); ++oldIndex )
	{
		const int32_t newIndex = map[oldIndex];
		if ( newIndex < 0 || newIndex >= newCount )
		{
			continue;
		}

		tempItems[newIndex] = m_Items[oldIndex];
	}

	m_Items = tempItems;
}

uint32_t TextureCollection::countItemsOfType(ItemType itemType) const
{
	uint32_t count = 0;

	for ( const ItemPtr& item : m_Items )
	{
		if ( item->type() == itemType )
		{
			++count;
		}
	}

	return count;
}

bool TextureCollection::allocateAndAppend(size_t count, ItemType type)
{
	if ( count < 1 || type == ItemType::Undefined )
	{
		return false;
	}

	const size_t oldSize = m_Items.size();
	m_Items.resize(oldSize + count);

	for ( uint32_t index = oldSize; index < m_Items.size(); ++index )
	{
		m_Items[index] = createItem(type);
		hlassert(m_Items[index]);
	}

	return true;
}

void TextureCollection::truncate(size_t newCount)
{
	if ( newCount >= m_Items.size() )
	{
		return;
	}

	m_Items.resize(newCount);
}

void TextureCollection::clear()
{
	m_Items.clear();
}

size_t TextureCollection::totalBytesInUse(ItemType itemType) const
{
	size_t size = 0;

	for ( const ItemPtr& item : m_Items )
	{
		if ( !item.get() || (itemType != ItemType::Undefined && item->type() != itemType) )
		{
			continue;
		}

		size += item->bytesInUse();
	}

	return size;
}

size_t TextureCollection::exportBytesRequired(ItemType itemType) const
{
	size_t size = 0;

	for ( const ItemPtr& item : m_Items )
	{
		if ( !item.get() || (itemType != ItemType::Undefined && item->type() != itemType) )
		{
			continue;
		}

		size += item->exportBytesRequired();
	}

	return size;
}

size_t TextureCollection::exportableTextureCount(ItemType itemType) const
{
	size_t count = 0;

	for ( const ItemPtr& item : m_Items )
	{
		if ( !item.get() || (itemType != ItemType::Undefined && item->type() != itemType) )
		{
			continue;
		}

		if ( item->isExportable() )
		{
			++count;
		}
	}

	return count;
}

TextureCollection::ItemPtr TextureCollection::createItem(ItemType type)
{
	switch (type)
	{
		case ItemType::Miptex:
		{
			return ItemPtr(new MiptexItem());
		}

		case ItemType::PngOnDisk:
		{
			return ItemPtr(new PNGTextureItem());
		}

		default:
		{
			return ItemPtr();
		}
	}
}
