#include "RectangleFloatProperty.h"

#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"

namespace Editor
{
	RectFProperty::RectFProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mWidget = widget;
		else
			mWidget = o2UI.CreateWidget<UIWidget>("rectangle property");

		mLeftEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("left edit"));
		mBottomEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("bottom edit"));
		mRightEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("right edit"));
		mTopEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("top edit"));

		mLeftEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectFProperty::OnLeftEdited);
		mLeftEditBox->SetFilterFloat();

		mBottomEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectFProperty::OnBottomEdited);
		mBottomEditBox->SetFilterFloat();

		mRightEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectFProperty::OnRightEdited);
		mRightEditBox->SetFilterFloat();

		mTopEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectFProperty::OnTopEdited);
		mTopEditBox->SetFilterFloat();
	}

	RectFProperty::~RectFProperty()
	{
		delete mWidget;
	}

	void RectFProperty::SetValue(const RectF& value)
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

	void RectFProperty::SetUnknownValue()
	{
		mLeftValuesDifferent = true;
		mBottomValuesDifferent = true;
		mRightValuesDifferent = true;
		mTopValuesDifferent = true;

		mCommonValue = RectF();

		mLeftEditBox->text = "--";
		mBottomEditBox->text = "--";
		mRightEditBox->text = "--";
		mTopEditBox->text = "--";
	}

	void RectFProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const RectF& value) { *((Property<RectF>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<RectF>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const RectF& value) { *((RectF*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((RectF*)(ptr)); };
		}

		mValuesPointers = targets;

		Update();
	}

	void RectFProperty::Update()
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
			mCommonValue.left = 0.0f;
		}
		else mLeftEditBox->text = (WString)mCommonValue.left;

		if (mBottomValuesDifferent)
		{
			mBottomEditBox->text = "--";
			mCommonValue.bottom = 0.0f;
		}
		else mBottomEditBox->text = (WString)mCommonValue.bottom;

		if (mRightValuesDifferent)
		{
			mRightEditBox->text = "--";
			mCommonValue.right = 0.0f;
		}
		else mRightEditBox->text = (WString)mCommonValue.right;

		if (mTopValuesDifferent)
		{
			mTopEditBox->text = "--";
			mCommonValue.top = 0.0f;
		}
		else mTopEditBox->text = (WString)mCommonValue.top;
	}

	UIWidget* RectFProperty::GetWidget() const
	{
		return mWidget;
	}

	RectF RectFProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool RectFProperty::IsValuesDifferent() const
	{
		return mLeftValuesDifferent || mBottomValuesDifferent || mRightValuesDifferent || mTopValuesDifferent;
	}

	const Type* RectFProperty::GetFieldType() const
	{
		return &TypeOf(RectF);
	}

	void RectFProperty::OnLeftEdited(const WString& data)
	{
		if (mLeftValuesDifferent && data == "--")
			return;

		mCommonValue.left = (const float)data;
		mLeftValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		onChanged();
	}

	void RectFProperty::OnBottomEdited(const WString& data)
	{
		if (mBottomValuesDifferent && data == "--")
			return;

		mCommonValue.bottom = (const float)data;
		mBottomValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		onChanged();
	}

	void RectFProperty::OnRightEdited(const WString& data)
	{
		if (mRightValuesDifferent && data == "--")
			return;

		mCommonValue.right = (const float)data;
		mRightValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		onChanged();
	}

	void RectFProperty::OnTopEdited(const WString& data)
	{
		if (mTopValuesDifferent && data == "--")
			return;

		mCommonValue.top = (const float)data;
		mTopValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		onChanged();
	}
}

CLASS_META(Editor::RectFProperty)
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
	PUBLIC_FUNCTION(void, SetValue, const RectF&);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(RectF, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, OnLeftEdited, const WString&);
	PROTECTED_FUNCTION(void, OnBottomEdited, const WString&);
	PROTECTED_FUNCTION(void, OnRightEdited, const WString&);
	PROTECTED_FUNCTION(void, OnTopEdited, const WString&);
}
END_META;
 