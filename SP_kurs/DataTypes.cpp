#include "DataTypes.h"

bool directory_exists(const std::string& directory)
{
	if (!directory.empty())
	{
		if (_access(directory.c_str(), 0) == 0)
		{
			struct stat status;
			stat(directory.c_str(), &status);
			if (status.st_mode & S_IFDIR)
				return true;
		}
	}
	// if any condition fails
	return false;
}