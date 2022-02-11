/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibWeb/Bindings/AudioContextWrapper.h>
#include <LibWeb/Audio/BaseAudioContext.h>
#include <LibWeb/DOM/ExceptionOr.h>

namespace Web::Audio {

struct AudioContextOptions {
    String latency_hint { "" };
    AK::Optional<double> sample_rate { -1 };
};

// https://webaudio.github.io/web-audio-api/#AudioContext
class AudioContext final
    : public BaseAudioContext {
public:
    using WrapperType = Bindings::AudioContextWrapper;

    static DOM::ExceptionOr<NonnullRefPtr<Audio::AudioContext>> create(AudioContextOptions const& options);
    static DOM::ExceptionOr<NonnullRefPtr<Audio::AudioContext>> create_with_global_object(Bindings::WindowObject&, AudioContextOptions const& options)
    {
        return AudioContext::create(options);
    }

    virtual ~AudioContext() = default;
    virtual JS::Object* create_wrapper(JS::GlobalObject&) override;

protected:
    AudioContext(AudioContextOptions const&);

private:
    AudioContextOptions m_options;
    String m_control_thread_state;
    String m_render_thread_state;
};

} // namespace Web::Audio

namespace Web::Bindings {

AudioContextWrapper* wrap(JS::GlobalObject&, Audio::AudioContext&);

}
