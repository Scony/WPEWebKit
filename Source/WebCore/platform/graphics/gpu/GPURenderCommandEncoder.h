/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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

#if ENABLE(WEBGPU)

#include <wtf/RetainPtr.h>

OBJC_PROTOCOL(MTLRenderCommandEncoder);

namespace WebCore {

class GPUBuffer;
class GPUCommandBuffer;
class GPUDepthStencilState;
class GPURenderPassDescriptor;
class GPURenderPipelineState;

class GPURenderCommandEncoder {
public:
    GPURenderCommandEncoder(const GPUCommandBuffer&, const GPURenderPassDescriptor&);
    ~GPURenderCommandEncoder();

    void setRenderPipelineState(const GPURenderPipelineState&) const;
    void setDepthStencilState(const GPUDepthStencilState&) const;
    void setVertexBuffer(const GPUBuffer&, unsigned offset, unsigned index) const;
    void setFragmentBuffer(const GPUBuffer&, unsigned offset, unsigned index) const;
    void drawPrimitives(unsigned type, unsigned start, unsigned count) const;
    void endEncoding() const;

#if USE(METAL)
private:
    RetainPtr<MTLRenderCommandEncoder> m_metal;
#endif
};
    
} // namespace WebCore

#endif
