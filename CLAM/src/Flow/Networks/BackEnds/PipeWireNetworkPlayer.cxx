#include "PipeWireNetworkPlayer.hxx"

#ifdef USE_PIPEWIRE

#include "AudioSink.hxx"
#include "AudioSource.hxx"

#include <pipewire/pipewire.h>
#include <spa/utils/hook.h>
#include <spa/param/audio/raw-utils.h>
#include <spa/param/param.h>
#include <spa/pod/builder.h>

#include <algorithm>
#include <cstring>
#include <iostream>

namespace CLAM
{

namespace
{

const char * streamStateError(
	enum pw_stream_state state,
	const char * error)
{
	if (error) return error;
	return pw_stream_state_as_string(state);
}

void streamStateChangedCallback(
	void * data,
	enum pw_stream_state oldState,
	enum pw_stream_state state,
	const char * error)
{
	(void)oldState;
	PipeWireNetworkPlayer * player = static_cast<PipeWireNetworkPlayer *>(data);
	player->OnStreamStateChanged(state, streamStateError(state, error));
}

const struct pw_stream_events & inputStreamEvents()
{
	static const struct pw_stream_events events = []
	{
		struct pw_stream_events result = {};
		result.version = PW_VERSION_STREAM_EVENTS;
		result.state_changed = streamStateChangedCallback;
		result.process = PipeWireNetworkPlayer::InputProcessCallback;
		return result;
	}();
	return events;
}

const struct pw_stream_events & outputStreamEvents()
{
	static const struct pw_stream_events events = []
	{
		struct pw_stream_events result = {};
		result.version = PW_VERSION_STREAM_EVENTS;
		result.state_changed = streamStateChangedCallback;
		result.process = PipeWireNetworkPlayer::OutputProcessCallback;
		return result;
	}();
	return events;
}

}

PipeWireNetworkPlayer::PipeWireNetworkPlayer(const std::string & name)
	: _threadLoop(0)
	, _context(0)
	, _core(0)
	, _inputStream(0)
	, _outputStream(0)
	, _inputListener(new spa_hook{})
	, _outputListener(new spa_hook{})
	, _sampleRate(DefaultSampleRate)
	, _bufferSize(DefaultBufferSize)
	, _inputChannels(0)
	, _outputChannels(0)
	, _streamError(false)
	, _clientName(name)
{
	InitClient();
}

PipeWireNetworkPlayer::~PipeWireNetworkPlayer()
{
	Stop();

	if (_threadLoop)
	{
		pw_thread_loop_stop(_threadLoop);
	}
	if (_core)
	{
		pw_core_disconnect(_core);
		_core = 0;
	}
	if (_context)
	{
		pw_context_destroy(_context);
		_context = 0;
	}
	if (_threadLoop)
	{
		pw_thread_loop_destroy(_threadLoop);
		_threadLoop = 0;
	}
	delete _inputListener;
	delete _outputListener;
	_inputListener = 0;
	_outputListener = 0;
}

void PipeWireNetworkPlayer::InitClient()
{
	if (_core) return;

	pw_init(0, 0);
	_threadLoop = pw_thread_loop_new("clam-pipewire", 0);
	if (!_threadLoop)
	{
		_errorMessage = "Could not create PipeWire thread loop";
		return;
	}

	_context = pw_context_new(pw_thread_loop_get_loop(_threadLoop), 0, 0);
	if (!_context)
	{
		_errorMessage = "Could not create PipeWire context";
		pw_thread_loop_destroy(_threadLoop);
		_threadLoop = 0;
		return;
	}

	_core = pw_context_connect(_context, 0, 0);
	if (!_core)
	{
		_errorMessage = "No PipeWire server available";
		pw_context_destroy(_context);
		_context = 0;
		pw_thread_loop_destroy(_threadLoop);
		_threadLoop = 0;
		return;
	}

	if (pw_thread_loop_start(_threadLoop) < 0)
	{
		_errorMessage = "Could not start PipeWire thread loop";
		pw_core_disconnect(_core);
		_core = 0;
		pw_context_destroy(_context);
		_context = 0;
		pw_thread_loop_destroy(_threadLoop);
		_threadLoop = 0;
	}
}

bool PipeWireNetworkPlayer::IsWorking()
{
	return _core != 0 && !_streamError;
}

std::string PipeWireNetworkPlayer::NonWorkingReason()
{
	if (IsWorking()) return "";
	if (!_errorMessage.empty()) return _errorMessage;
	return "No connection to PipeWire server available";
}

bool PipeWireNetworkPlayer::CreateStream(
	pw_stream ** stream,
	spa_hook * listener,
	const std::string & name,
	const char * category,
	unsigned direction,
	unsigned channels,
	bool input)
{
	if (!channels) return true;

	std::string nodeName = _clientName + (input ? " capture" : " playback");
	pw_properties * properties = pw_properties_new(
		PW_KEY_MEDIA_TYPE, "Audio",
		PW_KEY_MEDIA_CATEGORY, category,
		PW_KEY_MEDIA_ROLE, "Production",
		PW_KEY_NODE_NAME, nodeName.c_str(),
		PW_KEY_NODE_DESCRIPTION, name.c_str(),
		nullptr);
	if (!properties)
	{
		_errorMessage = "Could not create PipeWire stream properties";
		return false;
	}
	const std::string & target = input ? _inputTarget : _outputTarget;
	if (!target.empty())
		pw_properties_set(properties, PW_KEY_TARGET_OBJECT, target.c_str());

	*stream = pw_stream_new(_core, name.c_str(), properties);
	if (!*stream)
	{
		_errorMessage = "Could not create PipeWire stream";
		return false;
	}

	const struct pw_stream_events & events = input ?
		inputStreamEvents() : outputStreamEvents();
	pw_stream_add_listener(*stream, listener, &events, this);

	uint8_t formatBuffer[1024];
	spa_pod_builder builder = SPA_POD_BUILDER_INIT(formatBuffer, sizeof(formatBuffer));
	spa_audio_info_raw audioInfo = SPA_AUDIO_INFO_RAW_INIT(
		SPA_AUDIO_FORMAT_F32,
		SPA_AUDIO_FLAG_UNPOSITIONED,
		_sampleRate,
		channels);
	const spa_pod * format = spa_format_audio_raw_build(
		&builder, SPA_PARAM_EnumFormat, &audioInfo);
	if (!format)
	{
		_errorMessage = "Could not build PipeWire audio format";
		pw_stream_destroy(*stream);
		*stream = 0;
		return false;
	}

	const spa_pod * params[] = { format };
	const pw_stream_flags flags = static_cast<pw_stream_flags>(
		PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS);
	int result = pw_stream_connect(
		*stream,
		static_cast<enum pw_direction>(direction),
		PW_ID_ANY,
		flags,
		params,
		1);
	if (result < 0)
	{
		_errorMessage = "Could not connect PipeWire stream: " +
			std::string(std::strerror(-result));
		pw_stream_destroy(*stream);
		*stream = 0;
		return false;
	}
	return true;
}

bool PipeWireNetworkPlayer::StreamsReady() const
{
	if (_streamError) return false;
	if (_inputStream)
	{
		const enum pw_stream_state state = pw_stream_get_state(_inputStream, 0);
		if (state != PW_STREAM_STATE_PAUSED && state != PW_STREAM_STATE_STREAMING)
			return false;
	}
	if (_outputStream)
	{
		const enum pw_stream_state state = pw_stream_get_state(_outputStream, 0);
		if (state != PW_STREAM_STATE_PAUSED && state != PW_STREAM_STATE_STREAMING)
			return false;
	}
	return true;
}

void PipeWireNetworkPlayer::DestroyStreams()
{
	if (_outputStream)
	{
		pw_stream_destroy(_outputStream);
		_outputStream = 0;
	}
	if (_inputStream)
	{
		pw_stream_destroy(_inputStream);
		_inputStream = 0;
	}
}

void PipeWireNetworkPlayer::Start()
{
	if (IsPlaying()) return;
	if (IsPaused())
	{
		BePlaying();
		return;
	}
	if (!IsWorking()) return;

	CacheSourcesAndSinks();
	_inputChannels = GetNSources();
	_outputChannels = GetNSinks();
	_inputBuffers.assign(_inputChannels, std::vector<float>(MaximumBufferSize, 0.0f));
	_outputBuffers.assign(_outputChannels, std::vector<float>(MaximumBufferSize, 0.0f));
	for (unsigned i = 0; i < _inputChannels; ++i)
		SetSourceFrameSize(i, _bufferSize);
	for (unsigned i = 0; i < _outputChannels; ++i)
		SetSinkFrameSize(i, _bufferSize);

	_streamError = false;
	_errorMessage.clear();
	pw_thread_loop_lock(_threadLoop);
	// PipeWire directions describe the application stream: capture consumes
	// from a source, while playback produces data to a sink.
	bool connected = CreateStream(
		&_inputStream, _inputListener, _clientName + " capture",
		"Capture", PW_DIRECTION_INPUT, _inputChannels, true);
	if (connected)
		connected = CreateStream(
			&_outputStream, _outputListener, _clientName + " playback",
			"Playback", PW_DIRECTION_OUTPUT, _outputChannels, false);

	while (connected && !StreamsReady())
	{
		if (pw_thread_loop_timed_wait(_threadLoop, 5) < 0)
		{
			_errorMessage = "Timed out waiting for PipeWire streams";
			connected = false;
			break;
		}
	}
	pw_thread_loop_unlock(_threadLoop);

	if (!connected)
	{
		pw_thread_loop_lock(_threadLoop);
		DestroyStreams();
		pw_thread_loop_unlock(_threadLoop);
		std::cerr << "PipeWire backend failed to start: "
			<< _errorMessage << std::endl;
		return;
	}

	std::cout << "PipeWire backend active: "
		<< _inputChannels << " capture channel(s), "
		<< _outputChannels << " playback channel(s)"
		<< " [capture node "
		<< (_inputStream ? std::to_string(pw_stream_get_node_id(_inputStream)) : "none")
		<< ", playback node "
		<< (_outputStream ? std::to_string(pw_stream_get_node_id(_outputStream)) : "none")
		<< "]" << std::endl;
	BePlaying();
}

void PipeWireNetworkPlayer::Stop()
{
	if (IsStopped()) return;
	BeStopped();
	if (_threadLoop)
	{
		pw_thread_loop_lock(_threadLoop);
		DestroyStreams();
		pw_thread_loop_unlock(_threadLoop);
	}
}

void PipeWireNetworkPlayer::InputProcessCallback(void * data)
{
	static_cast<PipeWireNetworkPlayer *>(data)->ProcessInput();
}

void PipeWireNetworkPlayer::OutputProcessCallback(void * data)
{
	static_cast<PipeWireNetworkPlayer *>(data)->ProcessOutput();
}

void PipeWireNetworkPlayer::OnStreamStateChanged(int state, const char * error)
{
	if (state == PW_STREAM_STATE_ERROR)
	{
		_streamError = true;
		_errorMessage = "PipeWire stream error: ";
		_errorMessage += error ? error : "unknown error";
		std::cerr << _errorMessage << std::endl;
	}
	if (_threadLoop)
		pw_thread_loop_signal(_threadLoop, false);
}

void PipeWireNetworkPlayer::ProcessInput()
{
	pw_buffer * buffer = pw_stream_dequeue_buffer(_inputStream);
	if (!buffer || !buffer->buffer || !buffer->buffer->n_datas)
		return;

	spa_data & data = buffer->buffer->datas[0];
	if (!data.data || !data.chunk)
	{
		pw_stream_queue_buffer(_inputStream, buffer);
		return;
	}

	const unsigned frameBytes = _inputChannels * sizeof(float);
	const unsigned stride = data.chunk->stride > 0 ?
		static_cast<unsigned>(data.chunk->stride) : frameBytes;
	const unsigned availableFrames = stride ? data.chunk->size / stride : 0;
	const unsigned nframes = std::min(availableFrames, MaximumBufferSize);
	const char * raw = static_cast<const char *>(data.data) + data.chunk->offset;
	const float * interleaved = reinterpret_cast<const float *>(raw);
	for (unsigned frame = 0; frame < nframes; ++frame)
		for (unsigned channel = 0; channel < _inputChannels; ++channel)
			_inputBuffers[channel][frame] = interleaved[frame * _inputChannels + channel];
	for (unsigned channel = 0; channel < _inputChannels; ++channel)
		std::fill(_inputBuffers[channel].begin() + nframes, _inputBuffers[channel].end(), 0.0f);
	pw_stream_queue_buffer(_inputStream, buffer);

	if (!_outputStream && !IsPaused() && nframes)
		ProcessNetwork(nframes);
}

void PipeWireNetworkPlayer::ProcessOutput()
{
	pw_buffer * buffer = pw_stream_dequeue_buffer(_outputStream);
	if (!buffer || !buffer->buffer || !buffer->buffer->n_datas)
		return;

	spa_data & data = buffer->buffer->datas[0];
	if (!data.data || !data.chunk)
	{
		pw_stream_queue_buffer(_outputStream, buffer);
		return;
	}

	const unsigned frameBytes = _outputChannels * sizeof(float);
	const unsigned stride = data.chunk->stride > 0 ?
		static_cast<unsigned>(data.chunk->stride) : frameBytes;
	const unsigned availableBytes = data.maxsize > data.chunk->offset ?
		data.maxsize - data.chunk->offset : 0;
	const unsigned maxFrames = stride ? availableBytes / stride : 0;
	const unsigned requestedFrames = buffer->requested ?
		static_cast<unsigned>(buffer->requested) : maxFrames;
	const unsigned nframes = std::min(
		std::min(requestedFrames, maxFrames), MaximumBufferSize);
	if (!nframes)
	{
		pw_stream_queue_buffer(_outputStream, buffer);
		return;
	}

	if (IsPaused())
	{
		std::memset(
			static_cast<char *>(data.data) + data.chunk->offset,
			0,
			nframes * frameBytes);
	}
	else
	{
		ProcessNetwork(nframes);
		char * raw = static_cast<char *>(data.data) + data.chunk->offset;
		float * interleaved = reinterpret_cast<float *>(raw);
		for (unsigned frame = 0; frame < nframes; ++frame)
			for (unsigned channel = 0; channel < _outputChannels; ++channel)
				interleaved[frame * _outputChannels + channel] =
					_outputBuffers[channel][frame];
	}
	data.chunk->size = nframes * frameBytes;
	data.chunk->stride = frameBytes;
	buffer->size = nframes;
	pw_stream_queue_buffer(_outputStream, buffer);
}

void PipeWireNetworkPlayer::ProcessNetwork(unsigned nframes)
{
	if (nframes > MaximumBufferSize) return;
	for (unsigned i = 0; i < _inputChannels; ++i)
		SetSourceBuffer(i, _inputBuffers[i].data(), nframes);
	for (unsigned i = 0; i < _outputChannels; ++i)
		SetSinkBuffer(i, _outputBuffers[i].data(), nframes);
	GetNetwork().Do();
}

} // namespace CLAM

#endif // USE_PIPEWIRE
