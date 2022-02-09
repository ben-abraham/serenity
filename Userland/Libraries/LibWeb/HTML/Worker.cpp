/*
 * Copyright (c) 2022, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <LibWeb/Bindings/WorkerWrapper.h>
#include <LibWeb/HTML/Worker.h>

namespace Web::HTML {


// https://html.spec.whatwg.org/multipage/workers.html#dedicated-workers-and-the-worker-interface

Worker::Worker(String& script_url, WorkerOptions const& options)
    : Worker(script_url, options, nullptr)
{
}

Worker::Worker(String& script_url, WorkerOptions const& options, DOM::Document* document)
    :   m_script_url(script_url),
        m_options(options),
        m_document(document)
{
    dbgln("Worker: Create={}", script_url);

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
    auto url = document->parse_url(m_script_url);

    // 4. If this fails, throw a "SyntaxError" DOMException.
    if (!url.is_valid()) {
        dbgln("Worker: Invalid URL loaded '{}'.", m_script_url);
        // FIXME: Throw DOMException
        /*
        queue_an_element_task(HTML::Task::Source::Unspecified, [this] {
            dispatch_event(DOM::Event::create(HTML::EventNames::error));
        });
        */
        return;
    }

    // 5. Let worker URL be the resulting URL record.
    auto worker_url = url;

    // 6. Let worker be a new Worker object.
    /* Gestures at constructor */

    // 7. Let outside port be a new MessagePort in outside settings's Realm.
    auto outside_port = MessagePort::create();

    // 8. Associate the outside port with worker.
    m_message_port = outside_port;

    // 9. Run this step in parallel:
    ResourceLoader::the().load(
        worker_url,
        [this, worker_url](auto data, auto&, auto) {
            if (data.is_null()) {
                dbgln("HTMLScriptElement: Failed to load {}", worker_url);
                return;
            }

            dbgln("Worker - Script ready!");
            auto script = ClassicScript::create(worker_url.to_string(), data, this->m_document->relevant_settings_object(), AK::URL());

            // FIXME: All 31(!) spec steps for properly executing the script and entangling the message ports.
            auto result = script->run();
        },
        [this](auto&, auto) {
            dbgln("HONK! Failed to load script.");
        });
    
    // 10. Return worker.
}

DOM::ExceptionOr<void> Worker::terminate()
{
    dbgln("Worker Terminate");

    return {};
}

DOM::ExceptionOr<void> Worker::post_message(JS::Value& message, NonnullRefPtrVector<JS::Value>&)
{
    dbgln("Worker Post Message: {}", message.to_string_without_side_effects());

    return {};
}

DOM::ExceptionOr<void> Worker::post_message(JS::Value& message, StructuredSerializeOptions const&)
{
    dbgln("Worker Post Message: {} w/options", message.to_string_without_side_effects());

    return {};
}

JS::Object* Worker::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

#undef __ENUMERATE
#define __ENUMERATE(attribute_name, event_name)                                          \
    void Worker::set_##attribute_name(Optional<Bindings::CallbackType> value)            \
    {                                                                                    \
        set_event_handler_attribute(event_name, move(value));                            \
    }                                                                                    \
    Bindings::CallbackType* Worker::attribute_name()                                     \
    {                                                                                    \
        return event_handler_attribute(event_name);                                      \
    }
ENUMERATE_WORKER_EVENT_HANDLERS(__ENUMERATE)
#undef __ENUMERATE

} // namespace Web::HTML