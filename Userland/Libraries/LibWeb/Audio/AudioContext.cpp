/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <LibWeb/Audio/AudioContext.h>

namespace Web::Audio {

// https://webaudio.github.io/web-audio-api/#AudioContext

AudioContext::AudioContext()//AudioContextOptions const& options)
    //: m_options(options)
	: BaseAudioContext()
{
}

JS::Object* AudioContext::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

}
