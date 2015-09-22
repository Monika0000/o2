#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Render/TextureRef.h"
#include "Utils/CommonTypes.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/String.h"

namespace o2
{
	class Mesh;
	class Render;

	// -----------------------------------------------------------
	// Font. Containing array of symbol glyphs, symbol index table
	// -----------------------------------------------------------
	class Font
	{
	protected:
		struct Character;

	public:
		// Default constructor
		Font();

		// Copy-constructor
		Font(const Font& font);

		// Destructor
		virtual ~Font();

		// Returns base height
		float GetHeight() const;

		// Returns character constant reference by id
		virtual const Character& GetCharacter(UInt16 id);

		// Checks characters for pre loading
		virtual void CheckCharacters(const WString& needChararacters);

	protected:

		// --------------------
		// Character definition
		// --------------------
		struct Character
		{
			RectF  mTexSrc;  // texture source rect
			Vec2F  mSize;    // Size of source rect
			Vec2F  mOffset;  // Symbol offset
			float  mAdvance; // Symbol advance
			UInt16 mId;      // Character id

			bool operator==(const Character& other) const;
		};
		typedef Vector<Character> CharactersVec;

	protected:
		CharactersVec mCharacters;      // Characters array
		float         mBaseHeight;      // Base height, in pixels
		TextureRef    mTexture;         // Texture
		RectI         mTextureSrcRect;  // Texture source rectangle
		bool          mReady;           // True when font is ready to use

		friend class Text;
	};
}