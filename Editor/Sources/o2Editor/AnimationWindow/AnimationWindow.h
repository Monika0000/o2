#pragma once

#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

using namespace o2;

namespace o2
{
	class AnimationClip;
	class Button;
	class EditBox;
	class HorizontalScrollBar;
	class Toggle;
	class WidgetDragHandle;
}

// Editor animation window accessor macros
#define o2EditorAnimationWindow AnimationWindow::Instance()

namespace Editor
{
	class AnimationTimeline;
	class AnimationTree;
	class CurvesSheet;
	class KeyHandlesSheet;

	// TODO
	// - �������� ����� ����� �����, ��������, �������
	// - �������� assign �������
	// - �������� "������" ��������� �����
	// - ������� �������� ������ ��� Vec2F
	// - ������������� Animation track � ��������� �� Track � Player
	// - ������ �� ��������

	class AnimationWindow : public IEditorWindow, public Singleton<AnimationWindow>
	{
		IOBJECT(AnimationWindow);

	public:
		// Default constructor
		AnimationWindow();

		// Destructor
		~AnimationWindow();

		// Updates window logic
		void Update(float dt) override;

		// Sets editing animation
		void SetAnimation(AnimationClip* animation, AnimationPlayer* player = nullptr);

		// Sets animation editable
		void SetAnimationEditable(IEditableAnimation* editable);

		// Sets target actor
		void SetTarget(ActorRef actor);

		// Sets curves or handles mode
		void SetCurvesMode(bool enabled);

		// Returns is curves mode enabled
		bool IsCurvesMode() const;

	protected:
		float mTreeViewWidth = 325.0f;    // Width of tree area. Changed by dragable separator
		float mMinTreeViewWidth = 250.0f; // Minimal tree width

		ActorRef         mTargetActor;         // Target actor on animation
		AnimationPlayer* mPlayer = nullptr;    // Animation player
		bool             mOwnPlayer = false;   // Is player owned by this
		AnimationClip*   mAnimation = nullptr; // Editing animation

		IEditableAnimation* mAnimationEditable = nullptr; // Editable animation holder. Deactivating when editing animation

		bool mDisableTimeTracking = false; // When true animation time changes has no effect


		Widget* mUpPanel = nullptr;  // Up panel with control buttons
		Widget* mWorkArea = nullptr; // Working area with tree and time line

		Widget*  mControlsPanel = nullptr;    // Panel with buttons described below
		Toggle*  mRecordToggle = nullptr;     // Record toggle
		Button*  mRewindLeft = nullptr;       // Rewind animation to start button
		Button*  mMoveLeft = nullptr;         // Move time one frame left
		Toggle*  mPlayPauseToggle = nullptr;  // Play - pause toggle
		Button*  mMoveRight = nullptr;        // Move time one frame right
		Button*  mRewindRight = nullptr;      // Rewind animation to end
		Toggle*  mLoopToggle = nullptr;       // Animation loop toggle
		Toggle*  mCurvesToggle = nullptr;     // Toggle curves view
		Button*  mAddKeyButton = nullptr;     // Add key on current time button
		Button*  mPropertiesButton = nullptr; // Open properties window

		AnimationTimeline*   mTimeline = nullptr;     // Animation time line
		HorizontalScrollBar* mTimeScroll = nullptr;   // Time line horizontal scrollbar
		AnimationTree*       mTree = nullptr;         // animation tracks tree
		KeyHandlesSheet*     mHandlesSheet = nullptr; // Animation keys handles sheet
		CurvesSheet*         mCurves = nullptr;       // Animation curves sheet

		WidgetDragHandle* mTreeSeparatorHandle = nullptr; // Tree separator handle. When it moves, it changes size of all dependent widgets

		ActionsList mActionsList; // List of actions in animation editor, also injecting into curves editor

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes handles sheet
		void InitializeHandlesSheet();

		// Initializes nodes tree
		void InitializeTree();

		// Initializes timeline and scrollbar
		void InitializeTimeline();

		// Initializes curves sheet widget
		void InitializeCurvesSheet();

		// Initializes up control panel
		void InitializeUpPanel();

		// Initializes separator handle view and events
		void InitializeSeparatorHandle();

		// It is called when editing animation changed. Invokes change methods in tree, curves etc
		void OnAnimationChanged();

		// It is called when animation has updated
		void OnAnimationUpdate(float time);

		// It is called when play/pause button was pressed
		void OnPlayPauseToggled(bool play);

		// It is called when loop button was pressed
		void OnLoopToggled(bool loop);

		// It is called when search edit box text was changed
		void OnSearchEdited(const WString& search);

		// It is called when menu filter button was pressed
		void OnMenuFilterPressed();

		// It is called when menu record button was pressed
		void OnMenuRecordPressed();

		friend class AnimationTimeline;
		friend class AnimationTree;
		friend class CurvesSheet;
		friend class KeyHandlesSheet;

		template<typename AnimationTrackType>
		friend class KeyFramesTrackControl;
	};
}

CLASS_BASES_META(Editor::AnimationWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
	BASE_CLASS(o2::Singleton<AnimationWindow>);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationWindow)
{
	PROTECTED_FIELD(mTreeViewWidth).DEFAULT_VALUE(325.0f);
	PROTECTED_FIELD(mMinTreeViewWidth).DEFAULT_VALUE(250.0f);
	PROTECTED_FIELD(mTargetActor);
	PROTECTED_FIELD(mPlayer).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mOwnPlayer).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mAnimation).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mAnimationEditable).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mDisableTimeTracking).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mUpPanel).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mWorkArea).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mControlsPanel).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mRecordToggle).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mRewindLeft).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mMoveLeft).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mPlayPauseToggle).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mMoveRight).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mRewindRight).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mLoopToggle).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mCurvesToggle).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mAddKeyButton).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mPropertiesButton).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mTimeline).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mTimeScroll).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mTree).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mHandlesSheet).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mCurves).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mTreeSeparatorHandle).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mActionsList);
}
END_META;
CLASS_METHODS_META(Editor::AnimationWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetAnimation, AnimationClip*, AnimationPlayer*);
	PUBLIC_FUNCTION(void, SetAnimationEditable, IEditableAnimation*);
	PUBLIC_FUNCTION(void, SetTarget, ActorRef);
	PUBLIC_FUNCTION(void, SetCurvesMode, bool);
	PUBLIC_FUNCTION(bool, IsCurvesMode);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeHandlesSheet);
	PROTECTED_FUNCTION(void, InitializeTree);
	PROTECTED_FUNCTION(void, InitializeTimeline);
	PROTECTED_FUNCTION(void, InitializeCurvesSheet);
	PROTECTED_FUNCTION(void, InitializeUpPanel);
	PROTECTED_FUNCTION(void, InitializeSeparatorHandle);
	PROTECTED_FUNCTION(void, OnAnimationChanged);
	PROTECTED_FUNCTION(void, OnAnimationUpdate, float);
	PROTECTED_FUNCTION(void, OnPlayPauseToggled, bool);
	PROTECTED_FUNCTION(void, OnLoopToggled, bool);
	PROTECTED_FUNCTION(void, OnSearchEdited, const WString&);
	PROTECTED_FUNCTION(void, OnMenuFilterPressed);
	PROTECTED_FUNCTION(void, OnMenuRecordPressed);
}
END_META;
