/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "RAMSize.h"

#include "StdLibExtras.h"
#include <mutex>

#if OS(WINDOWS)
#include <windows.h>
#elif defined(USE_SYSTEM_MALLOC) && USE_SYSTEM_MALLOC
#if OS(UNIX)
#include <sys/sysinfo.h>
#endif // OS(UNIX)
#else
#include <wtf/text/WTFString.h>
#include <bmalloc/bmalloc.h>
#endif

namespace WTF {

#if OS(WINDOWS)
static const size_t ramSizeGuess = 512 * MB;
#endif

static size_t customRAMSize()
{
    // Syntax: Case insensitive, unit multipliers (M=Mb, K=Kb, <empty>=bytes).
    // Example: WPE_RAM_SIZE='500M'

    String s(getenv("WPE_RAM_SIZE"));
    if (!s.isEmpty()) {
        String value = s.stripWhiteSpace().convertToLowercaseWithoutLocale();
        size_t units = 1;
        if (value.endsWith('k'))
            units = 1024;
        else if (value.endsWith('m'))
            units = 1024 * 1024;
        if (units != 1)
            value = value.substring(0, value.length()-1);
        bool ok = false;
        size_t size = size_t(value.toUInt64(&ok));
        if (ok)
            return size * units;
    }

    return 0;
}

static size_t computeRAMSize(bool real)
{
#if OS(WINDOWS)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    bool result = GlobalMemoryStatusEx(&status);
    if (!result)
        return ramSizeGuess;
    return status.ullTotalPhys;
#elif defined(USE_SYSTEM_MALLOC) && USE_SYSTEM_MALLOC
#if OS(UNIX)
    struct sysinfo si;
    sysinfo(&si);
    return si.totalram * si.mem_unit;
#else
#error "Missing a platform specific way of determining the available RAM"
#endif // OS(UNIX)
#else
    if (!real) {
        size_t custom = customRAMSize();
        if (custom)
            return custom;
    }
    return bmalloc::api::availableMemory();
#endif
}

size_t ramSize()
{
    static size_t ramSize;
    static std::once_flag onceFlag;
    std::call_once(onceFlag, [] {
        ramSize = computeRAMSize(false);
    });
    return ramSize;
}

size_t realRamSize()
{
    static size_t ramSize;
    static std::once_flag onceFlag;
    std::call_once(onceFlag, [] {
        ramSize = computeRAMSize(true);
    });
    return ramSize;
}

} // namespace WTF
