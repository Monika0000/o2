#pragma once

#include "o2/Assets/Asset.h"

namespace o2
{
	// ---------------
	// Asset reference
	// ---------------
	class AssetRef: public ISerializable
	{
	public:
		// Default constructor, references to null
		AssetRef();

		// Copy-constructor
		AssetRef(const AssetRef& other);

		// Constructor from asset path
		AssetRef(const String& path);

		// Constructor from asset id
		AssetRef(const UID& id);

		// Destructor
		virtual ~AssetRef();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		AssetRef& operator=(const AssetRef& other);

		// Getter operator
		Asset& operator*();

		// Constant getter operator
		const Asset& operator*() const;

		// Asset members and field operator
		Asset* operator->();

		// Constant asset members and field operator
		const Asset* operator->() const;

		// Check equals operator
		bool operator==(const AssetRef& other) const;

		// Check not equals operator
		bool operator!=(const AssetRef& other) const;

		// Returns is reference is valid
		bool IsValid() const;

		// Returns asset type
		virtual const Type& GetAssetType() const;

		SERIALIZABLE(AssetRef);

	protected:
		bool   mAssetOwner = false;   // Is this reference owner of asset
		int*   mRefCounter = nullptr; // Reference count pointer
		Asset* mAssetPtr = nullptr;   // Asset pointer   

	protected:
		// Constructor for Assets manager
		AssetRef(Asset* assetPtr, int* refCounter);

		// Beginning serialization callback - writes path and id
		void OnSerialize(DataNode& node) const override;

		// Completion deserialization callback -  reads path and id and searches asset
		void OnDeserialized(const DataNode& node) override;

		friend class Assets;
	};

	template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>
	class Ref: public AssetRef
	{
	public:
		// Default constructor, references to null
		Ref(): AssetRef() {}

		// Copy-constructor
		Ref(const AssetRef& other): AssetRef(other) { mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr); }

		// Constructor from asset path
		Ref(const String& path): AssetRef(path) { mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr); }

		// Constructor from asset id
		Ref(const UID& id): AssetRef(id) { mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr); }

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		Ref<T>& operator=(const Ref<T>& other)
		{
			AssetRef::operator=(other); 
			mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr);
			return *this;
		}

		// Getter operator
		T& operator*() { return *mSpecAssetPtr; }

		// Constant getter operator
		const T& operator*() const { return *mSpecAssetPtr; }

		// Asset members and field operator
		T* operator->() { return mSpecAssetPtr; }

		// Constant asset members and field operator
		const T* operator->() const { return mSpecAssetPtr; }

		// Check equals operator
		bool operator==(const Ref<T>& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const Ref<T>& other) const { return AssetRef::operator!=(other); }

		// Returns asset type
		virtual const Type& GetAssetType() const { return TypeOf(T); }

		SERIALIZABLE(Ref<T>);

	protected:
		T* mSpecAssetPtr = nullptr;
	};
}

CLASS_BASES_META(o2::AssetRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetRef)
{
	PROTECTED_FIELD(mAssetOwner);
	PROTECTED_FIELD(mRefCounter);
	PROTECTED_FIELD(mAssetPtr);
}
END_META;
CLASS_METHODS_META(o2::AssetRef)
{

	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(const Type&, GetAssetType);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;

META_TEMPLATES(typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type)
CLASS_BASES_META(o2::Ref<T, std::enable_if<std::is_base_of<Asset, T>::value>::type>)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
META_TEMPLATES(typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type)
CLASS_FIELDS_META(o2::Ref<T, std::enable_if<std::is_base_of<Asset, T>::value>::type>)
{
	PROTECTED_FIELD(mSpecAssetPtr);
}
END_META;
META_TEMPLATES(typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type)
CLASS_METHODS_META(o2::Ref<T, std::enable_if<std::is_base_of<Asset, T>::value>::type>)
{

	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;