/*
 * Copyright (C) 2017 Igalia S.L.
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
#include "ResourceUsageOverlay.h"

#if ENABLE(RESOURCE_USAGE) && OS(LINUX)

#include "Chrome.h"
#include "ChromeClient.h"
#include "CommonVM.h"
#include "GraphicsContext.h"
#include "Page.h"
#include "RenderTheme.h"
#include "ResourceUsageThread.h"
#include <JavaScriptCore/VM.h>

namespace WebCore {

static ResourceUsageData gData;

static String cpuUsageString(float cpuUsage)
{
    if (cpuUsage < 0)
        return "<unknown>"_s;
    return String::format("%.1f%%", cpuUsage);
}

static String formatByteNumber(size_t number)
{
    if (number >= 1024 * 1048576)
        return String::format("%.3f GB", static_cast<double>(number) / (1024 * 1048576));
    if (number >= 1048576)
        return String::format("%.2f MB", static_cast<double>(number) / 1048576);
    if (number >= 1024)
        return String::format("%.1f kB", static_cast<double>(number) / 1024);
    return String::format("%zu", number);
}

static String gcTimerString(MonotonicTime timerFireDate, MonotonicTime now)
{
    if (std::isnan(timerFireDate))
        return "[not scheduled]"_s;
    return String::format("%g", (timerFireDate - now).seconds());
}

static const float gFontSize = 14;

class ResourceUsageOverlayPainter final : public GraphicsLayerClient {
public:
    ResourceUsageOverlayPainter(ResourceUsageOverlay& overlay)
        : m_overlay(overlay)
    {
        FontCascadeDescription fontDescription;
        RenderTheme::singleton().systemFont(CSSValueMessageBox, fontDescription);
        fontDescription.setComputedSize(gFontSize);
        m_textFont = FontCascade(WTFMove(fontDescription), 0, 0);
        m_textFont.update(nullptr);
    }

    ~ResourceUsageOverlayPainter() = default;

private:
    void paintContents(const GraphicsLayer*, GraphicsContext& context, GraphicsLayerPaintingPhase, const FloatRect& clip, GraphicsLayerPaintBehavior) override
    {
        GraphicsContextStateSaver stateSaver(context);
        context.fillRect(clip, Color(0.0f, 0.0f, 0.0f, 0.8f));
        context.setFillColor(Color(0.9f, 0.9f, 0.9f, 1.f));

        FloatPoint position = { 10, 20 };
        String string =  "CPU: " + cpuUsageString(gData.cpu);
        context.drawText(m_textFont, TextRun(string), position);
        position.move(0, gFontSize + 2);

        string = "Memory: " + formatByteNumber(gData.totalDirtySize);
        context.drawText(m_textFont, TextRun(string), position);
        position.move(0, gFontSize + 2);

        string = "External: " + formatByteNumber(gData.totalExternalSize);
        context.drawText(m_textFont, TextRun(string), position);
        position.move(0, gFontSize + 2);

        string = "GC Heap: " + formatByteNumber(gData.categories[MemoryCategory::GCHeap].dirtySize);
        context.drawText(m_textFont, TextRun(string), position);
        position.move(0, gFontSize + 2);

        string = "GC owned: " + formatByteNumber(gData.categories[MemoryCategory::GCOwned].dirtySize);
        context.drawText(m_textFont, TextRun(string), position);
        position.move(0, gFontSize + 2);

        MonotonicTime now = MonotonicTime::now();
        string = "Eden GC: " + gcTimerString(gData.timeOfNextEdenCollection, now);
        context.drawText(m_textFont, TextRun(string), position);
        position.move(0, gFontSize + 2);

        string = "Full GC: " + gcTimerString(gData.timeOfNextFullCollection, now);
        context.drawText(m_textFont, TextRun(string), position);
        position.move(0, gFontSize + 2);
    }

    void notifyFlushRequired(const GraphicsLayer*) override
    {
        m_overlay.overlay().page()->chrome().client().scheduleCompositingLayerFlush();
    }

    ResourceUsageOverlay& m_overlay;
    FontCascade m_textFont;
};

void ResourceUsageOverlay::platformInitialize()
{
    m_overlayPainter = std::make_unique<ResourceUsageOverlayPainter>(*this);
    m_paintLayer = GraphicsLayer::create(overlay().page()->chrome().client().graphicsLayerFactory(), *m_overlayPainter);
    m_paintLayer->setAnchorPoint(FloatPoint3D());
    m_paintLayer->setSize({ normalWidth, normalHeight });
    m_paintLayer->setBackgroundColor(Color(0.0f, 0.0f, 0.0f, 0.8f));
    m_paintLayer->setDrawsContent(true);
    overlay().layer().addChild(m_paintLayer.get());

    ResourceUsageThread::addObserver(this, [this] (const ResourceUsageData& data) {
        gData = data;
        m_paintLayer->setNeedsDisplay();
    });
}

void ResourceUsageOverlay::platformDestroy()
{
    ResourceUsageThread::removeObserver(this);
    if (!m_paintLayer)
        return;

    m_paintLayer->removeFromParent();
    m_paintLayer = nullptr;
    m_overlayPainter = nullptr;
}

} // namespace WebCore

#endif // ENABLE(RESOURCE_USAGE) && OS(LINUX)
