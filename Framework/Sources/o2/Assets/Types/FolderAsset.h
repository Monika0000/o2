#pragma once

#include "o2/Assets/Asset.h"

namespace o2
{
	// ------------
	// Folder asset
	// ------------
	class FolderAsset: public AssetWithDefaultMeta<FolderAsset>
	{
	public:
		// Check equals operator
		FolderAsset& operator=(const FolderAsset& asset);

		// Returns containing assets infos
		Vector<AssetRef> GetContainingAssetsInfos() const;

		SERIALIZABLE(FolderAsset);

	protected:
		// Default constructor
		FolderAsset();

		// Copy-constructor
		FolderAsset(const FolderAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		// Saves asset data
		void SaveData(const String& path) const override;

		friend class Assets;
	};

	typedef Ref<FolderAsset> FolderAssetRef;
}

CLASS_BASES_META(o2::FolderAsset)
{
	BASE_CLASS(o2::AssetWithDefaultMeta<FolderAsset>);
}
END_META;
CLASS_FIELDS_META(o2::FolderAsset)
{
}
END_META;
CLASS_METHODS_META(o2::FolderAsset)
{

	PUBLIC_FUNCTION(Vector<AssetRef>, GetContainingAssetsInfos);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;
