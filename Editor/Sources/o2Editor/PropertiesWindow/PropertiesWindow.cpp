#include "o2Editor/stdafx.h"
#include "PropertiesWindow.h"

#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2Editor/Core/Properties/Objects/DefaultObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/PropertiesWindow/DefaultPropertiesViewer.h"
#include "o2Editor/PropertiesWindow/IPropertiesViewer.h"


DECLARE_SINGLETON(Editor::PropertiesWindow);

namespace Editor
{
	PropertiesWindow::PropertiesWindow():
		mCurrentViewer(nullptr)
	{
		InitializeWindow();
		InitializeViewers();
	}

	PropertiesWindow::~PropertiesWindow()
	{
		for (auto viewer : mViewers)
			delete viewer;
	}

	void PropertiesWindow::ResetTargets()
	{
		SetTarget(nullptr);
	}

	void PropertiesWindow::InitializeWindow()
	{
		mWindow->caption = "Properties";
		mWindow->name = "properties window";
		mWindow->SetIcon(mnew Sprite("ui/UI4_gear_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 2)));
		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, -2, 0, 15));

		InitializeWindowContext();
	}

	void PropertiesWindow::InitializeWindowContext()
	{
		auto context = mWindow->GetOptionsMenu();
		if (!context)
		{
			o2Debug.LogError("Failed to initialize properties window context menu: not found menu on window");
			return;
		}

		context->AddItem(ContextMenu::Item::Separator());
		context->AddItem(ContextMenu::Item("Private visible", false, THIS_FUNC(OnPrivateFieldsVisibleChanged)));
	}

	void PropertiesWindow::InitializeViewers()
	{
		auto viewersTypes = TypeOf(IPropertiesViewer).GetDerivedTypes();
		viewersTypes.Remove(&TypeOf(DefaultPropertiesViewer));

		for (auto type : viewersTypes)
			mViewers.Add((IPropertiesViewer*)type->CreateSample());

		mDefaultViewer = mnew DefaultPropertiesViewer();
	}

	void PropertiesWindow::OnPrivateFieldsVisibleChanged(bool visible)
	{
		o2EditorProperties.SetPrivateFieldsVisible(visible);

		if (mCurrentViewer)
			mCurrentViewer->SetTargets(mTargets);
	}

	void PropertiesWindow::OnPropertyChanged(IPropertyField* field)
	{
		mTargetsChanged = true;
	}

	void PropertiesWindow::SetTarget(IObject* target)
	{
		if (target == nullptr)
			SetTargets(Vector<IObject*>());
		else
			SetTargets({ target });
	}

	void PropertiesWindow::SetTargets(const Vector<IObject*> targets, const Function<void()>& targetsChangedDelegate /*= Function<void()>()*/)
	{
		if (mTargetsChanged)
			mOnTargetsChangedDelegate();

		IPropertiesViewer* objectViewer = nullptr;
		if (!targets.IsEmpty())
		{
			auto type = &targets[0]->GetType();
			objectViewer = mViewers.FindMatchOrDefault([&](auto x) { return type->IsBasedOn(*x->GetViewingObjectType()); });
		}

		if (!objectViewer)
			objectViewer = mDefaultViewer;

		if (objectViewer != mCurrentViewer)
		{
			if (mCurrentViewer)
			{
				mCurrentViewer->mContentWidget->Hide(true);
				mCurrentViewer->OnDisabled();
			}

			mCurrentViewer = objectViewer;

			if (mCurrentViewer)
			{
				mCurrentViewer->mContentWidget->SetParent(mWindow);
				*mCurrentViewer->mContentWidget->layout = WidgetLayout::BothStretch();
				mCurrentViewer->mContentWidget->Show(true);
				mCurrentViewer->OnEnabled();
			}
		}

		mTargets = targets;

		if (mCurrentViewer)
			mCurrentViewer->SetTargets(mTargets);

		mOnTargetsChangedDelegate = targetsChangedDelegate;
		mTargetsChanged = false;
	}

	Vector<IObject*> PropertiesWindow::GetTargets() const
	{
		return mTargets;
	}

	void PropertiesWindow::Update(float dt)
	{
		if (mCurrentViewer)
			mCurrentViewer->Update(dt);
	}

	void PropertiesWindow::Draw()
	{
		if (mCurrentViewer)
			mCurrentViewer->Draw();
	}

	bool PropertiesWindow::IsTargetsChanged() const
	{
		return mTargetsChanged;
	}
}

DECLARE_CLASS(Editor::PropertiesWindow);
