/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 * Copyright (C) 2013-2108 Apple Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(MEDIA_STREAM)

#include "CaptureDeviceManager.h"
#include "MockMediaDevice.h"
#include "MockRealtimeAudioSource.h"
#include "MockRealtimeVideoSource.h"
#include "RealtimeMediaSourceCenter.h"

namespace WebCore {

class MockRealtimeMediaSourceCenter final : public RealtimeMediaSourceCenter {
public:
    WEBCORE_EXPORT static void setMockRealtimeMediaSourceCenterEnabled(bool);

    WEBCORE_EXPORT static void setDevices(Vector<MockMediaDevice>&&);
    WEBCORE_EXPORT static void addDevice(const MockMediaDevice&);
    WEBCORE_EXPORT static void removeDevice(const String& persistentId);
    WEBCORE_EXPORT static void resetDevices();

    static Vector<CaptureDevice>& audioDevices();
    static Vector<CaptureDevice>& videoDevices();
    static Vector<CaptureDevice>& displayDevices();

    static std::optional<MockMediaDevice> mockDeviceWithPersistentID(const String&);
    static std::optional<CaptureDevice> captureDeviceWithPersistentID(CaptureDevice::DeviceType, const String&);

private:
    MockRealtimeMediaSourceCenter() = default;
    friend NeverDestroyed<MockRealtimeMediaSourceCenter>;

    static MockRealtimeMediaSourceCenter& singleton();

    AudioCaptureFactory& audioFactory() final;
    VideoCaptureFactory& videoFactory() final;
    DisplayCaptureFactory& displayCaptureFactory() final;

    CaptureDeviceManager& audioCaptureDeviceManager() final { return m_audioCaptureDeviceManager; }
    CaptureDeviceManager& videoCaptureDeviceManager() final { return m_videoCaptureDeviceManager; }
    CaptureDeviceManager& displayCaptureDeviceManager() final { return m_displayCaptureDeviceManager; }

    class MockAudioCaptureDeviceManager final : public CaptureDeviceManager {
    private:
        const Vector<CaptureDevice>& captureDevices() final { return MockRealtimeMediaSourceCenter::audioDevices(); }
        std::optional<CaptureDevice> captureDeviceWithPersistentID(CaptureDevice::DeviceType type, const String& id) final { return MockRealtimeMediaSourceCenter::captureDeviceWithPersistentID(type, id); }
    };
    class MockVideoCaptureDeviceManager final : public CaptureDeviceManager {
    private:
        const Vector<CaptureDevice>& captureDevices() final { return MockRealtimeMediaSourceCenter::videoDevices(); }
        std::optional<CaptureDevice> captureDeviceWithPersistentID(CaptureDevice::DeviceType type, const String& id) final { return MockRealtimeMediaSourceCenter::captureDeviceWithPersistentID(type, id); }
    };
    class MockDisplayCaptureDeviceManager final : public CaptureDeviceManager {
    private:
        const Vector<CaptureDevice>& captureDevices() final { return MockRealtimeMediaSourceCenter::displayDevices(); }
        std::optional<CaptureDevice> captureDeviceWithPersistentID(CaptureDevice::DeviceType type, const String& id) final { return MockRealtimeMediaSourceCenter::captureDeviceWithPersistentID(type, id); }
    };

    MockAudioCaptureDeviceManager m_audioCaptureDeviceManager;
    MockVideoCaptureDeviceManager m_videoCaptureDeviceManager;
    MockDisplayCaptureDeviceManager m_displayCaptureDeviceManager;
};

}

#endif // MockRealtimeMediaSourceCenter_h
