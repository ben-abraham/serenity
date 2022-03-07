/*
 * Copyright (c) 2022, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Optional.h>
#include <AK/RefCounted.h>
#include <AK/URL.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibWeb/Bindings/Wrappable.h>
#include <LibWeb/DOM/EventTarget.h>
#include <LibWeb/DOM/ExceptionOr.h>
#include <LibWeb/Forward.h>
#include <LibWeb/HTML/GlobalEventHandlers.h>
#include <LibWeb/HTML/WorkerLocation.h>
#include <LibWeb/HTML/WorkerNavigator.h>

#define ENUMERATE_WORKER_GLOBAL_SCOPE_EVENT_HANDLERS(E)       \
    E(onerror, HTML::EventNames::error)                       \
    E(onlanguagechange, HTML::EventNames::languagechange)     \
    E(ononline, HTML::EventNames::online)                     \
    E(onoffline, HTML::EventNames::offline)                   \
    E(onrejectionhandled, HTML::EventNames::rejectionhandled) \
    E(onunhandledrejection, HTML::EventNames::unhandledrejection)

namespace Web {
namespace Bindings {

// https://html.spec.whatwg.org/multipage/workers.html#the-workerglobalscope-common-interface
// WorkerGlobalScope is the base class of each real WorkerGlobalScope that will be created when the
// user agent runs the run a worker algorithm.
class WorkerGlobalScope
    : public JS::GlobalObject
    , public RefCounted<WorkerGlobalScope>
    , public Weakable<WorkerGlobalScope> {
    JS_OBJECT(WorkerGlobalScope, JS::GlobalObject)
public:
    explicit WorkerGlobalScope();
    virtual void initialize_global_object() override;
    virtual ~WorkerGlobalScope() override;

    // Following methods are from the WorkerGlobalScope IDL definition
    // https://html.spec.whatwg.org/multipage/workers.html#the-workerglobalscope-common-interface

    // https://html.spec.whatwg.org/multipage/workers.html#dom-workerglobalscope-self
    NonnullRefPtr<WorkerGlobalScope const> self() const { return *this; }

    HTML::WorkerLocation& location() { return *m_location; };
    HTML::WorkerNavigator& navigator() { return *m_navigator; };
    DOM::ExceptionOr<void> import_scripts(Vector<String> urls);

/*
#undef __ENUMERATE
#define __ENUMERATE(attribute_name, event_name)                  \
    void set_##attribute_name(Optional<Bindings::CallbackType>); \
    Bindings::CallbackType* attribute_name();
    ENUMERATE_WORKER_GLOBAL_SCOPE_EVENT_HANDLERS(__ENUMERATE)
#undef __ENUMERATE
*/

    // Following methods are from the WindowOrWorkerGlobalScope mixin
    // https://html.spec.whatwg.org/multipage/webappapis.html#windoworworkerglobalscope-mixin

    String origin() const;
    bool is_secure_context() const;
    bool cross_origin_isolated() const;
    DOM::ExceptionOr<String> btoa(String const& data) const;
    DOM::ExceptionOr<String> atob(String const& data) const;

    // Non-IDL public methods

    AK::URL const& url() const { return m_url.value(); }
    void set_url(AK::URL const& url) { m_url = url; }

    JS::Object* web_prototype(const String& class_name) { return m_prototypes.get(class_name).value_or(nullptr); }
    JS::NativeFunction* web_constructor(const String& class_name) { return m_constructors.get(class_name).value_or(nullptr); }

    template<typename T>
    JS::Object& ensure_web_prototype(const String& class_name)
    {
        auto it = m_prototypes.find(class_name);
        if (it != m_prototypes.end())
            return *it->value;
        auto* prototype = heap().allocate<T>(*this, *this);
        m_prototypes.set(class_name, prototype);
        return *prototype;
    }

    template<typename T>
    JS::NativeFunction& ensure_web_constructor(const String& class_name)
    {
        auto it = m_constructors.find(class_name);
        if (it != m_constructors.end())
            return *it->value;
        auto* constructor = heap().allocate<T>(*this, *this);
        m_constructors.set(class_name, constructor);
        define_direct_property(class_name, JS::Value(constructor), JS::Attribute::Writable | JS::Attribute::Configurable);
        return *constructor;
    }

    virtual JS::ThrowCompletionOr<bool> internal_set_prototype_of(JS::Object* prototype) override;

private:

    void visit_edges(Visitor& visitor) override;

    JS_DECLARE_NATIVE_FUNCTION(location_getter);
    JS_DECLARE_NATIVE_FUNCTION(navigator_getter);

    NonnullRefPtr<HTML::WorkerLocation> m_location;

    // FIXME: Implement WorkerNavigator according to the spec
    NonnullRefPtr<HTML::WorkerNavigator> m_navigator;

    // FIXME: Add all these internal slots

    // https://html.spec.whatwg.org/multipage/workers.html#concept-WorkerGlobalScope-owner-set
    // A WorkerGlobalScope object has an associated owner set (a set of Document and WorkerGlobalScope objects). It is initially empty and populated when the worker is created or obtained.
    //     Note: It is a set, instead of a single owner, to accommodate SharedWorkerGlobalScope objects.

    // https://html.spec.whatwg.org/multipage/workers.html#concept-workerglobalscope-type
    // A WorkerGlobalScope object has an associated type ("classic" or "module"). It is set during creation.

    // https://html.spec.whatwg.org/multipage/workers.html#concept-workerglobalscope-url
    // A WorkerGlobalScope object has an associated url (null or a URL). It is initially null.
    Optional<AK::URL> m_url;

    // https://html.spec.whatwg.org/multipage/workers.html#concept-workerglobalscope-name
    // A WorkerGlobalScope object has an associated name (a string). It is set during creation.
    //  Note: The name can have different semantics for each subclass of WorkerGlobalScope.
    //        For DedicatedWorkerGlobalScope instances, it is simply a developer-supplied name, useful mostly for debugging purposes.
    //        For SharedWorkerGlobalScope instances, it allows obtaining a reference to a common shared worker via the SharedWorker() constructor.
    //        For ServiceWorkerGlobalScope objects, it doesn't make sense (and as such isn't exposed through the JavaScript API at all).

    // https://html.spec.whatwg.org/multipage/workers.html#concept-workerglobalscope-policy-container
    // A WorkerGlobalScope object has an associated policy container (a policy container). It is initially a new policy container.

    // https://html.spec.whatwg.org/multipage/workers.html#concept-workerglobalscope-embedder-policy
    // A WorkerGlobalScope object has an associated embedder policy (an embedder policy).

    // https://html.spec.whatwg.org/multipage/workers.html#concept-workerglobalscope-module-map
    // A WorkerGlobalScope object has an associated module map. It is a module map, initially empty.

    // https://html.spec.whatwg.org/multipage/workers.html#concept-workerglobalscope-cross-origin-isolated-capability
    bool m_cross_origin_isolated_capability { false };

    HashMap<String, JS::Object*> m_prototypes;
    HashMap<String, JS::NativeFunction*> m_constructors;
};

}
}
