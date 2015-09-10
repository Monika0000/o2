#include "Render/Render.h"

#include "Application/BaseApplication.h"
#include "Render/Mesh.h"
#include "Render/Texture.h"
#include "Utils/Debug.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	DECLARE_SINGLETON(Render);

	Render::Render():
		mReady(false), mStencilDrawing(false), mStencilTest(false), mScissorTest(false)
	{
		// TODO: initialize max buffers size

		// Create log stream
		mLog = mnew LogStream("Render");
		Debug.GetLog()->BindStream(mLog);

		// Initialize OpenGL
		mLog->Out("Initializing OpenGL render..");

		mResolution = o2Application.GetContentSize();

		GLuint pixelFormat;
		static	PIXELFORMATDESCRIPTOR pfd= // pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
			1,							   // Version Number
			PFD_DRAW_TO_WINDOW |		   // Format Must Support Window
			PFD_SUPPORT_OPENGL |		   // Format Must Support OpenGL
			PFD_DOUBLEBUFFER,			   // Must Support Double Buffering
			PFD_TYPE_RGBA,				   // Request An RGBA Format
			32,  						   // Select Our Color Depth
			0, 0, 0, 0, 0, 0,			   // Color Bits Ignored
			0,							   // No Alpha Buffer
			0,							   // Shift Bit Ignored
			0,							   // No Accumulation Buffer
			0, 0, 0, 0,					   // Accumulation Bits Ignored
			16,							   // 16Bit Z-Buffer (Depth Buffer)  
			1,							   // No Stencil Buffer
			0,							   // No Auxiliary Buffer
			PFD_MAIN_PLANE,				   // Main Drawing Layer
			0,							   // Reserved
			0, 0, 0						   // Layer Masks Ignored
		};

		mHDC = GetDC(o2Application.mHWnd);
		if (!mHDC)
		{
			mLog->Error("Can't Create A GL Device Context.\n");
			return;
		}

		pixelFormat = ChoosePixelFormat(mHDC, &pfd);
		if (!pixelFormat)
		{
			mLog->Error("Can't Find A Suitable PixelFormat.\n");
			return;
		}

		if (!SetPixelFormat(mHDC, pixelFormat, &pfd))
		{
			mLog->Error("Can't Set The PixelFormat.\n");
			return;
		}

		mGLContext = wglCreateContext(mHDC);
		if (!mGLContext)
		{
			mLog->Error("Can't Create A GL Rendering Context.\n");
			return;
		}

		if (!wglMakeCurrent(mHDC, mGLContext))
		{
			mLog->Error("Can't Activate The GL Rendering Context.\n");
			return;
		}

		// Get OpenGL extensions
		GetGLExtensions(mLog);

		GL_CHECK_ERROR(mLog);

		// Check compatibles
		CheckCompatibles();

		// Initialize buffers
		mVertexData = new UInt8[mVertexBufferSize*sizeof(Vertex2)];

		mVertexIndexData = new UInt16[mIndexBufferSize];
		mLastDrawVertex = 0;
		mTrianglesCount = 0;
		mCurrentPrimitiveType = GL_TRIANGLES;

		// Configure OpenGL
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex2), mVertexData + sizeof(float)*3);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex2), mVertexData + sizeof(float)*3 + sizeof(unsigned long));
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex2), mVertexData + 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLineWidth(1.0f);

		GL_CHECK_ERROR(mLog);

		mLog->Out("GL_VENDOR: %s", glGetString(GL_VENDOR));
		mLog->Out("GL_RENDERER: %s", glGetString(GL_RENDERER));
		mLog->Out("GL_VERSION: %s", glGetString(GL_VERSION));

		mCurrentRenderTarget = nullptr;
		mReady = true;
	}

	Render::~Render()
	{
		if (!mReady)
			return;

		if (mGLContext)
		{
			//RemoveAllTextures();

			if (!wglMakeCurrent(NULL, NULL))
			{
				mLog->Error("Release Of DC And RC Failed.\n");
			}

			if (!wglDeleteContext(mGLContext))
			{
				mLog->Error("Release Rendering Context Failed.\n");
			}

			mGLContext = NULL;
		}

		mReady = false;
	}

	void Render::CheckCompatibles()
	{
		//check render targets available
		char* extensions[] ={"GL_ARB_framebuffer_object", "GL_EXT_framebuffer_object", "GL_EXT_framebuffer_blit",
			"GL_EXT_packed_depth_stencil"};

		mRenderTargetsAvailable = true;
		for (int i = 0; i < 4; i++)
		{
			if (!IsGLExtensionSupported(extensions[i]))
				mRenderTargetsAvailable = false;
		}

		//get max texture size
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize.x);
		mMaxTextureSize.y = mMaxTextureSize.x;
	}

	void Render::Begin()
	{
		if (!mReady)
			return;

		// Reset batching params
		mLastDrawTexture      = NULL;
		mLastDrawVertex       = 0;
		mLastDrawIdx          = 0;
		mTrianglesCount       = 0;
		mFrameTrianglesCount  = 0;
		mDIPCount             = 0;
		mCurrentPrimitiveType = GL_TRIANGLES;

		// Reset view matrices
		SetupViewMatrix(mResolution);

		UpdateCameraTransforms();

		Clear();
	}

	void Render::DrawPrimitives()
	{
		if (mLastDrawVertex < 1)
			return;

		glDrawElements(mCurrentPrimitiveType, mLastDrawIdx, GL_UNSIGNED_SHORT, mVertexIndexData);

		GL_CHECK_ERROR(mLog);

		mFrameTrianglesCount += mTrianglesCount;
		mLastDrawVertex = mTrianglesCount = mLastDrawIdx = 0;

		mDIPCount++;
	}

	void Render::SetupViewMatrix(const Vec2I& viewSize)
	{
		float projMat[16];
		Math::OrthoProjMatrix(projMat, 0.0f, (float)viewSize.x, (float)viewSize.y, 0.0f, 0.0f, 10.0f);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, viewSize.x, viewSize.y);
		glLoadMatrixf(projMat);
	}

	void Render::End()
	{
		if (!mReady)
			return;

		DrawPrimitives();
		SwapBuffers(mHDC);

		GL_CHECK_ERROR(mLog);
	}

	void Render::Clear(const Color4& color /*= Color4::Black()*/)
	{
		glClearColor(color.RF(), color.GF(), color.BF(), color.AF());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GL_CHECK_ERROR(mLog);
	}

	Vec2I Render::GetResolution() const
	{
		return mResolution;
	}

	void Render::SetCamera(const Camera& camera)
	{
		mCamera = camera;
		UpdateCameraTransforms();
	}

	Camera Render::GetCamera() const
	{
		return mCamera;
	}

	void Render::UpdateCameraTransforms()
	{
		DrawPrimitives();

		glMatrixMode(GL_MODELVIEW);
		float modelMatrix[16] ={
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, -1, 1};

		// 		float cs = cosf(-mCamera->mRotation), sn = sinf(-mCurrentCamera->mRotation);
		// 		Vec2F scale(1.0f/mCurrentCamera->mScale.x, 1.0f/mCurrentCamera->mScale.y);
		// 		Vec2F pivotOffset = mCurrentCamera->mPivot.scale(mResolution).scale(mCurrentCamera->mScale).rotate(mCurrentCamera->mRotation);
		// 		Vec2F offs = mCurrentCamera->mPosition - pivotOffset;
		// 		float ofx = -offs.x*scale.x, ofy = -offs.y*scale.y;
		// 
		// 		modelMatrix[0] = cs*scale.x;  modelMatrix[1] = sn*scale.x;
		// 		modelMatrix[4] = -sn*scale.y; modelMatrix[5] = cs*scale.y;
		// 		modelMatrix[12] = cs*ofx - sn*ofy;     modelMatrix[13] = sn*ofx + cs*ofy;

		glLoadMatrixf(modelMatrix);
	}

	void Render::DrawLine(const Vec2F& a, const Vec2F& b, const Color4 color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] ={Vertex2(a.x, a.y, dcolor, 0, 0), Vertex2(b.x, b.y, dcolor, 0, 0)};
		DrawLines(v, 1);
	}

	void Render::DrawRectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4 color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] ={
			Vertex2(minp.x, minp.y, dcolor, 0, 0), Vertex2(maxp.x, minp.y, dcolor, 0, 0),
			Vertex2(maxp.x, minp.y, dcolor, 0, 0), Vertex2(maxp.x, maxp.y, dcolor, 0, 0),
			Vertex2(maxp.x, maxp.y, dcolor, 0, 0), Vertex2(minp.x, maxp.y, dcolor, 0, 0),
			Vertex2(minp.x, maxp.y, dcolor, 0, 0), Vertex2(minp.x, minp.y, dcolor, 0, 0)
		};
		DrawLines(v, 4);
	}

	void Render::DrawCross(const Vec2F& pos, float size /*= 5*/, const Color4 color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] ={
			Vertex2(pos.x - size, pos.y, dcolor, 0, 0), Vertex2(pos.x + size, pos.y, dcolor, 0, 0),
			Vertex2(pos.x, pos.y - size, dcolor, 0, 0), Vertex2(pos.x, pos.y + size, dcolor, 0, 0)};
		DrawLines(v, 2);
	}

	void Render::DrawCircle(const Vec2F& pos, float radius /*= 5*/, const Color4 color /*= Color4::White()*/)
	{
		const int segCount = 20;
		Vertex2 v[segCount*2];
		ULong dcolor = color.ABGR();

		float angleSeg = 2.0f*Math::PI()/(float)(segCount - 1);
		for (int i = 0; i < segCount; i++)
		{
			float a = (float)i*angleSeg;
			v[i*2]     = Vertex2(Vec2F::Rotated(a)*radius + pos, dcolor, 0, 0);
			v[i*2 + 1] = Vertex2(Vec2F::Rotated(a + angleSeg)*radius + pos, dcolor, 0, 0);
		}

		DrawLines(v, segCount);
	}

	void Render::BeginRenderToStencilBuffer()
	{
		if (mStencilDrawing || mStencilTest)
			return;

		DrawPrimitives();

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0x1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		GL_CHECK_ERROR(mLog);

		mStencilDrawing = true;
	}

	void Render::EndRenderToStencilBuffer()
	{
		if (!mStencilDrawing)
			return;

		DrawPrimitives();

		glDisable(GL_STENCIL_TEST);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		GL_CHECK_ERROR(mLog);

		mStencilDrawing = false;
	}

	void Render::EnableStencilTest()
	{
		if (mStencilTest || mStencilDrawing)
			return;

		DrawPrimitives();

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);

		GL_CHECK_ERROR(mLog);

		mStencilTest = true;
	}

	void Render::DisableStencilTest()
	{
		if (!mStencilTest)
			return;

		DrawPrimitives();

		glDisable(GL_STENCIL_TEST);

		mStencilTest = false;
	}

	bool Render::IsStencilTestEnabled() const
	{
		return mStencilTest;
	}

	void Render::ClearStencil()
	{
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		GL_CHECK_ERROR(mLog);
	}

	void Render::SetupScissorRect(const RectI& rect)
	{
		if (rect == mScissorRect)
			return;

		glScissor((int)rect.left, mResolution.y - (int)rect.bottom, (int)rect.Width(), (int)rect.Height());

		mScissorRect = rect;
	}

	RectI Render::GetScissorRect() const
	{
		return mScissorRect;
	}

	void Render::EnableScissorTest()
	{
		if (mScissorTest)
			return;

		DrawPrimitives();

		glEnable(GL_SCISSOR_TEST);

		GL_CHECK_ERROR(mLog);

		mScissorTest = true;
	}

	void Render::DisableScissorTest()
	{
		if (!mScissorTest)
			return;

		DrawPrimitives();

		glDisable(GL_SCISSOR_TEST);

		GL_CHECK_ERROR(mLog);

		mScissorTest = false;
	}

	bool Render::IsScissorTestEnabled() const
	{
		return mScissorTest;
	}

	bool Render::DrawMesh(Ptr<Mesh> mesh)
	{
		if (!mReady)
			return false;

		// Check difference
		if (mLastDrawTexture != mesh->mTexture ||
			mLastDrawVertex + mesh->mVertexCount >= mVertexBufferSize ||
			mLastDrawIdx + mesh->mPolyCount*3 >= mIndexBufferSize ||
			mCurrentPrimitiveType == GL_LINES)
		{
			DrawPrimitives();

			mLastDrawTexture = mesh->mTexture;
			mCurrentPrimitiveType = GL_TRIANGLES;

			if (mLastDrawTexture)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, mLastDrawTexture->mHandle);

				GL_CHECK_ERROR(mLog);
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}
		}

		// Copy data
		memcpy(&mVertexData[mLastDrawVertex*sizeof(Vertex2)], mesh->mVerticies, sizeof(Vertex2)*mesh->mVertexCount);

		for (unsigned int i = mLastDrawIdx, j = 0; j < mesh->mPolyCount*3; i++, j++)
		{
			mVertexIndexData[i] = mLastDrawVertex + mesh->mIndexes[j];
		}

		mTrianglesCount += mesh->mPolyCount;
		mLastDrawVertex += mesh->mVertexCount;
		mLastDrawIdx += mesh->mPolyCount*3;

		return true;
	}

	bool Render::DrawLines(Vertex2* verticies, int count)
	{
		if (!mReady)
			return false;

		// Check difference
		if (mCurrentPrimitiveType == GL_TRIANGLES ||
			mLastDrawVertex + count*2 >= mVertexBufferSize ||
			mLastDrawIdx + count*2 >= mIndexBufferSize)
		{
			DrawPrimitives();

			mLastDrawTexture = NULL;
			mCurrentPrimitiveType = GL_LINES;
			glDisable(GL_TEXTURE_2D);
		}

		// Copy data
		memcpy(&mVertexData[mLastDrawVertex*sizeof(Vertex2)], verticies, sizeof(Vertex2)*count*2);

		for (unsigned int i = mLastDrawIdx, j = 0; j < (unsigned int)count*2; i++, j++)
		{
			mVertexIndexData[i] = mLastDrawVertex + j;
		}

		mTrianglesCount += count;
		mLastDrawVertex += count*2;
		mLastDrawIdx += count*2;

		return true;
	}

	void Render::SetRenderTexture(Ptr<Texture> renderTarget)
	{
		if (!renderTarget)
		{
			UnbindRenderTexture();
			return;
		}

		if (renderTarget->mUsage != Texture::Usage::RenderTarget)
		{
			mLog->Error("Can't set texture as render target: not render target texture");
			UnbindRenderTexture();
			return;
		}

		if (!renderTarget->IsReady())
		{
			mLog->Error("Can't set texture as render target: texture isn't ready");
			UnbindRenderTexture();
			return;
		}

		DrawPrimitives();

		glBindFramebufferEXT(GL_FRAMEBUFFER, renderTarget->mFrameBuffer);

		GL_CHECK_ERROR(mLog);

		SetupViewMatrix(renderTarget->GetSize());

		mCurrentRenderTarget = renderTarget;
	}

	void Render::UnbindRenderTexture()
	{
		if (!mCurrentRenderTarget)
			return;

		DrawPrimitives();

		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

		GL_CHECK_ERROR(mLog);

		SetupViewMatrix(mResolution);

		mCurrentRenderTarget = nullptr;
	}

	Ptr<Texture> Render::GetRenderTexture() const
	{
		return mCurrentRenderTarget;
	}

	bool Render::IsRenderTextureAvailable() const
	{
		return mRenderTargetsAvailable;
	}

	Vec2I Render::GetMaxTextureSize() const
	{
		return mMaxTextureSize;
	}

	void Render::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Render, CurrentCamera, SetCamera, GetCamera);
		INITIALIZE_PROPERTY(Render, ScissorRect, SetupScissorRect, GetScissorRect);
		INITIALIZE_PROPERTY(Render, RenderTexture, SetRenderTexture, GetRenderTexture);
		INITIALIZE_GETTER(Render, Resolution, GetResolution);
		INITIALIZE_GETTER(Render, RenderTextureAvailable, IsRenderTextureAvailable);
		INITIALIZE_GETTER(Render, MaxTextureSize, GetMaxTextureSize);
	}

	Render& Render::operator=(const Render& other)
	{
		return *this;
	}
}