#include "DefaultActorComponentViewer.h"

#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Component.h"
#include "UI/Label.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"

namespace Editor
{
	DefaultActorComponentViewer::DefaultActorComponentViewer()
	{}

	DefaultActorComponentViewer::~DefaultActorComponentViewer()
	{}

	void DefaultActorComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		mTargetComponents = components;
		mFieldProperties.Set(components.Select<IObject*>([](auto x) { return (IObject*)x; }));
	}

	const Type* DefaultActorComponentViewer::GetComponentType() const
	{
		return mComponentType;
	}

	void DefaultActorComponentViewer::SepcializeComponentType(const Type* type)
	{
		mComponentType = type;
		o2EditorProperties.BuildTypeViewer((UIVerticalLayout*)mPropertiesLayout, type, mFieldProperties);
		mNameCaption->text = type->Name();
	}

}
 
CLASS_META(Editor::DefaultActorComponentViewer)
{
	BASE_CLASS(Editor::IActorComponentViewer);

	PROTECTED_FIELD(mTargetComponents);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mComponentType);

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
	PUBLIC_FUNCTION(void, SepcializeComponentType, const Type*);
}
END_META;
 