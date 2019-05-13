#pragma once

#include "Utils/FileSystem/File.h"
#include "Utils/Serialization/Serializable.h"
#include "Utils/System/Time/Time.h"
#include "Utils/Types/CommonTypes.h"

namespace o2
{
	// ---------
	// File info
	// ---------
	class FileInfo
	{
	public:
		String    path;
		FileType  fileType;
		TimeStamp createdDate;
		TimeStamp accessDate;
		TimeStamp editDate;
		UInt      size;

		bool operator==(const FileInfo& other) const;
	};

	// ---------
	// Path info
	// ---------
	class FolderInfo
	{
	public:
		typedef Vector<FileInfo> FilesVec;
		typedef Vector<FolderInfo> PathsVec;

		String   path;  // Path of this 
		FilesVec files; // Files vector
		PathsVec folders; // Paths vector

	public:
		// Equal operator
		bool operator==(const FolderInfo& other) const;

		// Returns true if this contains specified path or file
		bool IsFileExist(const String& filePath);

		// Cuts repeating path parts to short style
		// Sample: 
		// SomePath/SomePath2
		// -SomePath/SomePath2/ff.x
		// -SomePath/SomePath2/ffy.x
		// Converting to:
		// SomePath/SomePath2
		// -ff.x
		// -ffy.x
		void ClampPathNames();

	protected:
		// Cut path recursive function
		void ProcessPathNamesClamping(int charCount);
	};
}
