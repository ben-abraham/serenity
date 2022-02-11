/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/RefCounted.h>
#include <AK/URLParser.h>
#include <LibWeb/Forward.h>
#include <LibWeb/Bindings/WindowObject.h>
#include <LibWeb/Bindings/Wrappable.h>
#include <LibWeb/DOM/Document.h>
#include <LibWeb/DOM/EventTarget.h>
#include <LibWeb/DOM/ExceptionOr.h>

namespace Web::Audio {

// https://html.spec.whatwg.org/multipage/workers.html#dedicated-workers-and-the-worker-interface
class MediaStream
    : public RefCounted<MediaStream>
    , public Weakable<MediaStream>
    , public DOM::EventTarget
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::MediaStreamWrapper;

    using RefCounted::ref;
    using RefCounted::unref;

    static NonnullRefPtr<MediaStream> create(JS::Value src_stream)//MediaStream const* src_stream)
    {
        return adopt_ref(*new Audio::MediaStream(src_stream));
    }
    static NonnullRefPtr<MediaStream> create_with_global_object(Bindings::WindowObject&, JS::Value src_stream)//MediaStream const* src_stream)
    {
        return Audio::MediaStream::create(src_stream);
    }

    virtual ~MediaStream() = default;

    // ^EventTarget
    virtual void ref_event_target() override { ref(); }
    virtual void unref_event_target() override { unref(); }
    virtual JS::Object* create_wrapper(JS::GlobalObject&) override;

protected:
    MediaStream(JS::Value);// const*);

private:
};

} // namespace Web::Audio

namespace Web::Bindings {

MediaStreamWrapper* wrap(JS::GlobalObject&, Audio::MediaStream&);

}