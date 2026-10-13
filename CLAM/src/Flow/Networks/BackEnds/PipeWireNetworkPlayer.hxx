#ifndef PIPEWIRENETWORKPLAYER_HXX
#define PIPEWIRENETWORKPLAYER_HXX

#include "NetworkPlayer.hxx"

#include <cstdint>
#include <string>
#include <vector>

struct pw_context;
struct pw_core;
struct pw_stream;
struct pw_thread_loop;
struct spa_hook;

namespace CLAM
{

/**
 * Network player backed by native PipeWire streams.
 *
 * The player exposes the network's audio sources and sinks as the channels of
 * PipeWire capture and playback streams. PipeWire performs the connection to
 * the default source/sink and any required sample-rate or quantum conversion.
 */
class PipeWireNetworkPlayer : public NetworkPlayer
{
private:
	static constexpr unsigned DefaultSampleRate = 44100;
	static constexpr unsigned DefaultBufferSize = 512;
	static constexpr unsigned MaximumBufferSize = 8192;

	pw_thread_loop * _threadLoop;
	pw_context * _context;
	pw_core * _core;
	pw_stream * _inputStream;
	pw_stream * _outputStream;
	spa_hook * _inputListener;
	spa_hook * _outputListener;

	unsigned _sampleRate;
	unsigned _bufferSize;
	unsigned _inputChannels;
	unsigned _outputChannels;
	std::vector<std::vector<float> > _inputBuffers;
	std::vector<std::vector<float> > _outputBuffers;

	bool _streamError;
	std::string _errorMessage;
	std::string _clientName;
	std::string _inputTarget;
	std::string _outputTarget;

	void InitClient();
	bool CreateStream(
		pw_stream ** stream,
		spa_hook * listener,
		const std::string & name,
		const char * category,
		unsigned direction,
		unsigned channels,
		bool input);
	bool StreamsReady() const;
	void DestroyStreams();
	void ProcessInput();
	void ProcessOutput();
	void ProcessNetwork(unsigned nframes);

public:
	PipeWireNetworkPlayer(const std::string & name = "CLAM network player");
	virtual ~PipeWireNetworkPlayer();
	static void InputProcessCallback(void * data);
	static void OutputProcessCallback(void * data);
	void OnStreamStateChanged(int state, const char * error);
	void SetInputTarget(const std::string & target) { _inputTarget = target; }
	void SetOutputTarget(const std::string & target) { _outputTarget = target; }

	bool IsWorking();
	std::string NonWorkingReason();
	virtual bool IsRealTime() const { return true; }
	virtual void Start();
	virtual void Stop();
	virtual void Init() { InitClient(); }

	virtual unsigned BackendBufferSize() { return _bufferSize; }
	virtual unsigned BackendSampleRate() { return _sampleRate; }
};

} // namespace CLAM

#endif
