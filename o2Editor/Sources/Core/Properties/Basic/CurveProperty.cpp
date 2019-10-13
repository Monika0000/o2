#include "stdafx.h"
#include "CurveProperty.h"

#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"
#include "Core/UI/CurvePreview.h"
#include "Core/Dialogs/CurveEditorDlg.h"

namespace Editor
{
	CurveProperty::CurveProperty()
	{}

	CurveProperty::CurveProperty(const CurveProperty& other) :
		TPropertyField<Curve>(other)
	{
		InitializeControls();
	}

	CurveProperty& CurveProperty::operator=(const CurveProperty& other)
	{
		TPropertyField<Curve>::operator=(other);
		return *this;
	}

	void CurveProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField<Curve>::CopyData(otherActor);
		InitializeControls();
	}

	void CurveProperty::InitializeControls()
	{
		mEditBox = GetChildWidget("layout/box");
		if (mEditBox)
		{
			mPreviewImage = mnew CurvePreview();
			*mPreviewImage->layout = WidgetLayout::BothStretch(1, 1, 1, 1);
			mPreviewImage->SetCurve(&mCommonValue);

			mEditBox->layout->minHeight = 10;
			mEditBox->onDraw += [&]() { mClickArea.OnDrawn(); };
			mEditBox->AddChild(mPreviewImage);

			mClickArea.isUnderPoint = [&](const Vec2F& point) { return mEditBox->IsUnderPoint(point); };
			mClickArea.onCursorReleased = [&](const Input::Cursor& cursor) { if (mEditBox->IsUnderPoint(cursor.position)) OnClicked(); };
		}
	}

	void CurveProperty::OnValueChanged()
	{
		for (auto ptr : mValuesProxies)
			SetProxy<Curve>(ptr.first, mCommonValue);
	}

	void CurveProperty::OnClicked()
	{
		StoreValues(mBeforeChangeValues);

		CurveEditorDlg::Show(THIS_FUNC(OnValueChanged),
							 MakeFunction<IPropertyField, void>(this, &CurveProperty::CheckValueChangeCompleted));

		CurveEditorDlg::RemoveAllEditingCurves();
		CurveEditorDlg::AddEditingCurve("property", &mCommonValue, Color4(44, 62, 80));
	}
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::Curve>);

DECLARE_CLASS(Editor::CurveProperty);
