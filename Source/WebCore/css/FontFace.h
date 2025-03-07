/*
 * Copyright (C) 2007, 2008, 2016 Apple Inc. All rights reserved.
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

#include "ActiveDOMObject.h"
#include "CSSFontFace.h"
#include "CSSPropertyNames.h"
#include "DOMPromiseProxy.h"
#include <wtf/UniqueRef.h>
#include <wtf/Variant.h>
#include <wtf/WeakPtr.h>

namespace JSC {
class ArrayBuffer;
class ArrayBufferView;
}

namespace WebCore {

class FontFace final : public RefCounted<FontFace>, public CanMakeWeakPtr<FontFace>, public ActiveDOMObject, private CSSFontFace::Client {
public:
    struct Descriptors {
        String style;
        String weight;
        String stretch;
        String unicodeRange;
        String variant;
        String featureSettings;
        String display;
    };
    
    using Source = Variant<String, RefPtr<JSC::ArrayBuffer>, RefPtr<JSC::ArrayBufferView>>;
    static ExceptionOr<Ref<FontFace>> create(Document&, const String& family, Source&&, const Descriptors&);
    static Ref<FontFace> create(CSSFontFace&);
    virtual ~FontFace();

    ExceptionOr<void> setFamily(const String&);
    ExceptionOr<void> setStyle(const String&);
    ExceptionOr<void> setWeight(const String&);
    ExceptionOr<void> setStretch(const String&);
    ExceptionOr<void> setUnicodeRange(const String&);
    ExceptionOr<void> setVariant(const String&);
    ExceptionOr<void> setFeatureSettings(const String&);
    ExceptionOr<void> setDisplay(const String&);

    String family() const;
    String style() const;
    String weight() const;
    String stretch() const;
    String unicodeRange() const;
    String variant() const;
    String featureSettings() const;
    String display() const;

    enum class LoadStatus { Unloaded, Loading, Loaded, Error };
    LoadStatus status() const;

    using LoadedPromise = DOMPromiseProxyWithResolveCallback<IDLInterface<FontFace>>;
    LoadedPromise& loadedForBindings();
    LoadedPromise& loadForBindings();

    void adopt(CSSFontFace&);

    CSSFontFace& backing() { return m_backing; }

    static RefPtr<CSSValue> parseString(const String&, CSSPropertyID);

    void fontStateChanged(CSSFontFace&, CSSFontFace::Status oldState, CSSFontFace::Status newState) final;

    void ref() final { RefCounted::ref(); }
    void deref() final { RefCounted::deref(); }

    bool hasPendingActivity() const final;
    bool canSuspendForDocumentSuspension() const final;

private:
    explicit FontFace(CSSFontSelector&);
    explicit FontFace(CSSFontFace&);

    const char* activeDOMObjectName() const final;

    // Callback for LoadedPromise.
    FontFace& loadedPromiseResolve();

    Ref<CSSFontFace> m_backing;
    UniqueRef<LoadedPromise> m_loadedPromise;
    bool m_mayLoadedPromiseBeScriptObservable { false };
};

}
