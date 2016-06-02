#pragma once

#include "UI/Tree.h"

namespace o2
{
	class Actor;
	class UIToggleGroup;
}

using namespace o2;

namespace Editor
{
	class ActorProperty;
	class ComponentProperty;

	// ------------------
	// Actors tree widget
	// ------------------
	class UIActorsTree: public UITree
	{
	public:
		typedef Vector<Actor*> ActorsVec;

	public:
		Function<void(ActorsVec, Actor*, Actor*)> onDraggedActors;
		Function<void(UITreeNode*, Actor*)>       onItemClick;
		Function<void(UITreeNode*, Actor*)>       onItemDblClick;
		Function<void(UITreeNode*, Actor*)>       onItemRBClick;
		Function<void(ActorsVec)>                 onItemsSelectionChanged;

		// Default constructor
		UIActorsTree();

		// Copy-constructor
		UIActorsTree(const UIActorsTree& other);

		// Destructor
		~UIActorsTree();

		// Copy-operator
		UIActorsTree& operator=(const UIActorsTree& other);

		// Attaches to scene events
		void AttachToSceneEvents();

		// Updates tree node for object
		void UpdateNodeView(Actor* object);

		// Returns ui node for object
		UITreeNode* GetNode(Actor* object);

		// Forcible begins to dragging actors' nodes
		void ManualBeginDraggingActors(const ActorsVec& actors);

		// Updates manual actors dragging
		void ManualUpdateDraggingActors(const Input::Cursor& cursor);

		// Completed manual dragging actors
		void CompleteManualDraggingActors();

		// Breaks actors' nodes dragging
		void BreakDragging();

		// Returns selected objects vector
		ActorsVec GetSelectedActors() const;

		// Sets selected objects
		void SetSelectedActors(const ActorsVec& actors);

		// Selects object
		void SelectActor(Actor* object);

		// Selects object
		void SelectAndExpandActor(Actor* object);

		// Deselects object
		void DeselectActor(Actor* object);

		// Deselects all objects
		void DeselectAllActors();

		// Scrolls view to actor's tree node
		void ScrollTo(Actor* object);

		SERIALIZABLE(UIActorsTree);

	protected:
		UIToggleGroup*     mEnableActorsTogglesGroup;   // Enable actors toggles group
		UIToggleGroup*     mLockActorsTogglesGroup;	    // Lock actors toggles group
		bool               mAttackedToSceneEvents;      // Is tree attached to scene events
						    						    
		ActorProperty*     mDragActorPropertyField;     // Actor property field under cursor when dragging actor
		ComponentProperty* mDragComponentPropertyField; // Component property field under cursor when dragging actor

	protected:
		// Initializes widget logic
		void Initialize();

		// Returns actor's parent (For tree widget)
		UnknownType* GetActorsParent(UnknownType* obj);

		// Returns actor's children (For tree widget)
		Vector<UnknownType*> GetActorsChildren(UnknownType* parentObj);

		// Initializes tree node by actor (For tree widget)
		void SetupTreeNodeActor(UITreeNode* node, UnknownType* actorObj);

		// Moves actors to new parent and position, when they was dragged in tree (For tree widget)
		void RearrangeActors(Vector<UnknownType*> objects, UnknownType* parentObj, UnknownType* prevObj);

		// Calls when tree node double clicked (For tree widget)
		void OnTreeNodeDblClick(UITreeNode* node, Actor* actor);

		// Calls when enable actors toggle group pressed
		void EnableActorsGroupPressed(bool value);

		// Calls when enable actors toggle group released
		void EnableActorsGroupReleased(bool value);

		// Calls when lock actors toggle group pressed
		void LockActorsGroupPressed(bool value);

		// Calls when lock actors toggle group released
		void LockActorsGroupReleased(bool value);

		// Calls when actor was created
		void OnActorCreated(Actor* actor);

		// Calls when actor was destroyed
		void OnActorDestroyed(Actor* actor);
// 
// 		// Calls when some selectable drag listeners was dragged above this area
// 		void OnDraggedAbove(SelectableDragableObjectsGroup* group);
// 
// 		// Calls when some selectable drag listeners was dropped to this
// 		void OnDropped(SelectableDragableObjectsGroup* group);
	};
}