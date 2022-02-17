/*
 * Copyright (c) 2022, Ben Abraham <ben.d.abraham@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <LibWeb/Bindings/WorkerWrapper.h>
#include <LibWeb/DOM/ExceptionOr.h>
#include <LibWeb/HTML/Worker.h>

namespace Web::HTML {

// https://html.spec.whatwg.org/multipage/workers.html#dedicated-workers-and-the-worker-interface

Worker::Worker(String& script_url, WorkerOptions const& options)
    : Worker(script_url, options, nullptr)
{
}

Worker::Worker(String& script_url, WorkerOptions const& options, DOM::Document* document)
    : m_script_url(script_url)
    , m_options(options)
    , m_document(document)
{
}

DOM::ExceptionOr<NonnullRefPtr<Worker>> Worker::create(String& script_url, WorkerOptions const& options, DOM::Document* document)
{
    dbgln_if(WEB_WORKER_DEBUG, "Worker: Create={}", script_url);

    // Returns a new Worker object. scriptURL will be fetched and executed in the background,
    // creating a new global environment for which worker represents the communication channel.
    // options can be used to define the name of that global environment via the name option,
    // primarily for debugging purposes. It can also ensure this new global environment supports
    // JavaScript modules (specify type: "module"), and if that is specified, can also be used
    // to specify how scriptURL is fetched through the credentials option.

    // FIXME: 1. The user agent may throw a "SecurityError" DOMException if the request violates
    // a policy decision (e.g. if the user agent is configured to not allow the page to start dedicated workers).
    // Technically not a fixme if our policy is not to throw errors :^)

    // 2. Let outside settings be the current settings object.
    auto outside_settings = options;

    // FIXME: 3. Parse the scriptURL argument relative to outside settings.
    auto url = document->parse_url(script_url);

    // 4. If this fails, throw a "SyntaxError" DOMException.
    if (!url.is_valid()) {
        dbgln_if(WEB_WORKER_DEBUG, "WebWorker: Invalid URL loaded '{}'.", script_url);
        return DOM::SyntaxError::create("Failed to parse selector");
    }

    // 5. Let worker URL be the resulting URL record.
    auto worker_url = url;

    // 6. Let worker be a new Worker object.
    auto worker = adopt_ref(*new Worker(script_url, move(options), document));

    // 7. Let outside port be a new MessagePort in outside settings's Realm.
    auto outside_port = MessagePort::create();

    // 8. Associate the outside port with worker.
    worker->m_message_port = outside_port;

    // 9. Run this step in parallel:
    ResourceLoader::the().load(
        worker_url,
        [document, worker_url](auto data, auto&, auto) {
            if (data.is_null()) {
                dbgln_if(WEB_WORKER_DEBUG, "WebWorker: Failed to load {}", worker_url);
                return;
            }

            dbgln_if(WEB_WORKER_DEBUG, "WebWorker: Script ready!");
            auto script = ClassicScript::create(worker_url.to_string(), data, document->relevant_settings_object(), AK::URL());

            // FIXME: All 31(!) spec steps for properly executing the script and entangling the message ports.
            auto result = script->run();
        },
        [](auto&, auto) {
            dbgln_if(WEB_WORKER_DEBUG, "WebWorker: HONK! Failed to load script.");
        });

    // 10. Return worker.
    return worker;
}

// https://html.spec.whatwg.org/multipage/workers.html#dom-worker-terminate
DOM::ExceptionOr<void> Worker::terminate()
{
    dbgln_if(WEB_WORKER_DEBUG, "WebWorker: Terminate");

    return {};
}
// https://html.spec.whatwg.org/multipage/workers.html#dom-worker-postmessage
DOM::ExceptionOr<void> Worker::post_message(JS::Value& message, NonnullRefPtrVector<JS::Value>&)
{
    dbgln_if(WEB_WORKER_DEBUG, "WebWorker: Post Message: {}", message.to_string_without_side_effects());

    return {};
}

// https://html.spec.whatwg.org/multipage/workers.html#dom-worker-postmessage-options
DOM::ExceptionOr<void> Worker::post_message(JS::Value& message, StructuredSerializeOptions const&)
{
    dbgln_if(WEB_WORKER_DEBUG, "WebWorker: Post Message: {} w/options", message.to_string_without_side_effects());

    return {};
}

JS::Object* Worker::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

#undef __ENUMERATE
#define __ENUMERATE(attribute_name, event_name)                               \
    void Worker::set_##attribute_name(Optional<Bindings::CallbackType> value) \
    {                                                                         \
        set_event_handler_attribute(event_name, move(value));                 \
    }                                                                         \
    Bindings::CallbackType* Worker::attribute_name()                          \
    {                                                                         \
        return event_handler_attribute(event_name);                           \
    }
ENUMERATE_WORKER_EVENT_HANDLERS(__ENUMERATE)
#undef __ENUMERATE

} // namespace Web::HTML
