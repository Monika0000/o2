#include "o2Editor/stdafx.h"
#include "PropertiesContext.h"

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/System/Time/Time.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

namespace Editor
{

	void PropertiesContext::Invalidate()
	{
		targets.Clear();
	}

	void PropertiesContext::Set(const Vector<Pair<IObject*, IObject*>>& targets, bool force /*= false*/)
	{
		if (this->targets == targets && !force)
		{
			Refresh();
			return;
		}

		this->targets = targets;

		if (targets.IsEmpty())
			return;

		for (auto& kv : properties)
		{
			auto fieldPointers = targets.Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
				[&](const Pair<IObject*, IObject*>& x)
			{
				auto fieldInfo = kv.first;
				const Type& type = *fieldInfo->GetOwnerType();
				const ObjectType* objType = dynamic_cast<const ObjectType*>(&type);

				if (objType == nullptr)
					return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(nullptr, nullptr);

				void* firstObjectPtr = objType->DynamicCastFromIObject(x.first);
				void* secondObjectPtr = nullptr;
				if (x.second)
					secondObjectPtr = objType->DynamicCastFromIObject(x.second);

				IAbstractValueProxy* firstValuePtr = fieldInfo->GetType()->GetValueProxy(fieldInfo->GetValuePtrStrong(firstObjectPtr));
				IAbstractValueProxy* secondValuePtr = nullptr;
				if (x.second)
					secondValuePtr = fieldInfo->GetType()->GetValueProxy(fieldInfo->GetValuePtrStrong(secondObjectPtr));

				return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(firstValuePtr, secondValuePtr);
			});

			kv.second->SetValueAndPrototypeProxy(fieldPointers);
		}
	}

	void PropertiesContext::Refresh()
	{
		for (auto& kv : properties)
			kv.second->Refresh();
	}

	bool PropertiesContext::IsBuiltWIthPrivateProperties() const
	{
		return builtWithPrivateProperties;
	}

	const Map<const FieldInfo*, IPropertyField*>& PropertiesContext::GetProperties() const
	{
		return properties;
	}

}
