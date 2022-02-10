/*
 * Copyright (c) 2022, Ben Abraham
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibWeb/Bindings/AudioContextWrapper.h>
#include <LibWeb/Audio/BaseAudioContext.h>

namespace Web::Audio {

struct AudioContextOptions {
    JS::Value latency_hint;
    float sample_rate { -1 };
};

// https://html.spec.whatwg.org/multipage/workers.html#dedicated-workers-and-the-worker-interface
class AudioContext final
    : public BaseAudioContext {
public:
    using WrapperType = Bindings::AudioContextWrapper;

    static NonnullRefPtr<Audio::AudioContext> create()//AudioContextOptions const& options
    {
        return adopt_ref(*new Audio::AudioContext());//move(options)
    }
    static NonnullRefPtr<Audio::AudioContext> create_with_global_object(Bindings::WindowObject&)//, AudioContextOptions const& options
    {
        return Audio::AudioContext::create();//options
    }

    virtual ~AudioContext() = default;

protected:
    AudioContext();//AudioContextOptions const&

private:
    virtual void ref_event_target() override { ref(); }
    virtual void unref_event_target() override { unref(); }
    virtual JS::Object* create_wrapper(JS::GlobalObject&) override;
    //AudioContextOptions m_options;
};

} // namespace Web::Audio

namespace Web::Bindings {

AudioContextWrapper* wrap(JS::GlobalObject&, Audio::AudioContext&);

}
