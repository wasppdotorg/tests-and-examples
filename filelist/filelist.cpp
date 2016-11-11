
#ifdef _WIN64
	intptr_t fh = _findfirst(pathWithExt.c_str(), &dir);
#else
	long fh = _findfirst(pathWithExt.c_str(), &dir);
#endif
