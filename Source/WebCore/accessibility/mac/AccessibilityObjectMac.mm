/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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

#import "config.h"
#import "AccessibilityObject.h"
#import "ElementAncestorIterator.h"
#import "HTMLFieldSetElement.h"
#import "RenderObject.h"
#import "Settings.h"

#if HAVE(ACCESSIBILITY) && PLATFORM(MAC)

#import "WebAccessibilityObjectWrapperMac.h"
#import "Widget.h"

namespace WebCore {

void AccessibilityObject::detachFromParent()
{
    if (isAttachment())
        overrideAttachmentParent(nullptr);
}

void AccessibilityObject::overrideAttachmentParent(AccessibilityObject* parent)
{
    if (!isAttachment())
        return;
    
    id parentWrapper = nil;
    if (parent) {
        if (parent->accessibilityIsIgnored())
            parent = parent->parentObjectUnignored();
        parentWrapper = parent->wrapper();
    }

    ALLOW_DEPRECATED_DECLARATIONS_BEGIN
    [[wrapper() attachmentView] accessibilitySetOverrideValue:parentWrapper forAttribute:NSAccessibilityParentAttribute];
    ALLOW_DEPRECATED_DECLARATIONS_END
}
    
bool AccessibilityObject::accessibilityIgnoreAttachment() const
{
    // FrameView attachments are now handled by AccessibilityScrollView, 
    // so if this is the attachment, it should be ignored.
    Widget* widget = nullptr;
    if (isAttachment() && (widget = widgetForAttachmentView()) && widget->isFrameView())
        return true;

    ALLOW_DEPRECATED_DECLARATIONS_BEGIN
    if ([wrapper() attachmentView])
        return [[wrapper() attachmentView] accessibilityIsIgnored];
    ALLOW_DEPRECATED_DECLARATIONS_END

    // Attachments are ignored by default (unless we determine that we should expose them).
    return true;
}

AccessibilityObjectInclusion AccessibilityObject::accessibilityPlatformIncludesObject() const
{
    if (isMenuListPopup() || isMenuListOption())
        return AccessibilityObjectInclusion::IgnoreObject;

    if (roleValue() == AccessibilityRole::Caption && ariaRoleAttribute() == AccessibilityRole::Unknown)
        return AccessibilityObjectInclusion::IgnoreObject;
    
    if (roleValue() == AccessibilityRole::Mark)
        return AccessibilityObjectInclusion::IncludeObject;

    // Never expose an unknown object on the Mac. Clients of the AX API will not know what to do with it.
    // Special case is when the unknown object is actually an attachment.
    if (roleValue() == AccessibilityRole::Unknown && !isAttachment())
        return AccessibilityObjectInclusion::IgnoreObject;
    
    if (roleValue() == AccessibilityRole::Inline && !isStyleFormatGroup())
        return AccessibilityObjectInclusion::IgnoreObject;

    if (RenderObject* renderer = this->renderer()) {
        // The legend element is ignored if it lives inside of a fieldset element that uses it to generate alternative text.
        if (renderer->isLegend()) {
            Element* element = this->element();
            if (element && ancestorsOfType<HTMLFieldSetElement>(*element).first())
                return AccessibilityObjectInclusion::IgnoreObject;
        }
    }
    
    return AccessibilityObjectInclusion::DefaultBehavior;
}
    
bool AccessibilityObject::caretBrowsingEnabled() const
{
    Frame* frame = this->frame();
    return frame && frame->settings().caretBrowsingEnabled();
}

void AccessibilityObject::setCaretBrowsingEnabled(bool on)
{
    Frame* frame = this->frame();
    if (!frame)
        return;
    frame->settings().setCaretBrowsingEnabled(on);
}

} // WebCore

#endif // HAVE(ACCESSIBILITY) && PLATFORM(MAC)
