#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/CursorEventsArea.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIEditBox;
	class UIHorizontalLayout;
	class UIWidget;
}

namespace Editor
{
	// ----------------------------
	// Editor float border property
	// ----------------------------
	class BorderFProperty: public IPropertyField
	{
	public:
		// Default constructor
		BorderFProperty(UIWidget* widget = nullptr);

		// Destructor
		~BorderFProperty();

		// Sets fields
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Sets value
		void SetValue(const BorderF& value);

		// Sets value left
		void SetValueLeft(float value);

		// Sets value right
		void SetValueRight(float value);

		// Sets value top
		void SetValueTop(float value);

		// Sets value bottom
		void SetValueBottom(float value);

		// Sets value as unknown
		void SetUnknownValue(const BorderF& defaultValue = BorderF());

		// Sets value left as unknown
		void SetLeftUnknownValue(float defaultValue = 0.0f);

		// Sets value right as unknown
		void SetRightUnknownValue(float defaultValue = 0.0f);

		// Sets value top as unknown
		void SetTopUnknownValue(float defaultValue = 0.0f);

		// Sets value bottom as unknown
		void SetBottomUnknownValue(float defaultValue = 0.0f);

		// Returns value
		BorderF GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(BorderFProperty);

	protected:
		Function<void(void*, const BorderF&)> mAssignFunc;      // Value assign function
		Function<BorderF(void*)>              mGetFunc;         // Get value function

		Function<void(void*, float)>          mLeftAssignFunc;  // Left Value assign function
		Function<float(void*)>                mLeftGetFunc;     // Get left value function

		Function<void(void*, float)>          mRightAssignFunc; // Right Value assign function
		Function<float(void*)>                mRightGetFunc;    // Get right value function

		Function<void(void*, float)>          mTopAssignFunc;   // Top Value assign function
		Function<float(void*)>                mTopGetFunc;      // Get top value function

		Function<void(void*, float)>          mBottomAssignFunc; // Bottom Value assign function
		Function<float(void*)>                mBottomGetFunc;    // Get bottom value function

		TargetsVec mValuesPointers;                 // Fields' pointers
		BorderF    mCommonValue;                    // Common field value (if not different)
		bool       mLeftValuesDifferent = true;     // Are left values different
		bool       mBottomValuesDifferent = true;   // Are bottom values different
		bool       mRightValuesDifferent = true;    // Are right values different
		bool       mTopValuesDifferent = true;      // Are top values different
				   
		UIWidget*  mPropertyWidget = nullptr;       // Property root widget
		UIButton*  mRevertBtn = nullptr;            // Property revert button to prototype source
		UIEditBox* mLeftEditBox = nullptr;          // Left Edit box 
		UIEditBox* mBottomEditBox = nullptr;        // Bottom Edit box 
		UIEditBox* mRightEditBox = nullptr;         // Right Edit box 
		UIEditBox* mTopEditBox = nullptr;           // Top Edit box 

		CursorEventsArea mLeftDragHangle;           // Left Value changing drag handle
		CursorEventsArea mRightDragHangle;          // Right Value changing drag handle
		CursorEventsArea mTopDragHangle;            // Top Value changing drag handle
		CursorEventsArea mBottomDragHangle;         // Bottom Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(const BorderF& value);

		// Sets common value left
		void SetCommonValueLeft(float value);

		// Sets common value right
		void SetCommonValueRight(float value);

		// Sets common value top
		void SetCommonValueTop(float value);

		// Sets common value bottom
		void SetCommonValueBottom(float value);

		// Left Edit box change event
		void OnLeftEdited(const WString& data);

		// Bottom Edit box change event
		void OnBottomEdited(const WString& data);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// Right Edit box change event
		void OnRightEdited(const WString& data);

		// Top Edit box change event
		void OnTopEdited(const WString& data);

		// Returns value increasing multiplier by cursor delta
		float GetValueMultiplier(float delta) const;

		// It is called when drag handle was moved and changes the property value
		void OnLeftDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnRightDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnTopDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnBottomDragHandleMoved(const Input::Cursor& cursor);

		// It is called when key was released and changes focused value
		void OnKeyReleased(const Input::Key& key);

		// It is called when change left value move handle pressed, sets cursor infinite mode and stores value to data
		void OnLeftMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change left value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnLeftMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change right value move handle pressed, sets cursor infinite mode and stores value to data
		void OnRightMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change right value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnRightMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change top value move handle pressed, sets cursor infinite mode and stores value to data
		void OnTopMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change top value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnTopMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change bottom value move handle pressed, sets cursor infinite mode and stores value to data
		void OnBottomMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change bottom value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnBottomMoveHandleReleased(const Input::Cursor& cursor);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetLeftValueByUser(float value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetRightValueByUser(float value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetBottomValueByUser(float value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetTopValueByUser(float value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}

CLASS_BASES_META(Editor::BorderFProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::BorderFProperty)
{
	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mLeftAssignFunc);
	PROTECTED_FIELD(mLeftGetFunc);
	PROTECTED_FIELD(mRightAssignFunc);
	PROTECTED_FIELD(mRightGetFunc);
	PROTECTED_FIELD(mTopAssignFunc);
	PROTECTED_FIELD(mTopGetFunc);
	PROTECTED_FIELD(mBottomAssignFunc);
	PROTECTED_FIELD(mBottomGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mLeftValuesDifferent);
	PROTECTED_FIELD(mBottomValuesDifferent);
	PROTECTED_FIELD(mRightValuesDifferent);
	PROTECTED_FIELD(mTopValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mLeftEditBox);
	PROTECTED_FIELD(mBottomEditBox);
	PROTECTED_FIELD(mRightEditBox);
	PROTECTED_FIELD(mTopEditBox);
	PROTECTED_FIELD(mLeftDragHangle);
	PROTECTED_FIELD(mRightDragHangle);
	PROTECTED_FIELD(mTopDragHangle);
	PROTECTED_FIELD(mBottomDragHangle);
}
END_META;
CLASS_METHODS_META(Editor::BorderFProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, const BorderF&);
	PUBLIC_FUNCTION(void, SetValueLeft, float);
	PUBLIC_FUNCTION(void, SetValueRight, float);
	PUBLIC_FUNCTION(void, SetValueTop, float);
	PUBLIC_FUNCTION(void, SetValueBottom, float);
	PUBLIC_FUNCTION(void, SetUnknownValue, const BorderF&);
	PUBLIC_FUNCTION(void, SetLeftUnknownValue, float);
	PUBLIC_FUNCTION(void, SetRightUnknownValue, float);
	PUBLIC_FUNCTION(void, SetTopUnknownValue, float);
	PUBLIC_FUNCTION(void, SetBottomUnknownValue, float);
	PUBLIC_FUNCTION(BorderF, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const BorderF&);
	PROTECTED_FUNCTION(void, SetCommonValueLeft, float);
	PROTECTED_FUNCTION(void, SetCommonValueRight, float);
	PROTECTED_FUNCTION(void, SetCommonValueTop, float);
	PROTECTED_FUNCTION(void, SetCommonValueBottom, float);
	PROTECTED_FUNCTION(void, OnLeftEdited, const WString&);
	PROTECTED_FUNCTION(void, OnBottomEdited, const WString&);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnRightEdited, const WString&);
	PROTECTED_FUNCTION(void, OnTopEdited, const WString&);
	PROTECTED_FUNCTION(float, GetValueMultiplier, float);
	PROTECTED_FUNCTION(void, OnLeftDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnLeftMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnLeftMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetLeftValueByUser, float);
	PROTECTED_FUNCTION(void, SetRightValueByUser, float);
	PROTECTED_FUNCTION(void, SetBottomValueByUser, float);
	PROTECTED_FUNCTION(void, SetTopValueByUser, float);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
