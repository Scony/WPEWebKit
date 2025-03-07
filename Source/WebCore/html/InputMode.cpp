/*
 * Copyright (C) 2018 Apple Inc. All rights reserved.
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
#include "InputMode.h"

#include <wtf/NeverDestroyed.h>

namespace WebCore {

InputMode inputModeForAttributeValue(const AtomicString& value)
{
    if (equalIgnoringASCIICase(value, InputModeNames::text()))
        return InputMode::Text;
    if (equalIgnoringASCIICase(value, InputModeNames::tel()))
        return InputMode::Telephone;
    if (equalIgnoringASCIICase(value, InputModeNames::url()))
        return InputMode::Url;
    if (equalIgnoringASCIICase(value, InputModeNames::email()))
        return InputMode::Email;
    if (equalIgnoringASCIICase(value, InputModeNames::numeric()))
        return InputMode::Numeric;
    if (equalIgnoringASCIICase(value, InputModeNames::decimal()))
        return InputMode::Decimal;
    if (equalIgnoringASCIICase(value, InputModeNames::search()))
        return InputMode::Search;

    return InputMode::Unspecified;
}

const AtomicString& stringForInputMode(InputMode mode)
{
    switch (mode) {
    case InputMode::Unspecified:
        return emptyAtom();
    case InputMode::Text:
        return InputModeNames::text();
    case InputMode::Telephone:
        return InputModeNames::tel();
    case InputMode::Url:
        return InputModeNames::url();
    case InputMode::Email:
        return InputModeNames::email();
    case InputMode::Numeric:
        return InputModeNames::numeric();
    case InputMode::Decimal:
        return InputModeNames::decimal();
    case InputMode::Search:
        return InputModeNames::search();
    }
    ASSERT_NOT_REACHED();
    return emptyAtom();
}

namespace InputModeNames {

const AtomicString& text()
{
    static NeverDestroyed<AtomicString> mode("text", AtomicString::ConstructFromLiteral);
    return mode;
}

const AtomicString& tel()
{
    static NeverDestroyed<AtomicString> mode("tel", AtomicString::ConstructFromLiteral);
    return mode;
}

const AtomicString& url()
{
    static NeverDestroyed<AtomicString> mode("url", AtomicString::ConstructFromLiteral);
    return mode;
}

const AtomicString& email()
{
    static NeverDestroyed<AtomicString> mode("email", AtomicString::ConstructFromLiteral);
    return mode;
}

const AtomicString& numeric()
{
    static NeverDestroyed<AtomicString> mode("numeric", AtomicString::ConstructFromLiteral);
    return mode;
}

const AtomicString& decimal()
{
    static NeverDestroyed<AtomicString> mode("decimal", AtomicString::ConstructFromLiteral);
    return mode;
}

const AtomicString& search()
{
    static NeverDestroyed<AtomicString> mode("search", AtomicString::ConstructFromLiteral);
    return mode;
}

} // namespace InputModeNames

} // namespace WebCore
