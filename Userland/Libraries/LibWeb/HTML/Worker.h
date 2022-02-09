/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/RefCounted.h>
#include <AK/URLParser.h>
#include <LibWeb/DOM/Document.h>
#include <LibWeb/DOM/EventTarget.h>
#include <LibWeb/DOM/ExceptionOr.h>
#include <LibWeb/DOM/Window.h>
#include <LibWeb/Forward.h>
#include <LibWeb/HTML/Scripting/ClassicScript.h>
#include <LibWeb/HTML/EventNames.h>
#include <LibWeb/HTML/EventLoop/Task.h>
#include <LibWeb/HTML/MessagePort.h>
#include <LibWeb/HTML/WorkerGlobalScope.h>
#include <LibWeb/HTML/WorkerLocation.h>
#include <LibWeb/Loader/LoadRequest.h>
#include <LibWeb/Loader/ResourceLoader.h>
#include <LibWeb/Bindings/Wrappable.h>
#include <LibWeb/Bindings/EventTargetWrapper.h>
#include <LibWeb/Bindings/WindowObject.h>
#include <LibWeb/Bindings/WorkerWrapper.h>

#define ENUMERATE_WORKER_EVENT_HANDLERS(E)              \
    E(onmessage, HTML::EventNames::message)             \
    E(onmessageerror, HTML::EventNames::messageerror)

namespace Web::HTML {

struct WorkerOptions {
    String type { "" };
    String credentials { "" };
    String name { "" };
};

struct StructuredSerializeOptions {
    Vector<JS::Value> transfer = {};
};

// https://html.spec.whatwg.org/multipage/workers.html#dedicated-workers-and-the-worker-interface
class Worker
    : public RefCounted<Worker>
    , public Weakable<Worker>
    , public DOM::EventTarget
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::WorkerWrapper;


    using RefCounted::ref;
    using RefCounted::unref;

    static NonnullRefPtr<Worker> create(String& script_url, WorkerOptions const& options, DOM::Document* document)
    {
        return adopt_ref(*new Worker(script_url, move(options), document));
    }
    static NonnullRefPtr<Worker> create_with_global_object(Bindings::WindowObject& window, String& script_url, WorkerOptions const& options)
    {
        dbgln("Worker: Create with global");
        return Worker::create(script_url, options, &window.impl().associated_document());
    }

    DOM::ExceptionOr<void> terminate();
    DOM::ExceptionOr<void> post_message(JS::Value& message, NonnullRefPtrVector<JS::Value>& transfer);
    DOM::ExceptionOr<void> post_message(JS::Value& message, StructuredSerializeOptions const& options);

    virtual ~Worker() = default;

    // ^EventTarget
    virtual void ref_event_target() override { ref(); }
    virtual void unref_event_target() override { unref(); }
    virtual JS::Object* create_wrapper(JS::GlobalObject&) override;

#undef __ENUMERATE
#define __ENUMERATE(attribute_name, event_name)                  \
    void set_##attribute_name(Optional<Bindings::CallbackType>); \
    Bindings::CallbackType* attribute_name();
    ENUMERATE_WORKER_EVENT_HANDLERS(__ENUMERATE)
#undef __ENUMERATE

protected:
    Worker(String&, WorkerOptions const&, DOM::Document*);
    Worker(String&, WorkerOptions const&);

private:
    String& m_script_url;
    WorkerOptions m_options;
    DOM::Document* m_document { nullptr };

    RefPtr<MessagePort> m_message_port;
};

} // namespace Web::HTML


namespace Web::Bindings {

    WorkerWrapper* wrap(JS::GlobalObject&, HTML::Worker&);

}
