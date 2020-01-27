#pragma once

#include "o2/Assets/AssetsTree.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Memory/MemoryManager.h"

#include "o2/Utils/Reflection/Type.h"

namespace o2
{
	class AssetsBuilder;

	// --------------------------------------
	// Asset converter interface
	// Converts assets with specialized types
	//---------------------------------------
	class IAssetConverter: public IObject
	{
	public:
		// Virtual destructor
		virtual ~IAssetConverter() {}

		// Returns vector of processing assets types
		virtual Vector<const Type*> GetProcessingAssetsTypes() const;

		// Converts asset by path
		virtual void ConvertAsset(const AssetsTree::AssetInfo& node);

		// Removes asset by path
		virtual void RemoveAsset(const AssetsTree::AssetInfo& node);

		// Moves asset to new path
		virtual void MoveAsset(const AssetsTree::AssetInfo& nodeFrom, const AssetsTree::AssetInfo& nodeTo);

		// Post processing
		virtual Vector<UID> AssetsPostProcess();

		// Resets converter
		virtual void Reset();

		// Sets owner assets builder
		void SetAssetsBuilder(AssetsBuilder* builder);

		IOBJECT(IAssetConverter);

	protected:
		// Assets builder pointer
		AssetsBuilder* mAssetsBuilder = nullptr;
	};
}

CLASS_BASES_META(o2::IAssetConverter)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::IAssetConverter)
{
	PROTECTED_FIELD(mAssetsBuilder);
}
END_META;
CLASS_METHODS_META(o2::IAssetConverter)
{

	PUBLIC_FUNCTION(Vector<const Type*>, GetProcessingAssetsTypes);
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetsTree::AssetInfo&, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(Vector<UID>, AssetsPostProcess);
	PUBLIC_FUNCTION(void, Reset);
	PUBLIC_FUNCTION(void, SetAssetsBuilder, AssetsBuilder*);
}
END_META;