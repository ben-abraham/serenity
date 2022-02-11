/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <LibWeb/Forward.h>
#include <LibWeb/Bindings/MediaStreamWrapper.h>
#include <LibWeb/Audio/MediaStream.h>

namespace Web::Audio {

// https://webaudio.github.io/web-audio-api/#baseaudiocontext

MediaStream::MediaStream(JS::Value)
    : DOM::EventTarget()
{
    dbgln("Media Stream Create!");
}

JS::Object* MediaStream::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

}
