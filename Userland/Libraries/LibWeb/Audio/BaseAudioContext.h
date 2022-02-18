/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/RefCounted.h>
#include <AK/URLParser.h>
#include <LibWeb/Bindings/WindowObject.h>
#include <LibWeb/Bindings/Wrappable.h>
#include <LibWeb/DOM/Document.h>
#include <LibWeb/DOM/EventTarget.h>
#include <LibWeb/DOM/ExceptionOr.h>
#include <LibWeb/Forward.h>

namespace Web::Audio {

// https://html.spec.whatwg.org/multipage/workers.html#dedicated-workers-and-the-worker-interface
class BaseAudioContext
    : public RefCounted<BaseAudioContext>
    , public Weakable<BaseAudioContext>
    , public DOM::EventTarget
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::BaseAudioContextWrapper;

    using RefCounted::ref;
    using RefCounted::unref;

    static NonnullRefPtr<BaseAudioContext> create()
    {
        return adopt_ref(*new BaseAudioContext());
    }
    static NonnullRefPtr<BaseAudioContext> create_with_global_object(Bindings::WindowObject&)
    {
        return BaseAudioContext::create();
    }

    virtual ~BaseAudioContext() = default;

    // ^EventTarget
    virtual void ref_event_target() override { ref(); }
    virtual void unref_event_target() override { unref(); }
    virtual JS::Object* create_wrapper(JS::GlobalObject&) override;

protected:
    BaseAudioContext();

private:
};

} // namespace Web::Audio

namespace Web::Bindings {

BaseAudioContextWrapper* wrap(JS::GlobalObject&, Audio::BaseAudioContext&);

}
