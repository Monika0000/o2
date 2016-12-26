#include "RectangleIntProperty.h"

#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"

namespace Editor
{
	RectIProperty::RectIProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mWidget = widget;
		else
			mWidget = o2UI.CreateWidget<UIWidget>("rectangle property");

		mLeftEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("left edit"));
		mBottomEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("bottom edit"));
		mRightEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("right edit"));
		mTopEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("top edit"));

		mLeftEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectIProperty::OnLeftEdited);
		mLeftEditBox->SetFilterInteger();

		mBottomEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectIProperty::OnBottomEdited);
		mBottomEditBox->SetFilterInteger();

		mRightEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectIProperty::OnRightEdited);
		mRightEditBox->SetFilterInteger();

		mTopEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectIProperty::OnTopEdited);
		mTopEditBox->SetFilterInteger();
	}

	RectIProperty::~RectIProperty()
	{
		delete mWidget;
	}

	void RectIProperty::SetValue(const RectI& value)
	{
		mLeftValuesDifferent = false;
		mBottomValuesDifferent = false;
		mRightValuesDifferent = false;
		mTopValuesDifferent = false;

		mCommonValue = value;

		mLeftEditBox->text = (WString)mCommonValue.left;
		mBottomEditBox->text = (WString)mCommonValue.bottom;
		mRightEditBox->text = (WString)mCommonValue.right;
		mTopEditBox->text = (WString)mCommonValue.top;
	}

	void RectIProperty::SetUnknownValue()
	{
		mLeftValuesDifferent = true;
		mBottomValuesDifferent = true;
		mRightValuesDifferent = true;
		mTopValuesDifferent = true;

		mCommonValue = RectI();

		mLeftEditBox->text = "--";
		mBottomEditBox->text = "--";
		mRightEditBox->text = "--";
		mTopEditBox->text = "--";
	}

	void RectIProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const RectI& value) { *((Property<RectI>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<RectI>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const RectI& value) { *((RectI*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((RectI*)(ptr)); };
		}

		mValuesPointers = targets;

		Update();
	}

	void RectIProperty::Update()
	{
		if (mValuesPointers.IsEmpty())
			return;

		mCommonValue = mGetFunc(mValuesPointers[0]);
		mLeftValuesDifferent = false;
		mBottomValuesDifferent = false;
		mRightValuesDifferent = false;
		mTopValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			auto value = mGetFunc(mValuesPointers[i]);
			if (!Math::Equals(mCommonValue.left, value.left))
			{
				mLeftValuesDifferent = true;
				break;
			}

			if (!Math::Equals(mCommonValue.bottom, value.bottom))
			{
				mBottomValuesDifferent = true;
				break;
			}

			if (!Math::Equals(mCommonValue.right, value.right))
			{
				mRightValuesDifferent = true;
				break;
			}

			if (!Math::Equals(mCommonValue.top, value.top))
			{
				mTopValuesDifferent = true;
				break;
			}
		}

		if (mLeftValuesDifferent)
		{
			mLeftEditBox->text = "--";
			mCommonValue.left = 0;
		}
		else mLeftEditBox->text = (WString)mCommonValue.left;

		if (mBottomValuesDifferent)
		{
			mBottomEditBox->text = "--";
			mCommonValue.bottom = 0;
		}
		else mBottomEditBox->text = (WString)mCommonValue.bottom;

		if (mRightValuesDifferent)
		{
			mRightEditBox->text = "--";
			mCommonValue.right = 0;
		}
		else mRightEditBox->text = (WString)mCommonValue.right;

		if (mTopValuesDifferent)
		{
			mTopEditBox->text = "--";
			mCommonValue.top = 0;
		}
		else mTopEditBox->text = (WString)mCommonValue.top;
	}

	UIWidget* RectIProperty::GetWidget() const
	{
		return mWidget;
	}

	RectI RectIProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool RectIProperty::IsValuesDifferent() const
	{
		return mLeftValuesDifferent || mBottomValuesDifferent || mRightValuesDifferent || mTopValuesDifferent;
	}

	const Type* RectIProperty::GetFieldType() const
	{
		return &TypeOf(RectI);
	}

	void RectIProperty::OnLeftEdited(const WString& data)
	{
		if (mLeftValuesDifferent && data == "--")
			return;

		mCommonValue.left = (const int)data;
		mLeftValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		onChanged();
	}

	void RectIProperty::OnBottomEdited(const WString& data)
	{
		if (mBottomValuesDifferent && data == "--")
			return;

		mCommonValue.bottom = (const int)data;
		mBottomValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		onChanged();
	}

	void RectIProperty::OnRightEdited(const WString& data)
	{
		if (mRightValuesDifferent && data == "--")
			return;

		mCommonValue.right = (const int)data;
		mRightValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		onChanged();
	}

	void RectIProperty::OnTopEdited(const WString& data)
	{
		if (mTopValuesDifferent && data == "--")
			return;

		mCommonValue.top = (const int)data;
		mTopValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		onChanged();
	}
}
 
CLASS_META(Editor::RectIProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mLeftValuesDifferent);
	PROTECTED_FIELD(mBottomValuesDifferent);
	PROTECTED_FIELD(mRightValuesDifferent);
	PROTECTED_FIELD(mTopValuesDifferent);
	PROTECTED_FIELD(mWidget);
	PROTECTED_FIELD(mLeftEditBox);
	PROTECTED_FIELD(mBottomEditBox);
	PROTECTED_FIELD(mRightEditBox);
	PROTECTED_FIELD(mTopEditBox);

	PUBLIC_FUNCTION(void, Setup, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Update);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, const RectI&);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(RectI, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, OnLeftEdited, const WString&);
	PROTECTED_FUNCTION(void, OnBottomEdited, const WString&);
	PROTECTED_FUNCTION(void, OnRightEdited, const WString&);
	PROTECTED_FUNCTION(void, OnTopEdited, const WString&);
}
END_META;
 