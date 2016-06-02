#include "Utils/Math/Math.h"

#include "Utils/Math/Vector2.h"
#include "Utils/Math/Rect.h"

namespace o2
{
	namespace Math
	{

		Vec2F Round(const Vec2F& value)
		{
			return Vec2F(Round(value.x), Round(value.y));
		}

		RectF Round(const RectF& value)
		{
			return RectF(Round(value.left), Round(value.top), Round(value.right), Round(value.bottom));
		}

		RectF Lerp(const RectF& a, const RectF& b, float coef)
		{
			return RectF(Lerp(a.left, b.left, coef), Lerp(a.top, b.top, coef),
						 Lerp(a.right, b.right, coef), Lerp(a.bottom, b.bottom, coef));
		}

		bool Lerp(const bool& a, const bool& b, float coef)
		{
			return coef > 0.5f ? b : a;
		}

		void OrthoProjMatrix(float* mat, float left, float right, float bottom, float top, float nearz, float farz)
		{
			float tx = -(right + left)/(right - left);
			float ty = -(top + bottom)/(top - bottom);
			float tz = -(farz + nearz)/(farz - nearz);

			mat[0] = 2.0f/(right - left); mat[4] = 0.0f;                mat[8] = 0.0f;                  mat[12] = tx;
			mat[1] = 0.0f;                mat[5] = 2.0f/(top - bottom); mat[9] = 0.0f;                  mat[13] = ty;
			mat[2] = 0.0f;                mat[6] = 0.0f;                mat[10] = -2.0f/(farz - nearz); mat[14] = tz;
			mat[3] = 0.0f;                mat[7] = 0.0f;                mat[11] = 0.0f;                 mat[15] = 1.0f;
		}
	}
} 