// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <IceUtil/DisableWarnings.h>
#define ICE_PATCH2_API_EXPORTS
#include <OS.h>
#include <IceUtil/Unicode.h>
#include <climits>

#ifdef __BCPLUSPLUS__
#  include <dir.h>
#  include <io.h>
#endif

using namespace std;
using namespace IceInternal::OS;

#ifdef _WIN32

int
IceInternal::OS::remove(const string& path)
{
    return ::_wremove(IceUtil::stringToWstring(path).c_str());
}

int
IceInternal::OS::rename(const string& from, const string& to)
{
    return ::_wrename(IceUtil::stringToWstring(from).c_str(), IceUtil::stringToWstring(to).c_str());
}

int
IceInternal::OS::rmdir(const string& path)
{
    return ::_wrmdir(IceUtil::stringToWstring(path).c_str());
}

int
IceInternal::OS::mkdir(const string& path, int)
{
    return ::_wmkdir(IceUtil::stringToWstring(path).c_str());
}

FILE*
IceInternal::OS::fopen(const string& path, const string& mode)
{
    return ::_wfopen(IceUtil::stringToWstring(path).c_str(), IceUtil::stringToWstring(mode).c_str());
}

int
IceInternal::OS::open(const string& path, int flags)
{
    return ::_wopen(IceUtil::stringToWstring(path).c_str(), flags);
}

int
IceInternal::OS::getcwd(string& cwd)
{
    wchar_t cwdbuf[_MAX_PATH];
    if(_wgetcwd(cwdbuf, _MAX_PATH) == NULL)
    {
        return -1;
    }
    cwd = IceUtil::wstringToString(cwdbuf);
    return 0;
}

#else

int
IceInternal::OS::remove(const string& path)
{
    return ::remove(path.c_str());
}

int
IceInternal::OS::rename(const string& from, const string& to)
{
    return ::rename(from.c_str(), to.c_str());
}

int
IceInternal::OS::rmdir(const string& path)
{
    return ::rmdir(path.c_str());
}

int
IceInternal::OS::mkdir(const string& path, int perm)
{
    return ::mkdir(path.c_str(), perm);
}

FILE*
IceInternal::OS::fopen(const string& path, const string& mode)
{
    return ::fopen(path.c_str(), mode.c_str());
}

int
IceInternal::OS::open(const string& path, int flags)
{
    return ::open(path.c_str(), flags);
}

int
IceInternal::OS::getcwd(string& cwd)
{
    char cwdbuf[PATH_MAX];
    if(::getcwd(cwdbuf, PATH_MAX) == NULL)
    {
        return -1;
    }
    cwd = cwdbuf;
    return 0;
}

#endif
