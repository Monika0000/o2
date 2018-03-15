#include "stdafx.h"
#include "ActorProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Application/Application.h"
#include "Assets/ActorAsset.h"
#include "Assets/Assets.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Scene/Actor.h"
#include "Scene/Component.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetState.h"

namespace Editor
{
	ActorProperty::ActorProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("actor property");

		mBox = mPropertyWidget->GetChildWidget("box");
		if (!mBox)
			mBox = mPropertyWidget;

		mBox->onDraw += [&]() { OnDrawn(); };
		mBox->SetFocusable(true);

		mNameText = mBox->GetLayerDrawable<Text>("caption");
		mNameText->text = "--";

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);
	}

	ActorProperty::~ActorProperty()
	{
		delete mPropertyWidget;
	}

	void ActorProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, Actor* value) { *((PROPERTY(Actor*>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((PROPERTY(Actor*>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, Actor* value) { *((Actor**)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Actor**)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void ActorProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newCommonValue != mGetFunc(mValuesPointers[i].first))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue();
		}
		else if (lastCommonValue != newCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);

		CheckRevertableState();
	}

	void ActorProperty::Revert()
	{
		auto propertyObjects = o2EditorProperties.GetTargets();

		for (int i = 0; i < mValuesPointers.Count() && i < propertyObjects.Count(); i++)
			RevertoToPrototype(mValuesPointers[i].first, mValuesPointers[i].second, propertyObjects[i]);

		Refresh();
	}

	void ActorProperty::RevertoToPrototype(void* target, void* source, IObject* targetOwner)
	{
		if (!source || !targetOwner || targetOwner->GetType().IsBasedOn(TypeOf(Component)))
			return;

		Actor* sourceActor = mGetFunc(source);
		Actor* topSourceActor = sourceActor;
		while (topSourceActor->GetParent())
			topSourceActor = topSourceActor->GetParent();

		Actor* actorOwner = dynamic_cast<Actor*>(targetOwner);

		if (actorOwner)
		{
			Actor* topTargetActor = actorOwner;
			while (topTargetActor->GetPrototypeLink() != topSourceActor && topTargetActor->GetParent())
				topTargetActor = topTargetActor->GetParent();

			Actor* sameToProtoSourceActor = topTargetActor->FindLinkedActor(sourceActor);

			if (sameToProtoSourceActor)
			{
				mAssignFunc(target, sameToProtoSourceActor);
				return;
			}
		}

		if (sourceActor->IsOnScene())
			mAssignFunc(target, sourceActor);
	}

	UIWidget* ActorProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	Actor* ActorProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool ActorProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* ActorProperty::GetFieldType() const
	{
		return &TypeOf(Actor*);
	}

	void ActorProperty::SetValue(Actor* value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void ActorProperty::SetUnknownValue()
	{
		mCommonValue = nullptr;
		mValuesDifferent = true;

		mNameText->text = "--";
		mBox->layer["caption"]->transparency = 1.0f;

		CheckRevertableState();

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	bool ActorProperty::IsUnderPoint(const Vec2F& point)
	{
		return mBox->IsUnderPoint(point);
	}

	void ActorProperty::SetCommonValue(Actor* value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		if (!mCommonValue)
		{
			mNameText->text = "Null:Actor";
			mBox->layer["caption"]->transparency = 0.5f;
		}
		else
		{
			mNameText->text = mCommonValue->GetName();
			mBox->layer["caption"]->transparency = 1.0f;
		}

		CheckRevertableState();

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void ActorProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesPointers)
		{
			if (ptr.second)
			{
				Actor* value = mGetFunc(ptr.first);
				Actor* proto = mGetFunc(ptr.second);

				if (value && value->GetPrototypeLink())
				{
					if (value->GetPrototypeLink() != proto)
					{
						revertable = true;
						break;
					}
				}
				else
				{
					if (value != proto)
					{
						revertable = true;
						break;
					}
				}
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void ActorProperty::OnCursorEnter(const Input::Cursor& cursor)
	{
		mBox->SetState("hover", true);
	}

	void ActorProperty::OnCursorExit(const Input::Cursor& cursor)
	{
		mBox->SetState("hover", false);
	}

	void ActorProperty::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mBox);

		if (mCommonValue)
		{
			if (mCommonValue->IsAsset())
				o2EditorAssets.ShowAssetIcon(o2Assets.GetAssetPath(mCommonValue->GetAssetID()));
			else if (mCommonValue->IsOnScene())
				o2EditorTree.HightlightActorsTreeNode(mCommonValue);
		}
	}

	void ActorProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox->IsFocused() && key == VK_DELETE || key == VK_BACK)
			SetValueByUser(nullptr);
	}

	void ActorProperty::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDroppedFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDroppedFromAssetsScroll(assetsScroll);
	}

	void ActorProperty::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDragEnterFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDragEnterFromAssetsScroll(assetsScroll);
	}

	void ActorProperty::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDragExitFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDragExitFromAssetsScroll(assetsScroll);
	}

	void ActorProperty::OnDroppedFromActorsTree(UIActorsTree* actorsTree)
	{
		if (actorsTree->GetSelectedActors().Count() > 1)
			return;

		SetValueByUser(actorsTree->GetSelectedActors()[0]);

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}

	void ActorProperty::OnDragEnterFromActorsTree(UIActorsTree* actorsTree)
	{
		if (actorsTree->GetSelectedActors().Count() > 1)
			return;

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	void ActorProperty::OnDragExitFromActorsTree(UIActorsTree* actorsTree)
	{
		if (actorsTree->GetSelectedActors().Count() > 1)
			return;

		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void ActorProperty::OnDroppedFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		auto lastSelectedAsset = assetsIconsScroll->GetSelectedAssets().Last();
		if (lastSelectedAsset.assetType != &TypeOf(ActorAsset))
			return;

		SetValueByUser(o2Scene.GetAssetActorByID(lastSelectedAsset.id));

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}

	void ActorProperty::OnDragEnterFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		auto lastSelectedAsset = assetsIconsScroll->GetSelectedAssets().Last();
		if (lastSelectedAsset.assetType != &TypeOf(ActorAsset))
			return;

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	void ActorProperty::OnDragExitFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void ActorProperty::SetValueByUser(Actor* value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void ActorProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void ActorProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::ActorProperty);
