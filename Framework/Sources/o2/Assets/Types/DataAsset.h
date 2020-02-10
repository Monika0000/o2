#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Utils/Serialization/DataNode.h"

namespace o2
{
	// ----------
	// Data asset
	// ----------
	class DataAsset: public AssetWithDefaultMeta<DataAsset>
	{
	public:
		DataNode data; // Asset data

	public:
		// Check equals operator
		DataAsset& operator=(const DataAsset& asset);

		// Returns extensions string
		const char* GetFileExtensions() const override;

		SERIALIZABLE(DataAsset);

	protected:
		// Default constructor
		DataAsset();

		// Copy-constructor
		DataAsset(const DataAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		// Saves data
		void SaveData(const String& path) const override;

		friend class Assets;
	};

	typedef Ref<DataAsset> DataAssetRef;
}

CLASS_BASES_META(o2::DataAsset)
{
	BASE_CLASS(o2::AssetWithDefaultMeta<DataAsset>);
}
END_META;
CLASS_FIELDS_META(o2::DataAsset)
{
	PUBLIC_FIELD(data);
}
END_META;
CLASS_METHODS_META(o2::DataAsset)
{

	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;