/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <LibWeb/Bindings/AudioContextWrapper.h>
#include <LibWeb/Audio/AudioContext.h>

namespace Web::Audio {


DOM::ExceptionOr<NonnullRefPtr<Audio::AudioContext>> AudioContext::create(AudioContextOptions const& options)
{
    dbgln("WebAudio: Create Audio Context");

    // If the current settings object’s responsible document is NOT fully active,
    // throw an InvalidStateError and abort these steps.

    // 1. Set a [[control thread state]] to suspended on the AudioContext.
    // 2. Set a [[rendering thread state]] to suspended on the AudioContext.
    // 3. Let [[pending resume promises]] be a slot on this AudioContext,
    //    that is an initially empty ordered list of promises.
    // 4. If contextOptions is given, apply the options:
    //      4a. Set the internal latency of this AudioContext according to contextOptions.
    //          latencyHint, as described in latencyHint.
    //      4b. If contextOptions.sampleRate is specified, set the sampleRate of this
    //          AudioContext to this value. Otherwise, use the sample rate of the default
    //          output device. If the selected sample rate differs from the sample rate of
    //          the output device, this AudioContext MUST resample the audio output to match
    //          the sample rate of the output device.
    //      Note: If resampling is required, the latency of the AudioContext may be affected, possibly by a large amount.
    
    // FIXME: Handle individually instead of all in the constructor?
    auto audio_context = adopt_ref(*new Audio::AudioContext(options));

    // 5. If the context is allowed to start, send a control message to start processing.
    // Following steps are performed in the background:
    // FIXME: Policy check

    //      5a. Attempt to acquire system resources. In case of failure, abort the following steps.
    // FIXME: Resource availability check

    //      5b. Set the [[rendering thread state]] to running on the AudioContext.
    audio_context->m_render_thread_state = "running";

    //      5c  queue a media element task to execute the following steps:
    //          1. Set the state attribute of the AudioContext to "running".
    //          2. Queue a media element task to fire an event named statechange at the AudioContext.
    
    // FIXME: Proper thread in the background
    

    // 6. Return this AudioContext object.
    return audio_context;
}

// https://webaudio.github.io/web-audio-api/#AudioContext-constructors
AudioContext::AudioContext(AudioContextOptions const& options)
	: BaseAudioContext()
    , m_options(options)
    , m_control_thread_state("suspended")
    , m_render_thread_state("suspended")
{
}

JS::Object* AudioContext::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

}
