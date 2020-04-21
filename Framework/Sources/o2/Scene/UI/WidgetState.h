#pragma once
#include "o2/Animation/AnimationClip.h"

#include "o2/Animation/AnimationPlayer.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Widget;

	// -----------------------------------------------------------------------
	// Widget state. Could be true or false, and animates widget by this state
	// -----------------------------------------------------------------------
	class WidgetState: public ISerializable
	{
	public:
		String name; // State name @SERIALIZABLE

		AnimationPlayer player;                        // Animation player
		float           offStateAnimationSpeed = 1.0f; // False state transition animation speed @SERIALIZABLE

	public:
		PROPERTIES(WidgetState);
		PROPERTY(bool, state, SetState, GetState); // State flag property

		PROPERTY(AnimationAssetRef, animationAsset, SetAnimationAsset, GetAnimationAsset); // Animation asset property @EDITOR_IGNORE
		PROPERTY(AnimationClip, animationClip, SetAnimationClip, GetAnimationClip);        // animation clip property @EDITOR_IGNORE

	public:
		Function<void()> onStateFullyTrue;	  // This event calls when state is completely true (at the end of animation)
		Function<void()> onStateFullyFalse;   // This event calls when state is completely false (at the end of animation)
		Function<void()> onStateBecomesTrue;  // This event calls when state becomes to true
		Function<void()> onStateBecomesFalse; // This event calls when state becomes to true

	public:
		// Default constructor
		WidgetState();

		// Copy-constructor
		WidgetState(const WidgetState& state);

		// Destructor
		~WidgetState();

		// Boolean cast operator
		operator bool();

		// Assign from boolean operator
		WidgetState& operator=(bool state);

		// Sets owner widget
		void SetOwner(Widget* owner, bool errors);

		// Sets animation asset
		void SetAnimationAsset(const AnimationAssetRef& asset);

		// Returns animation asset
		const AnimationAssetRef& GetAnimationAsset() const;

		// Sets animation asset instance clip
		void SetAnimationClip(const AnimationClip& animation);

		// Returns animation asset instance clip, if exists
		AnimationClip& GetAnimationClip();

		// Sets current state
		void SetState(bool state);

		// Sets state immediately
		void SetStateForcible(bool state);

		// Returns current state
		bool GetState() const;

		// Updates animation
		void Update(float dt);

		SERIALIZABLE(WidgetState);

	protected:
		bool    mState = false;   // Current state @SERIALIZABLE
		Widget* mOwner = nullptr; // Owner widget pointer

		AnimationAssetRef mAnimation; // Widget animation @SERIALIZABLE @EDITOR_PROPERTY

		friend class Widget;
	};
}

CLASS_BASES_META(o2::WidgetState)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::WidgetState)
{
	PUBLIC_FIELD(name).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(player);
	PUBLIC_FIELD(offStateAnimationSpeed).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(state);
	PUBLIC_FIELD(animationAsset).EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(animationClip).EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(onStateFullyTrue);
	PUBLIC_FIELD(onStateFullyFalse);
	PUBLIC_FIELD(onStateBecomesTrue);
	PUBLIC_FIELD(onStateBecomesFalse);
	PROTECTED_FIELD(mState).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mOwner);
	PROTECTED_FIELD(mAnimation).EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::WidgetState)
{

	PUBLIC_FUNCTION(void, SetOwner, Widget*, bool);
	PUBLIC_FUNCTION(void, SetAnimationAsset, const AnimationAssetRef&);
	PUBLIC_FUNCTION(const AnimationAssetRef&, GetAnimationAsset);
	PUBLIC_FUNCTION(void, SetAnimationClip, const AnimationClip&);
	PUBLIC_FUNCTION(AnimationClip&, GetAnimationClip);
	PUBLIC_FUNCTION(void, SetState, bool);
	PUBLIC_FUNCTION(void, SetStateForcible, bool);
	PUBLIC_FUNCTION(bool, GetState);
	PUBLIC_FUNCTION(void, Update, float);
}
END_META;
