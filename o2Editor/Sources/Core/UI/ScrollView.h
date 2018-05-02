#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/Camera.h"
#include "Render/TextureRef.h"
#include "UI/Widget.h"

using namespace o2;

namespace o2
{
	class Sprite;
}

namespace Editor
{
	// ----------------------------
	// Scrollable and zoomable view
	// ----------------------------
	class UIScrollView: public UIWidget, public CursorAreaEventsListener
	{
	public:
		// Default constructor
		UIScrollView();

		// Copy-constructor
		UIScrollView(const UIScrollView& other);

		// Destructor
		~UIScrollView();

		// Copy-operator
		UIScrollView& operator=(const UIScrollView& other);

		// Draws widget, updates render target 
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Transforms point from screen space to local space
		Vec2F ScreenToLocalPoint(const Vec2F& point);

		// Transforms point from local space to screen space
		Vec2F LocalToScreenPoint(const Vec2F& point);

		// Returns camera scale
		Vec2F GetCameraScale() const;

		// Returns view camera
		const Camera& GetCamera() const;

		// Sets back color
		void SetBackColor(const Color4& color);

		// Sets grid color
		void SetGridColor(const Color4& color);

		// Updates layout
		void UpdateTransform(bool withChildren = true) override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Returns is this widget can be focused
		bool IsFocusable() const override;

		SERIALIZABLE(UIScrollView);

	protected:
		bool       mReady = false;                         // Is widget initialized and ready to use
				   								           
		Sprite*    mRenderTargetSprite = nullptr;          // Render target sprite, using for caching graphics
		TextureRef mRenderTarget;                          // Render target texture, using for caching graphics
		bool       mNeedRedraw = false;                    // Is need to redraw render target
				   								           
		Color4     mBackColor;                             // Color of back @SERIALIZABLE
		Color4     mGridColor;                             // Color of grid @SERIALIZABLE

		RectF      mViewArea;                              // View area range
		Camera     mViewCamera;     					   // Scene view camera
		Vec2F      mViewCameraTargetScale = Vec2F(1, 1);   // Camera target scale
		float      mViewCameraScaleSence = 0.1f / 120.0f;  // Camera scale sense
		float      mViewCameraScaleElasticyCoef = 30.0f;   // Scale smoothing coefficient
		Vec2F      mViewCameraTargetPos;				   // Target camera position
		Vec2F      mViewCameraVelocity;					   // Camera velocity
		float      mViewCameraPosElasticyCoef = 40.0f;	   // Camera dragging smoothing coefficient
		float      mViewCameraVelocityDampingCoef = 10.0f; // Camera velocity damping coefficient
		float      mViewCameraMinScale = 0.001f;		   // Minimal camera scale
		float      mViewCameraMaxScale = 10000.0f;		   // Maximal camera scale
			       
		Basis      mLocalToScreenTransform;                 // Local to screen transformation
		Basis      mScreenToLocalTransform;                 // Screen to local transformation

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when transformation was changed and updated, updates render texture and sprite
		void OnTransformUpdated() override;

		// Updates transparency for this and children widgets
		void UpdateTransparency() override;

		// Updates camera
		void UpdateCamera(float dt);

		// Updates scene to screen and screen and scene transformations
		void UpdateLocalScreenTransforms();

		// Redraws content into render target
		void RedrawRenderTarget();

		// Redraws content into render target
		virtual void RedrawContent();

		// Draws grid
		virtual void DrawGrid();

		// It is called when camera position was changed
		virtual void OnCameraTransformChanged();

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		// It is called when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// It is called when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(Editor::UIScrollView)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::UIScrollView)
{
	PROTECTED_FIELD(mReady);
	PROTECTED_FIELD(mRenderTargetSprite);
	PROTECTED_FIELD(mRenderTarget);
	PROTECTED_FIELD(mNeedRedraw);
	PROTECTED_FIELD(mBackColor).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mGridColor).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mViewArea);
	PROTECTED_FIELD(mViewCamera);
	PROTECTED_FIELD(mViewCameraTargetScale);
	PROTECTED_FIELD(mViewCameraScaleSence);
	PROTECTED_FIELD(mViewCameraScaleElasticyCoef);
	PROTECTED_FIELD(mViewCameraTargetPos);
	PROTECTED_FIELD(mViewCameraVelocity);
	PROTECTED_FIELD(mViewCameraPosElasticyCoef);
	PROTECTED_FIELD(mViewCameraVelocityDampingCoef);
	PROTECTED_FIELD(mViewCameraMinScale);
	PROTECTED_FIELD(mViewCameraMaxScale);
	PROTECTED_FIELD(mLocalToScreenTransform);
	PROTECTED_FIELD(mScreenToLocalTransform);
}
END_META;
CLASS_METHODS_META(Editor::UIScrollView)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(Vec2F, ScreenToLocalPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, LocalToScreenPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetCameraScale);
	PUBLIC_FUNCTION(const Camera&, GetCamera);
	PUBLIC_FUNCTION(void, SetBackColor, const Color4&);
	PUBLIC_FUNCTION(void, SetGridColor, const Color4&);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, UpdateCamera, float);
	PROTECTED_FUNCTION(void, UpdateLocalScreenTransforms);
	PROTECTED_FUNCTION(void, RedrawRenderTarget);
	PROTECTED_FUNCTION(void, RedrawContent);
	PROTECTED_FUNCTION(void, DrawGrid);
	PROTECTED_FUNCTION(void, OnCameraTransformChanged);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
}
END_META;
