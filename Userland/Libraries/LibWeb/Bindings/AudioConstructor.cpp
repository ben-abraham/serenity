/*
 * Copyright (c) 2022, Ben Abraham <ben.d.abraham@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/Bindings/HTMLAudioElementPrototype.h>
#include <LibWeb/Bindings/HTMLAudioElementWrapper.h>
#include <LibWeb/Bindings/AudioConstructor.h>
#include <LibWeb/Bindings/NodeWrapperFactory.h>
#include <LibWeb/DOM/ElementFactory.h>
#include <LibWeb/DOM/Window.h>
#include <LibWeb/Namespace.h>

namespace Web::Bindings {

AudioConstructor::AudioConstructor(JS::GlobalObject& global_object)
    : NativeFunction(*global_object.function_prototype())
{
}

void AudioConstructor::initialize(JS::GlobalObject& global_object)
{
    auto& vm = this->vm();
    auto& window = static_cast<WindowObject&>(global_object);
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, &window.ensure_web_prototype<HTMLAudioElementPrototype>("HTMLAudioElement"), 0);
    define_direct_property(vm.names.length, JS::Value(0), JS::Attribute::Configurable);
}

AudioConstructor::~AudioConstructor()
{
}

JS::ThrowCompletionOr<JS::Value> AudioConstructor::call()
{
    return vm().throw_completion<JS::TypeError>(global_object(), JS::ErrorType::ConstructorWithoutNew, "Audio");
}

// https://html.spec.whatwg.org/multipage/embedded-content.html#dom-Audio
JS::ThrowCompletionOr<JS::Object*> AudioConstructor::construct(FunctionObject&)
{
    auto& window = static_cast<WindowObject&>(global_object());
    auto& document = window.impl().associated_document();
    auto Audio_element = DOM::create_element(document, HTML::TagNames::img, Namespace::HTML);

    if (vm().argument_count() > 0) {
        u32 width = TRY(vm().argument(0).to_u32(global_object()));
        Audio_element->set_attribute(HTML::AttributeNames::width, String::formatted("{}", width));
    }

    if (vm().argument_count() > 1) {
        u32 height = TRY(vm().argument(1).to_u32(global_object()));
        Audio_element->set_attribute(HTML::AttributeNames::height, String::formatted("{}", height));
    }

    return wrap(global_object(), Audio_element);
}

}
