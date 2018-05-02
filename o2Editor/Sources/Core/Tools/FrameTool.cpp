#include "stdafx.h"
#include "FrameTool.h"

#include "Core/Actions/ActorsTransform.h"
#include "Core/EditorApplication.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"

namespace Editor
{
	FrameTool::FrameTool()
	{
		mLeftTopRotateHandle.SetRegularSprite(mnew Sprite("ui/UI3_rotate_regular.png"));
		mLeftTopRotateHandle.SetHoverSprite(mnew Sprite("ui/UI3_rotate_hover.png"));
		mLeftTopRotateHandle.SetPressedSprite(mnew Sprite("ui/UI3_rotate_pressed.png"));
		mLeftTopRotateHandle.GetRegularSprite()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.GetHoverSprite()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.GetPressedSprite()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.pixelPerfect = false;

		mLeftBottomRotateHandle = mLeftTopRotateHandle;
		mRightTopRotateHandle = mLeftTopRotateHandle;
		mRightBottomRotateHandle = mLeftTopRotateHandle;

		mLeftTopHandle.SetRegularSprite(mnew Sprite("ui/UI2_handle_regular.png"));
		mLeftTopHandle.SetHoverSprite(mnew Sprite("ui/UI2_handle_select.png"));
		mLeftTopHandle.SetPressedSprite(mnew Sprite("ui/UI2_handle_pressed.png"));
		mLeftTopHandle.pixelPerfect = false;

		mLeftBottomHandle = mLeftTopHandle;
		mRightTopHandle = mLeftTopHandle;
		mRightBottomHandle = mLeftTopHandle;

		mLeftHandle.SetRegularSprite(mnew Sprite("ui/UI2_handle_side_regular.png"));
		mLeftHandle.SetHoverSprite(mnew Sprite("ui/UI2_handle_side_select.png"));
		mLeftHandle.SetPressedSprite(mnew Sprite("ui/UI2_handle_side_pressed.png"));
		mLeftHandle.pixelPerfect = false;

		mTopHandle = mLeftHandle;
		mBottomHandle = mLeftHandle;
		mRightHandle = mLeftHandle;

		mPivotHandle.SetRegularSprite(mnew Sprite("ui/UI2_pivot.png"));
		mPivotHandle.SetHoverSprite(mnew Sprite("ui/UI2_pivot_select.png"));
		mPivotHandle.SetPressedSprite(mnew Sprite("ui/UI2_pivot_pressed.png"));
		mPivotHandle.checkSnappingFunc = THIS_FUNC(CheckPivotSnapping);

		mAnchorsLeftTopHandle.SetRegularSprite(mnew Sprite("ui/UI3_anchor_pressed.png"));
		mAnchorsLeftTopHandle.SetHoverSprite(mnew Sprite("ui/UI3_anchor_hover.png"));
		mAnchorsLeftTopHandle.SetPressedSprite(mnew Sprite("ui/UI3_anchor_regular.png"));
		mAnchorsLeftTopHandle.GetRegularSprite()->pivot = Vec2F(1, 0);
		mAnchorsLeftTopHandle.GetHoverSprite()->pivot = Vec2F(1, 0);
		mAnchorsLeftTopHandle.GetPressedSprite()->pivot = Vec2F(1, 0);

		mAnchorsRightBottomHandle = mAnchorsLeftTopHandle;
		mAnchorsLeftBottomHandle = mAnchorsLeftTopHandle;
		mAnchorsRightTopHandle = mAnchorsLeftTopHandle;

		auto centerAnchorsRegularSprite = mnew Sprite();
		centerAnchorsRegularSprite->SetSize(Vec2F(0, 0));
		mAnchorsCenter.SetRegularSprite(centerAnchorsRegularSprite);
		mAnchorsCenter.SetHoverSprite(mnew Sprite("ui/UI3_anchors_hover.png"));
		mAnchorsCenter.SetPressedSprite(mnew Sprite("ui/UI3_anchors_pressed.png"));

		mLeftTopHandle.onChangedPos = THIS_FUNC(OnLeftTopHandle);
		mLeftHandle.onChangedPos = THIS_FUNC(OnLeftHandle);
		mLeftBottomHandle.onChangedPos = THIS_FUNC(OnLeftBottomHandle);
		mTopHandle.onChangedPos = THIS_FUNC(OnTopHandle);
		mBottomHandle.onChangedPos = THIS_FUNC(OnBottomHandle);
		mRightTopHandle.onChangedPos = THIS_FUNC(OnRightTopHandle);
		mRightHandle.onChangedPos = THIS_FUNC(OnRightHandle);
		mRightBottomHandle.onChangedPos = THIS_FUNC(OnRightBottomHandle);
		mPivotHandle.onChangedPos = THIS_FUNC(OnPivotHandle);
		mLeftTopRotateHandle.onChangedPos = THIS_FUNC(OnLeftTopRotateHandle);
		mLeftBottomRotateHandle.onChangedPos = THIS_FUNC(OnLeftBottomRotateHandle);
		mRightTopRotateHandle.onChangedPos = THIS_FUNC(OnRightTopRotateHandle);
		mRightBottomRotateHandle.onChangedPos = THIS_FUNC(OnRightBottomRotateHandle);
		mAnchorsLeftTopHandle.onChangedPos = THIS_FUNC(OnAnchorLeftTopHandle);
		mAnchorsRightBottomHandle.onChangedPos = THIS_FUNC(OnAnchorRightBottomHandle);
		mAnchorsLeftBottomHandle.onChangedPos = THIS_FUNC(OnAnchorLeftBottomHandle);
		mAnchorsRightTopHandle.onChangedPos = THIS_FUNC(OnAnchorRightTopHandle);
		mAnchorsCenter.onChangedPos = THIS_FUNC(OnCenterAnchorHandle);

		mLeftTopHandle.onPressed = THIS_FUNC(HandlePressed);
		mLeftHandle.onPressed = THIS_FUNC(HandlePressed);
		mLeftBottomHandle.onPressed = THIS_FUNC(HandlePressed);
		mTopHandle.onPressed = THIS_FUNC(HandlePressed);
		mBottomHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightTopHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightBottomHandle.onPressed = THIS_FUNC(HandlePressed);
		mPivotHandle.onPressed = THIS_FUNC(HandlePressed);
		mLeftTopRotateHandle.onPressed = THIS_FUNC(HandlePressed);
		mLeftBottomRotateHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightTopRotateHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightBottomRotateHandle.onPressed = THIS_FUNC(HandlePressed);
		mAnchorsLeftTopHandle.onPressed = THIS_FUNC(HandlePressed);
		mAnchorsRightBottomHandle.onPressed = THIS_FUNC(HandlePressed);
		mAnchorsLeftBottomHandle.onPressed = THIS_FUNC(HandlePressed);
		mAnchorsRightTopHandle.onPressed = THIS_FUNC(HandlePressed);
		mAnchorsCenter.onPressed = THIS_FUNC(HandlePressed);

		mLeftTopHandle.onReleased = THIS_FUNC(HandleReleased);
		mLeftHandle.onReleased = THIS_FUNC(HandleReleased);
		mLeftBottomHandle.onReleased = THIS_FUNC(HandleReleased);
		mTopHandle.onReleased = THIS_FUNC(HandleReleased);
		mBottomHandle.onReleased = THIS_FUNC(HandleReleased);
		mRightTopHandle.onReleased = THIS_FUNC(HandleReleased);
		mRightHandle.onReleased = THIS_FUNC(HandleReleased);
		mRightBottomHandle.onReleased = THIS_FUNC(HandleReleased);
		mPivotHandle.onReleased = THIS_FUNC(HandleReleased);
		mLeftTopRotateHandle.onReleased = THIS_FUNC(HandleReleased);
		mLeftBottomRotateHandle.onReleased = THIS_FUNC(HandleReleased);
		mRightTopRotateHandle.onReleased = THIS_FUNC(HandleReleased);
		mRightBottomRotateHandle.onReleased = THIS_FUNC(HandleReleased);
		mAnchorsLeftTopHandle.onReleased = THIS_FUNC(HandleReleased);
		mAnchorsRightBottomHandle.onReleased = THIS_FUNC(HandleReleased);
		mAnchorsLeftBottomHandle.onReleased = THIS_FUNC(HandleReleased);
		mAnchorsRightTopHandle.onReleased = THIS_FUNC(HandleReleased);
		mAnchorsCenter.onReleased = THIS_FUNC(HandleReleased);

		mTopHandle.checkSnappingFunc = THIS_FUNC(CheckTopSnapping);
		mLeftHandle.checkSnappingFunc = THIS_FUNC(CheckLeftSnapping);
		mBottomHandle.checkSnappingFunc = THIS_FUNC(CheckBottomSnapping);
		mRightHandle.checkSnappingFunc = THIS_FUNC(CheckRightSnapping);
		mLeftTopHandle.checkSnappingFunc = THIS_FUNC(CheckLeftTopSnapping);
		mLeftBottomHandle.checkSnappingFunc = THIS_FUNC(CheckLeftBottomSnapping);
		mRightTopHandle.checkSnappingFunc = THIS_FUNC(CheckRightTopSnapping);
		mRightBottomHandle.checkSnappingFunc = THIS_FUNC(CheckRightBottomSnapping);

		mAnchorsRightBottomHandle.checkSnappingFunc = THIS_FUNC(CheckAnchorRightBottomSnapping);
		mAnchorsLeftBottomHandle.checkSnappingFunc = THIS_FUNC(CheckAnchorLeftBottomSnapping);
		mAnchorsRightTopHandle.checkSnappingFunc = THIS_FUNC(CheckAnchorRightTopSnapping);
		mAnchorsLeftTopHandle.checkSnappingFunc = THIS_FUNC(CheckAnchorLeftTopSnapping);
		mAnchorsCenter.checkSnappingFunc = THIS_FUNC(CheckAnchorCenterSnapping);

		mTopHandle.isPointInside = THIS_FUNC(IsPointInTopHandle);
		mBottomHandle.isPointInside = THIS_FUNC(IsPointInBottomHandle);
		mLeftHandle.isPointInside = THIS_FUNC(IsPointInLeftHandle);
		mRightHandle.isPointInside = THIS_FUNC(IsPointInRightHandle);
		mAnchorsCenter.isPointInside = THIS_FUNC(IsPointInAnchorsCenterHandle);

		SetHandlesEnable(false);
	}

	FrameTool::~FrameTool()
	{}

	void FrameTool::DrawScene()
	{
		SelectionTool::DrawScene();

		if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
		{
			for (auto actor : o2EditorSceneScreen.GetSelectedActors())
				o2Render.DrawAABasis(actor->transform->GetWorldBasis(), mActorColor, mActorColor, mActorColor);

			o2Render.DrawAABasis(mFrame, mFrameColor, mFrameColor, mFrameColor);

			if (mAnchorsFrameEnabled)
			{
				for (auto actor : o2EditorSceneScreen.GetSelectedActors())
				{
					if (actor->GetParent())
						o2Render.DrawAABasis(actor->GetParent()->transform->GetWorldBasis(), mParentColor, mParentColor, mParentColor);
				}

				o2Render.DrawAABasis(mAnchorsFrame, mAnchorsFrameColor, mAnchorsFrameColor, mAnchorsFrameColor, 1.0f,
									 LineType::Dash);
			}
		}

		DrawSnapLines();
	}

	void FrameTool::DrawSnapLines()
	{
		for (auto& line : mSnapLines)
			o2Render.DrawAALine(line.begin, line.end, line.color);
	}

	void FrameTool::OnEnabled()
	{
		UpdateSelectionFrame();
	}

	void FrameTool::OnDisabled()
	{
		SetHandlesEnable(false);
	}

	void FrameTool::OnSceneChanged(Vector<Actor*> changedActors)
	{
		mNeedRedraw = true;
		if (mChangedFromThis)
			mChangedFromThis = false;
		else
			UpdateSelectionFrame();
	}

	void FrameTool::OnActorsSelectionChanged(Vector<Actor*> actors)
	{
		UpdateSelectionFrame();
	}

	void FrameTool::OnKeyPressed(const Input::Key& key)
	{
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		if (key == VK_LEFT)
			TransformActorsWithAction(Basis::Translated(Vec2F::Left()));

		if (key == VK_RIGHT)
			TransformActorsWithAction(Basis::Translated(Vec2F::Right()));

		if (key == VK_UP)
			TransformActorsWithAction(Basis::Translated(Vec2F::Up()));

		if (key == VK_DOWN)
			TransformActorsWithAction(Basis::Translated(Vec2F::Down()));

		SelectionTool::OnKeyPressed(key);
	}

	void FrameTool::OnKeyStayDown(const Input::Key& key)
	{
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		if (key.pressedTime < 0.3f)
			return;

		if (key == VK_LEFT)
			TransformActorsWithAction(Basis::Translated(Vec2F::Left()));

		if (key == VK_RIGHT)
			TransformActorsWithAction(Basis::Translated(Vec2F::Right()));

		if (key == VK_UP)
			TransformActorsWithAction(Basis::Translated(Vec2F::Up()));

		if (key == VK_DOWN)
			TransformActorsWithAction(Basis::Translated(Vec2F::Down()));
	}

	void FrameTool::OnKeyReleased(const Input::Key& key)
	{}

	void FrameTool::TransformActors(const Basis& transform)
	{
		for (auto actor : o2EditorSceneScreen.GetTopSelectedActors())
		{
			if (actor->transform->size != Vec2F())
				actor->transform->SetWorldBasis(actor->transform->GetWorldBasis()*transform);
			else
				actor->transform->SetWorldNonSizedBasis(actor->transform->GetWorldNonSizedBasis()*transform);

			actor->UpdateTransform();
		}

		mChangedFromThis = true;

		UpdateSelectionFrame();
		UpdateHandlesTransform();
	}

	void FrameTool::TransformActorsWithAction(const Basis& transform)
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return *x->transform; });

		TransformActors(transform);

		auto action = mnew ActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);
	}

	void FrameTool::TransformAnchorsActors(const Basis& transform)
	{
		RectF anchorsFrame(transform.origin, transform.origin + Vec2F(transform.xv.Length(), transform.yv.Length()));

		for (auto actor : o2EditorSceneScreen.GetTopSelectedActors())
		{
			auto widget = dynamic_cast<UIWidget*>(actor);
			if (widget)
			{
				auto parent = widget->GetParent();
				auto parentWidget = dynamic_cast<UIWidget*>(parent);

				if (parent)
				{
					RectF parentWorldRect;

					if (parentWidget)
						parentWorldRect = parentWidget->GetChildrenRect();
					else
						parentWorldRect = parent->transform->GetWorldRect();

					RectF prevWorldRect = widget->layout->GetWorldRect();
					widget->layout->SetAnchorMin((anchorsFrame.LeftBottom() - parentWorldRect.LeftBottom())/parentWorldRect.Size());
					widget->layout->SetAnchorMax((anchorsFrame.RightTop() - parentWorldRect.LeftBottom())/parentWorldRect.Size());
					widget->layout->SetWorldRect(prevWorldRect);

					actor->UpdateTransform();
				}
			}
		}

		mChangedFromThis = true;

		UpdateSelectionFrame();
		UpdateHandlesTransform();
	}

	void FrameTool::UpdateSelectionFrame()
	{
		auto selectedActors = o2EditorSceneScreen.GetSelectedActors();

		mAnchorsFrameEnabled = false;

		if (selectedActors.Count() == 1)
		{
			mFrame = selectedActors[0]->transform->GetWorldBasis();
			mPivotHandle.position = selectedActors[0]->transform->GetWorldPivot();

			UIWidget* widget = dynamic_cast<UIWidget*>(selectedActors[0]);
			mAnchorsFrameEnabled = widget != nullptr && widget->GetParent() != nullptr;

			if (mAnchorsFrameEnabled)
			{
				auto parent = widget->GetParent();
				auto parentWidget = dynamic_cast<UIWidget*>(parent);

				RectF parentWorldRect;

				if (parentWidget)
					parentWorldRect = parentWidget->GetChildrenRect();
				else
					parentWorldRect = parent->transform->GetWorldRect();

				RectF worldRectangle(parentWorldRect.LeftBottom() + widget->layout->GetAnchorMin()*parentWorldRect.Size(),
									 parentWorldRect.LeftBottom() + widget->layout->GetAnchorMax()*parentWorldRect.Size());

				mAnchorsFrame.origin = worldRectangle.LeftBottom();
				mAnchorsFrame.xv = Vec2F(Math::Max(worldRectangle.Width(), 0.001f), 0);
				mAnchorsFrame.yv = Vec2F(0, Math::Max(worldRectangle.Height(), 0.001f));
			}
		}
		else if (selectedActors.Count() > 0)
		{
			Basis frameBasis = selectedActors.Last()->transform->GetWorldNonSizedBasis();
			Vec2F frameOrigin = frameBasis.origin;
			Vec2F xAxis = frameBasis.xv.Normalized();
			Vec2F yAxis = frameBasis.yv.Normalized();
			Vec2F sx, sy;

			const Vec2F cp[4] ={ Vec2F(0, 0), Vec2F(0, 1), Vec2F(1, 0), Vec2F(1, 1) };
			for (auto actor : selectedActors)
			{
				Basis actorTransform = actor->transform->GetWorldBasis();
				for (int i = 0; i < 4; i++)
				{
					Vec2F wp = cp[i] * actorTransform;
					float px = (wp - frameOrigin).Dot(xAxis);
					float py = (wp - frameOrigin).Dot(yAxis);

					sx.x = Math::Min(sx.x, px);
					sx.y = Math::Max(sx.y, px);
					sy.x = Math::Min(sy.x, py);
					sy.y = Math::Max(sy.y, py);
				}
			}

			mFrame.Set(frameOrigin + xAxis*sx.x + yAxis*sy.x, xAxis*(sx.y - sx.x), yAxis*(sy.y - sy.x));
			mPivotHandle.position = mFrame.origin + mFrame.xv*0.5f + mFrame.yv*0.5f;
		}
		else
		{
			SetHandlesEnable(false);
			return;
		}

		SetHandlesEnable(true);
		UpdateHandlesTransform();
	}

	void FrameTool::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
		{
			if (mFrame.IsPointInside(o2EditorSceneScreen.ScreenToScenePoint(cursor.position)))
			{
				mIsDragging = true;
				mBeginDraggingFrame = mFrame;
				mBeginDraggingOffset = o2EditorSceneScreen.ScreenToLocalPoint(cursor.position) - mFrame.origin;

				SetHandlesEnable(false);
				HandlePressed();
				return;
			}
		}

		SelectionTool::OnCursorPressed(cursor);
	}

	void FrameTool::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mIsDragging = false;
			SetHandlesEnable(true);
			HandleReleased();
		}
		else SelectionTool::OnCursorReleased(cursor);
	}

	void FrameTool::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mIsDragging = false;
			SetHandlesEnable(true);
		}
		else SelectionTool::OnCursorPressBreak(cursor);
	}

	void FrameTool::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mSnapLines.Clear();

			Vec2F cursorPos = o2EditorSceneScreen.ScreenToLocalPoint(cursor.position);

			if (o2Input.IsKeyDown(VK_CONTROL))
			{
				Basis preTransformed(cursorPos - mBeginDraggingOffset, mBeginDraggingFrame.xv, mBeginDraggingFrame.yv);

				cursorPos = CalculateSnapOffset(cursorPos, preTransformed,
				{ Vec2F(0, 0), Vec2F(0, 1), Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(1, 0), Vec2F(1, 1) }, preTransformed.xv.Normalized(),
				{ Vec2F(0, 0), Vec2F(1, 0), Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, 1), Vec2F(1, 1) }, preTransformed.yv.Normalized(),
												o2Scene.GetAllActors());
			}

			Vec2F delta = cursorPos - mBeginDraggingOffset;
			Basis transformed(delta, mBeginDraggingFrame.xv, mBeginDraggingFrame.yv);

			if (o2Input.IsKeyDown(VK_SHIFT))
			{
				if (Math::Abs(transformed.origin.x - mBeginDraggingFrame.origin.x) > Math::Abs(transformed.origin.y - mBeginDraggingFrame.origin.y))
					transformed.origin.y = mBeginDraggingFrame.origin.y;
				else
					transformed.origin.x = mBeginDraggingFrame.origin.x;
			}

			TransformActors(mFrame.Inverted()*transformed);
		}
		else SelectionTool::OnCursorStillDown(cursor);
	}

	void FrameTool::OnLeftTopHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetLeftTopHandleTransformed(position));
	}

	void FrameTool::OnLeftHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetLeftHandleTransformed(position));
	}

	void FrameTool::OnLeftBottomHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetLeftBottomHandleTransformed(position));
	}

	void FrameTool::OnTopHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetTopHandleTransformed(position));
	}

	void FrameTool::OnBottomHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetBottomHandleTransformed(position));
	}

	void FrameTool::OnRightTopHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetRightTopHandleTransformed(position));
	}

	void FrameTool::OnRightHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetRightHandleTransformed(position));
	}

	void FrameTool::OnRightBottomHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetRightBottomHandleTransformed(position));
	}

	void FrameTool::OnAnchorLeftTopHandle(const Vec2F& position)
	{
		TransformAnchorsActors(GetLeftTopAnchorHandleTransformed(position));
	}

	void FrameTool::OnAnchorLeftBottomHandle(const Vec2F& position)
	{
		TransformAnchorsActors(GetLeftBottomAnchorHandleTransformed(position));
	}

	void FrameTool::OnAnchorRightTopHandle(const Vec2F& position)
	{
		TransformAnchorsActors(GetRightTopAnchorHandleTransformed(position));
	}

	void FrameTool::OnAnchorRightBottomHandle(const Vec2F& position)
	{
		TransformAnchorsActors(GetRightBottomAnchorHandleTransformed(position));
	}

	void FrameTool::OnCenterAnchorHandle(const Vec2F& position)
	{
		TransformAnchorsActors(GetAnchorsCenterHandleTransformed(position));
	}

	void FrameTool::OnPivotHandle(const Vec2F& position)
	{
		auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
		if (selectedActors.Count() == 1)
		{
			selectedActors[0]->transform->SetWorldPivot(position);
		}
	}

	void FrameTool::OnLeftTopRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameTool::OnLeftBottomRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameTool::OnRightTopRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameTool::OnRightBottomRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameTool::OnRotateHandle(const Vec2F& position, Vec2F lastHandleCoords)
	{
		Vec2F rotatePivot = mPivotHandle.position;
		float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
		Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
		Basis transformed = mFrame*transform;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float angle = Math::Rad2deg(transformed.GetAngle());
			float step = 15.0f;
			float targetAngle = Math::Round(angle/step)*step;
			float delta = targetAngle - angle;

			Basis deltaTransform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(Math::Deg2rad(delta))
				*Basis::Translated(rotatePivot);

			transformed = transformed*deltaTransform;
		}

		TransformActors(mFrame.Inverted()*transformed);
	}

	void FrameTool::SetHandlesEnable(bool enable)
	{
		mPivotHandle.enabled = enable;
		mLeftTopRotateHandle.enabled = enable;
		mLeftBottomRotateHandle.enabled = enable;
		mRightTopRotateHandle.enabled = enable;
		mRightBottomRotateHandle.enabled = enable;
		mLeftTopHandle.enabled = enable;
		mLeftHandle.enabled = enable;
		mLeftBottomHandle.enabled = enable;
		mTopHandle.enabled = enable;
		mBottomHandle.enabled = enable;
		mRightTopHandle.enabled = enable;
		mRightHandle.enabled = enable;
		mRightBottomHandle.enabled = enable;

		mAnchorsLeftBottomHandle.enabled = enable && mAnchorsFrameEnabled;
		mAnchorsLeftTopHandle.enabled = enable && mAnchorsFrameEnabled;
		mAnchorsRightTopHandle.enabled = enable && mAnchorsFrameEnabled;
		mAnchorsRightBottomHandle.enabled = enable && mAnchorsFrameEnabled;

		CheckAnchorsCenterEnabled();
	}

	void FrameTool::UpdateHandlesTransform()
	{
		float handlesAngle = mFrame.GetAngle();

		auto getHandleType = [&](const Vec2F& pos) {
			Vec2F v = pos - Vec2F(0.5f, 0.5f)*mFrame;
			float angle = Math::Rad2deg(v.Angle(Vec2F::Right()));

			if (angle < 22.5f || angle > 337.5f)
				return CursorType::SizeWE;
			else if (angle > 22.5f && angle < 67.5f)
				return CursorType::SizeNwSe;
			else if (angle > 67.5f && angle < 112.5f)
				return CursorType::SizeNS;
			else if (angle > 112.5f && angle < 157.5f)
				return CursorType::SizeNeSw;
			else if (angle > 157.5f && angle < 202.5f)
				return CursorType::SizeWE;
			else if (angle > 202.5f && angle < 247.5f)
				return CursorType::SizeNwSe;
			else if (angle > 247.5f && angle < 292.5f)
				return CursorType::SizeNS;
			else if (angle > 292.5f && angle < 337.5f)
				return CursorType::SizeNeSw;

			return CursorType::SizeNeSw;
		};

		mLeftTopHandle.position = Vec2F(0.0f, 1.0f)*mFrame;
		mLeftHandle.position = Vec2F(0.0f, 0.5f)*mFrame;
		mLeftBottomHandle.position = Vec2F(0.0f, 0.0f)*mFrame;
		mTopHandle.position = Vec2F(0.5f, 1.0f)*mFrame;
		mBottomHandle.position = Vec2F(0.5f, 0.0f)*mFrame;
		mRightTopHandle.position = Vec2F(1.0f, 1.0f)*mFrame;
		mRightHandle.position = Vec2F(1.0f, 0.5f)*mFrame;
		mRightBottomHandle.position = Vec2F(1.0f, 0.0f)*mFrame;

		mLeftTopHandle.cursorType = getHandleType(mLeftTopHandle.GetPosition());
		mLeftHandle.cursorType = getHandleType(mLeftHandle.GetPosition());
		mLeftBottomHandle.cursorType = getHandleType(mLeftBottomHandle.GetPosition());
		mTopHandle.cursorType = getHandleType(mTopHandle.GetPosition());
		mBottomHandle.cursorType = getHandleType(mBottomHandle.GetPosition());
		mRightTopHandle.cursorType = getHandleType(mRightTopHandle.GetPosition());
		mRightHandle.cursorType = getHandleType(mRightHandle.GetPosition());
		mRightBottomHandle.cursorType = getHandleType(mRightBottomHandle.GetPosition());

		mLeftTopRotateHandle.position = Vec2F(0.0f, 1.0f)*mFrame;
		mLeftBottomRotateHandle.position = Vec2F(0.0f, 0.0f)*mFrame;
		mRightTopRotateHandle.position = Vec2F(1.0f, 1.0f)*mFrame;
		mRightBottomRotateHandle.position = Vec2F(1.0f, 0.0f)*mFrame;

		mLeftTopHandle.angle = handlesAngle + Math::PI()*0.5f;
		mLeftHandle.angle = handlesAngle + Math::PI();
		mLeftBottomHandle.angle = handlesAngle + Math::PI();
		mTopHandle.angle = handlesAngle + Math::PI()*0.5f;
		mBottomHandle.angle = handlesAngle - Math::PI()*0.5f;
		mRightTopHandle.angle = handlesAngle;
		mRightHandle.angle = handlesAngle;
		mRightBottomHandle.angle = handlesAngle - Math::PI()*0.5f;

		mLeftTopRotateHandle.angle = handlesAngle + Math::PI()*0.5f;
		mLeftBottomRotateHandle.angle = handlesAngle + Math::PI();
		mRightTopRotateHandle.angle = handlesAngle;
		mRightBottomRotateHandle.angle = handlesAngle + Math::PI()*1.5f;

		if (mAnchorsFrameEnabled)
		{
			mAnchorsLeftTopHandle.position = Vec2F(0.0f, 1.0f)*mAnchorsFrame;
			mAnchorsLeftBottomHandle.position = Vec2F(0.0f, 0.0f)*mAnchorsFrame;
			mAnchorsRightTopHandle.position = Vec2F(1.0f, 1.0f)*mAnchorsFrame;
			mAnchorsRightBottomHandle.position = Vec2F(1.0f, 0.0f)*mAnchorsFrame;
			mAnchorsCenter.position = mAnchorsFrame.origin;

			mAnchorsLeftTopHandle.angle = handlesAngle;
			mAnchorsLeftBottomHandle.angle = handlesAngle + Math::PI()*0.5f;
			mAnchorsRightTopHandle.angle = handlesAngle + Math::PI()*1.5f;
			mAnchorsRightBottomHandle.angle = handlesAngle + Math::PI();

			CheckAnchorsCenterEnabled();
		}

		mNeedRedraw = true;
	}

	void FrameTool::HandlePressed()
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return *x->transform; });

		mBeginDraggingFrame = mFrame;
	}

	void FrameTool::HandleReleased()
	{
		auto action = mnew ActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);

		mSnapLines.Clear();
	}

	Basis FrameTool::GetLeftTopHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.xv;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			- xn*Math::Max(0.0f, xn.Dot(position - correctOrigin) + mFrameMinimalSize)
			+ yn*Math::Max(0.0f, -yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.origin += frameDeltaX;
		transformedFrame.xv -= frameDeltaX;
		transformedFrame.yv += frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
			{
				Vec2F xd = transformedFrame.xv.Normalized()*
					(transformedFrame.yv.Length()*aspect - transformedFrame.xv.Length());

				transformedFrame.origin -= xd;
				transformedFrame.xv += xd;
			}
			else transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetLeftHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.xv;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F correctedPos = position
			- xn*Math::Max(0.0f, xn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.5f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);

		transformedFrame.origin += frameDeltaX;
		transformedFrame.xv -= frameDeltaX;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetLeftBottomHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.xv + mFrame.yv;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			- xn*Math::Max(0.0f, xn.Dot(position - correctOrigin) + mFrameMinimalSize)
			- yn*Math::Max(0.0f, yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.origin += frameDeltaX + frameDeltaY;
		transformedFrame.xv -= frameDeltaX;
		transformedFrame.yv -= frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
			{
				Vec2F xd = transformedFrame.xv.Normalized()*
					(transformedFrame.yv.Length()*aspect - transformedFrame.xv.Length());

				transformedFrame.origin -= xd;
				transformedFrame.xv += xd;
			}
			else
			{
				Vec2F yd = transformedFrame.yv.Normalized()*
					(transformedFrame.xv.Length()/aspect - transformedFrame.yv.Length());

				transformedFrame.origin -= yd;
				transformedFrame.yv += yd;
			}
		}

		return transformedFrame;
	}

	Basis FrameTool::GetTopHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin;
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			+ yn*Math::Max(0.0f, -yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.5f, 1.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.yv += frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetBottomHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.yv;
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			- yn*Math::Max(0.0f, yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.5f, 0.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.origin += frameDeltaY;
		transformedFrame.yv -= frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetRightTopHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			+ xn*Math::Max(0.0f, -xn.Dot(position - correctOrigin) + mFrameMinimalSize)
			+ yn*Math::Max(0.0f, -yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.xv += frameDeltaX;
		transformedFrame.yv += frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
				transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
			else
				transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetRightHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F correctedPos = position
			+ xn*Math::Max(0.0f, -xn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.5f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);

		transformedFrame.xv += frameDeltaX;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetRightBottomHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.yv;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			+ xn*Math::Max(0.0f, -xn.Dot(position - correctOrigin) + mFrameMinimalSize)
			- yn*Math::Max(0.0f, yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.origin += frameDeltaY;
		transformedFrame.xv += frameDeltaX;
		transformedFrame.yv -= frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
				transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
			else
			{
				Vec2F yd = transformedFrame.yv.Normalized()*
					(transformedFrame.xv.Length()/aspect - transformedFrame.yv.Length());

				transformedFrame.origin -= yd;
				transformedFrame.yv += yd;
			}
		}

		return transformedFrame;
	}

	Basis FrameTool::GetLeftTopAnchorHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mAnchorsFrame.origin + mAnchorsFrame.xv;
		Vec2F xn = mAnchorsFrame.xv.Normalized();
		Vec2F yn = mAnchorsFrame.yv.Normalized();
		Vec2F correctedPos = position
			- xn*Math::Max(0.0f, xn.Dot(position - correctOrigin))
			+ yn*Math::Max(0.0f, -yn.Dot(position - correctOrigin));

		Basis transformedFrame = mAnchorsFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mAnchorsFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mAnchorsFrame.xv);
		Vec2F frameDeltaY = delta.Project(mAnchorsFrame.yv);

		transformedFrame.origin += frameDeltaX;
		transformedFrame.xv -= frameDeltaX;
		transformedFrame.yv += frameDeltaY;

		return transformedFrame;
	}

	Basis FrameTool::GetLeftBottomAnchorHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mAnchorsFrame.origin + mAnchorsFrame.xv + mAnchorsFrame.yv;
		Vec2F xn = mAnchorsFrame.xv.Normalized();
		Vec2F yn = mAnchorsFrame.yv.Normalized();
		Vec2F correctedPos = position
			- xn*Math::Max(0.0f, xn.Dot(position - correctOrigin))
			- yn*Math::Max(0.0f, yn.Dot(position - correctOrigin));

		Basis transformedFrame = mAnchorsFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mAnchorsFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mAnchorsFrame.xv);
		Vec2F frameDeltaY = delta.Project(mAnchorsFrame.yv);

		transformedFrame.origin += frameDeltaX + frameDeltaY;
		transformedFrame.xv -= frameDeltaX;
		transformedFrame.yv -= frameDeltaY;

		return transformedFrame;
	}

	Basis FrameTool::GetRightTopAnchorHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mAnchorsFrame.origin;
		Vec2F xn = mAnchorsFrame.xv.Normalized();
		Vec2F yn = mAnchorsFrame.yv.Normalized();
		Vec2F correctedPos = position
			+ xn*Math::Max(0.0f, -xn.Dot(position - correctOrigin))
			+ yn*Math::Max(0.0f, -yn.Dot(position - correctOrigin));

		Basis transformedFrame = mAnchorsFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mAnchorsFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mAnchorsFrame.xv);
		Vec2F frameDeltaY = delta.Project(mAnchorsFrame.yv);

		transformedFrame.xv += frameDeltaX;
		transformedFrame.yv += frameDeltaY;

		return transformedFrame;
	}

	Basis FrameTool::GetRightBottomAnchorHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mAnchorsFrame.origin + mAnchorsFrame.yv;
		Vec2F xn = mAnchorsFrame.xv.Normalized();
		Vec2F yn = mAnchorsFrame.yv.Normalized();
		Vec2F correctedPos = position
			+ xn*Math::Max(0.0f, -xn.Dot(position - correctOrigin))
			- yn*Math::Max(0.0f, yn.Dot(position - correctOrigin));

		Basis transformedFrame = mAnchorsFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mAnchorsFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mAnchorsFrame.xv);
		Vec2F frameDeltaY = delta.Project(mAnchorsFrame.yv);

		transformedFrame.origin += frameDeltaY;
		transformedFrame.xv += frameDeltaX;
		transformedFrame.yv -= frameDeltaY;

		return transformedFrame;
	}

	Basis FrameTool::GetAnchorsCenterHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mAnchorsFrame;
		transformedFrame.origin = position;

		return transformedFrame;
	}

	Vec2F FrameTool::CheckFrameSnapping(const Vec2F& point, const Basis& frame)
	{
		mSnapLines.Clear();

		static Vector<Vec2F> snapPoints =
		{
			Vec2F(0.0f, 0.0f), Vec2F(0.0f, 0.5f), Vec2F(0.0f, 1.0f),
			Vec2F(0.5f, 0.0f), Vec2F(0.5f, 0.5f), Vec2F(0.5f, 1.0f),
			Vec2F(1.0f, 0.0f), Vec2F(1.0f, 0.5f), Vec2F(1.0f, 1.0f)
		};

		static Vector<Vector<Vector<Vec2F>>> snapPointsLines =
		{
			{ { Vec2F(0.0f, 0.0f), Vec2F(0.5f, 0.0f) },{ Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.0f) } },
			{ { Vec2F(0.0f, 0.25f), Vec2F(0.0f, 0.75f) },{ Vec2F(0.0f, 0.5f), Vec2F(0.25f, 0.5f) } },
			{ { Vec2F(0.0f, 1.0f), Vec2F(0.5f, 1.0f) },{ Vec2F(0.0f, 1.0f), Vec2F(0.0f, 0.5f) } },

			{ { Vec2F(0.25f, 0.0f), Vec2F(0.75f, 0.0f) },{ Vec2F(0.5f, 0.0), Vec2F(0.5f, 0.25f) } },
			{ { Vec2F(0.25f, 0.5f), Vec2F(0.75f, 0.5f) },{ Vec2F(0.5f, 0.25f), Vec2F(0.5f, 0.75f) } },
			{ { Vec2F(0.25f, 1.0f), Vec2F(0.75f, 1.0f) },{ Vec2F(0.5f, 1.0f), Vec2F(0.5f, 0.75f) } },

			{ { Vec2F(0.5f, 0.0f), Vec2F(1.0f, 0.0f) },{ Vec2F(1.0f, 0.0f), Vec2F(1.0f, 0.5f) } },
			{ { Vec2F(1.0f, 0.25f), Vec2F(1.0f, 0.75f) },{ Vec2F(1.0f, 0.5f), Vec2F(0.75f, 0.5f) } },
			{ { Vec2F(1.0f, 1.0f), Vec2F(0.5f, 1.0f) },{ Vec2F(1.0f, 1.0f), Vec2F(1.0f, 0.5f) } }
		};

		const float snapThresholdPx = 5.0f;
		Vec2F screenpoint = o2EditorSceneScreen.SceneToScreenPoint(point);

		int i = 0;
		for (auto snapPoint : snapPoints)
		{
			Vec2F framePoint = o2EditorSceneScreen.LocalToScreenPoint(snapPoint*frame);

			if ((screenpoint - framePoint).Length() < snapThresholdPx)
			{
				mSnapLines.Add(SnapLine(snapPointsLines[i][0][0]*frame, snapPointsLines[i][0][1]*frame, mSnapLinesColor));
				mSnapLines.Add(SnapLine(snapPointsLines[i][1][0]*frame, snapPointsLines[i][1][1]*frame, mSnapLinesColor));

				return o2EditorSceneScreen.ScreenToLocalPoint(framePoint);
			}

			i++;
		}

		return point;
	}

	Vec2F FrameTool::CheckPivotSnapping(const Vec2F& point)
	{
		return CheckFrameSnapping(point, mFrame);
	}

	Vec2F FrameTool::CheckAnchorCenterSnapping(const Vec2F& point)
	{
		auto actors = o2EditorSceneScreen.GetTopSelectedActors();

		if (actors.Count() > 0 && actors[0]->GetParent())
			return CheckFrameSnapping(point, actors[0]->GetParent()->transform->GetWorldBasis());

		return point;
	}

	Vec2F FrameTool::CheckAnchorLeftTopSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		auto actors = o2EditorSceneScreen.GetTopSelectedActors();
		if (actors.Count() > 0 && actors[0]->GetParent())
		{
			Basis transformedFrame = GetLeftTopAnchorHandleTransformed(point);

			Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
			{ Vec2F(0, 0), Vec2F(0, 1) }, transformedFrame.xv.Normalized(),
			{ Vec2F(0, 1), Vec2F(1, 1) }, transformedFrame.yv.Normalized(), { actors[0]->GetParent() });

			return snapped;
		}

		return point;
	}

	Vec2F FrameTool::CheckAnchorLeftBottomSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		auto actors = o2EditorSceneScreen.GetTopSelectedActors();
		if (actors.Count() > 0 && actors[0]->GetParent())
		{
			Basis transformedFrame = GetLeftBottomAnchorHandleTransformed(point);

			Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
			{ Vec2F(0, 0), Vec2F(0, 1) }, transformedFrame.xv.Normalized(),
			{ Vec2F(0, 0), Vec2F(1, 0) }, transformedFrame.yv.Normalized(), { actors[0]->GetParent() });

			return snapped;
		}

		return point;
	}

	Vec2F FrameTool::CheckAnchorRightTopSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		auto actors = o2EditorSceneScreen.GetTopSelectedActors();
		if (actors.Count() > 0 && actors[0]->GetParent())
		{
			Basis transformedFrame = GetRightTopAnchorHandleTransformed(point);

			Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
			{ Vec2F(1, 0), Vec2F(1, 1) }, transformedFrame.xv.Normalized(),
			{ Vec2F(0, 1), Vec2F(1, 1) }, transformedFrame.yv.Normalized(), { actors[0]->GetParent() });

			return snapped;
		}

		return point;
	}

	Vec2F FrameTool::CheckAnchorRightBottomSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		auto actors = o2EditorSceneScreen.GetTopSelectedActors();
		if (actors.Count() > 0 && actors[0]->GetParent())
		{
			Basis transformedFrame = GetRightBottomAnchorHandleTransformed(point);

			Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
			{ Vec2F(1, 0), Vec2F(1, 1) }, transformedFrame.xv.Normalized(),
			{ Vec2F(0, 0), Vec2F(1, 0) }, transformedFrame.yv.Normalized(), { actors[0]->GetParent() });

			return snapped;
		}

		return point;
	}

	Vec2F FrameTool::CheckTopSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		Basis transformedFrame = GetTopHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{}, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 1), Vec2F(1, 1) }, transformedFrame.yv.Normalized(), o2Scene.GetAllActors());

		return snapped;
	}

	Vec2F FrameTool::CheckBottomSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		Basis transformedFrame = GetBottomHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{}, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 0), Vec2F(1, 0) }, transformedFrame.yv.Normalized(), o2Scene.GetAllActors());

		return snapped;
	}

	Vec2F FrameTool::CheckLeftSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		Basis transformedFrame = GetLeftHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(0, 0), Vec2F(0, 1) }, transformedFrame.xv.Normalized(),
		{}, transformedFrame.yv.Normalized(), o2Scene.GetAllActors());

		return snapped;
	}

	Vec2F FrameTool::CheckRightSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		Basis transformedFrame = GetRightHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(1, 0), Vec2F(1, 1) }, transformedFrame.xv.Normalized(),
		{}, transformedFrame.yv.Normalized(), o2Scene.GetAllActors());

		return snapped;
	}

	Vec2F FrameTool::CheckLeftTopSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		Basis transformedFrame = GetLeftTopHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(0, 0), Vec2F(0, 1) }, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 1), Vec2F(1, 1) }, transformedFrame.yv.Normalized(), o2Scene.GetAllActors());

		return snapped;
	}

	Vec2F FrameTool::CheckLeftBottomSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		Basis transformedFrame = GetLeftBottomHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(0, 0), Vec2F(0, 1) }, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 0), Vec2F(1, 0) }, transformedFrame.yv.Normalized(), o2Scene.GetAllActors());

		return snapped;
	}

	Vec2F FrameTool::CheckRightTopSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		Basis transformedFrame = GetRightTopHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(1, 0), Vec2F(1, 1) }, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 1), Vec2F(1, 1) }, transformedFrame.yv.Normalized(), o2Scene.GetAllActors());

		return snapped;
	}

	Vec2F FrameTool::CheckRightBottomSnapping(const Vec2F& point)
	{
		mSnapLines.Clear();

		Basis transformedFrame = GetRightBottomHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(1, 0), Vec2F(1, 1) }, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 0), Vec2F(1, 0) }, transformedFrame.yv.Normalized(), o2Scene.GetAllActors());

		return snapped;
	}

	bool FrameTool::IsPointInTopHandle(const Vec2F& point)
	{
		float camScale = o2EditorSceneScreen.GetCameraScale().x;
		float spriteSize = mLeftTopHandle.GetRegularSprite()->GetSize().x*camScale;
		Basis transformNonScaled(mFrame.origin, mFrame.xv.Normalized(), mFrame.yv.Normalized());
		Basis b(mFrame.origin + mFrame.yv + transformNonScaled.xv*spriteSize*0.5f - transformNonScaled.yv*spriteSize*0.5f,
				mFrame.xv - transformNonScaled.xv*spriteSize,
				transformNonScaled.yv*spriteSize);

		return b.IsPointInside(o2EditorSceneScreen.ScreenToLocalPoint(point));
	}

	bool FrameTool::IsPointInLeftHandle(const Vec2F& point)
	{
		float camScale = o2EditorSceneScreen.GetCameraScale().x;
		float spriteSize = mLeftTopHandle.GetRegularSprite()->GetSize().x*camScale;
		Basis transformNonScaled(mFrame.origin, mFrame.xv.Normalized(), mFrame.yv.Normalized());
		Basis b(mFrame.origin - transformNonScaled.xv*spriteSize*0.5f + transformNonScaled.yv*spriteSize*0.5f,
				transformNonScaled.xv*spriteSize,
				mFrame.yv - transformNonScaled.yv*spriteSize);

		return b.IsPointInside(o2EditorSceneScreen.ScreenToLocalPoint(point));
	}

	bool FrameTool::IsPointInRightHandle(const Vec2F& point)
	{
		float camScale = o2EditorSceneScreen.GetCameraScale().x;
		float spriteSize = mLeftTopHandle.GetRegularSprite()->GetSize().x*camScale;
		Basis transformNonScaled(mFrame.origin, mFrame.xv.Normalized(), mFrame.yv.Normalized());
		Basis b(mFrame.origin + mFrame.xv - transformNonScaled.xv*spriteSize*0.5f + transformNonScaled.yv*spriteSize*0.5f,
				transformNonScaled.xv*spriteSize,
				mFrame.yv - transformNonScaled.yv*spriteSize);

		return b.IsPointInside(o2EditorSceneScreen.ScreenToLocalPoint(point));
	}

	bool FrameTool::IsPointInBottomHandle(const Vec2F& point)
	{
		float camScale = o2EditorSceneScreen.GetCameraScale().x;
		float spriteSize = mLeftTopHandle.GetRegularSprite()->GetSize().x*camScale;
		Basis transformNonScaled(mFrame.origin, mFrame.xv.Normalized(), mFrame.yv.Normalized());
		Basis b(mFrame.origin + transformNonScaled.xv*spriteSize*0.5f - transformNonScaled.yv*spriteSize*0.5f,
				mFrame.xv - transformNonScaled.xv*spriteSize,
				transformNonScaled.yv*spriteSize);

		return b.IsPointInside(o2EditorSceneScreen.ScreenToLocalPoint(point));
	}

	bool FrameTool::IsPointInAnchorsCenterHandle(const Vec2F& point)
	{
		float camScale = o2EditorSceneScreen.GetCameraScale().x;
		float handleSize = 10.0f;
		return (o2EditorSceneScreen.ScreenToLocalPoint(point) - mAnchorsFrame.origin).Length() < handleSize*camScale;
	}

	void FrameTool::CheckAnchorsCenterEnabled()
	{
		float lengthThreshold = 0.1f;
		bool enabled = mAnchorsFrame.xv.Length() < lengthThreshold && mAnchorsFrame.yv.Length() < lengthThreshold;
		mAnchorsCenter.enabled = false;
		mAnchorsCenter.enabled = enabled && mAnchorsFrameEnabled;
	}

	Vec2F FrameTool::CalculateSnapOffset(const Vec2F& point, const Basis& frame,
										 const Vector<Vec2F>& xLines, const Vec2F& xNormal,
										 const Vector<Vec2F>& yLines, const Vec2F& yNormal,
										 const Vector<Actor*>& actors)
	{
		const float pxThreshold = 5.0f;
		const float sameSnapDistanceThreshold = 0.01f;
		const Camera& camera = o2EditorSceneScreen.GetCamera();

		Vector<Vector<Vec2F>> snapLines =
		{
			{ Vec2F(0.0f, 0.0f), Vec2F(1.0f, 0.0f) },
			{ Vec2F(1.0f, 0.0f), Vec2F(1.0f, 1.0f) },
			{ Vec2F(1.0f, 1.0f), Vec2F(0.0f, 1.0f) },
			{ Vec2F(0.0f, 1.0f), Vec2F(0.0f, 0.0f) },
			{ Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f) },
			{ Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f) }
		};

		Vector<Vec2F> worldSnapLines;
		for (auto actor : actors)
		{
			if (o2EditorSceneScreen.GetSelectedActors().Contains(actor))
				continue;

			Basis actorBasis = actor->transform->GetWorldBasis();

			for (auto snapLine : snapLines)
			{
				Vec2F actorLineBegin = snapLine[0]*actorBasis;
				Vec2F actorLineEnd = snapLine[1]*actorBasis;
				Vec2F actorLineVec = actorLineEnd - actorLineBegin;

				if (actorLineVec.SqrLength() < 0.1f)
					continue;

				worldSnapLines.Add(actorLineBegin);
				worldSnapLines.Add(actorLineEnd);
			}
		}

		Vec2F offset;
		Vector<Vec2F> localSnapLines;

		for (int i = 0; i < 2; i++)
		{
			const Vector<Vec2F>& currentAxisLines = i == 0 ? xLines : yLines;
			Vec2F currentAxisNormal = i == 0 ? xNormal : yNormal;

			bool axisSnapLineFound = false;
			Vec2F axisSnapOffset;
			float axisSnapDistance = FLT_MAX;
			Vec2F axisSnapBegin, axisSnapEnd;

			for (int j = 0; j < currentAxisLines.Count(); j += 2)
			{
				Vec2F lineBeg = currentAxisLines[j]*frame;
				Vec2F lineEnd = currentAxisLines[j + 1]*frame;
				Vec2F lineVec = lineEnd - lineBeg;

				for (int k = 0; k < worldSnapLines.Count(); k += 2)
				{
					Vec2F actorLineBegin = worldSnapLines[k];
					Vec2F actorLineEnd = worldSnapLines[k + 1];
					Vec2F actorLineVec = actorLineEnd - actorLineBegin;

					if (!lineVec.IsParallel(actorLineVec))
						continue;

					float projDistance = (actorLineBegin - lineBeg).Dot(currentAxisNormal);
					float screenProjDistance = Math::Abs(projDistance/camera.GetScale().x);

					if (screenProjDistance < pxThreshold && screenProjDistance < axisSnapDistance)
					{
						axisSnapOffset = currentAxisNormal*projDistance;
						axisSnapDistance = screenProjDistance;
						axisSnapBegin = currentAxisLines[j];
						axisSnapEnd = currentAxisLines[j + 1];

						axisSnapLineFound = true;
					}
				}
			}

			if (!axisSnapLineFound)
				continue;

			for (int j = 0; j < currentAxisLines.Count(); j += 2)
			{
				Vec2F lineBeg = currentAxisLines[j]*frame + axisSnapOffset;
				Vec2F lineEnd = currentAxisLines[j + 1]*frame + axisSnapOffset;
				Vec2F lineVec = lineEnd - lineBeg;

				bool found = false;

				for (int k = 0; k < worldSnapLines.Count(); k += 2)
				{
					Vec2F actorLineBegin = worldSnapLines[k];
					Vec2F actorLineEnd = worldSnapLines[k + 1];
					Vec2F actorLineVec = actorLineEnd - actorLineBegin;

					if (!lineVec.IsParallel(actorLineVec))
						continue;

					float projDistance = Math::Abs((actorLineBegin - lineBeg).Dot(currentAxisNormal));

					if (projDistance < sameSnapDistanceThreshold)
					{
						mSnapLines.Add(SnapLine(actorLineBegin, actorLineEnd, mSnapLinesColor));

						found = true;
					}
				}

				if (found)
				{
					localSnapLines.Add(currentAxisLines[j]);
					localSnapLines.Add(currentAxisLines[j + 1]);
				}
			}

			offset += axisSnapOffset;
		}

		Basis frameWithOffset = frame;
		frameWithOffset.Translate(offset);

		for (int i = 0; i < localSnapLines.Count(); i += 2)
			mSnapLines.Add(SnapLine(localSnapLines[i]*frameWithOffset, localSnapLines[i + 1]*frameWithOffset, mSnapLinesColor));

		return point + offset;
	}

}

DECLARE_CLASS(Editor::FrameTool);
