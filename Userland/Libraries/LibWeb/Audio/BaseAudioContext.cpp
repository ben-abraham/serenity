/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <LibWeb/Forward.h>
#include <LibWeb/Bindings/BaseAudioContextWrapper.h>
#include <LibWeb/Audio/BaseAudioContext.h>

namespace Web::Audio {

// https://webaudio.github.io/web-audio-api/#baseaudiocontext

BaseAudioContext::BaseAudioContext()
    : DOM::EventTarget()
{
}

JS::Object* BaseAudioContext::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

}
