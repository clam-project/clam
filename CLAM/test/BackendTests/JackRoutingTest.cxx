#include "JACKNetworkPlayer.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <regex>
#include <set>
#include <stdexcept>

// A small JACK graph exercises the real player without requiring a server.
struct _jack_client { std::string name; };
struct _jack_port
{
	std::string name, shortName, type;
	unsigned long flags;
	jack_client_t * owner;
	float buffer[512] = {};
};
static _jack_client client;
static std::map<std::string, std::unique_ptr<_jack_port>> ports;
static std::set<std::pair<std::string, std::string>> connections;
static unsigned queries = 0, allocations = 0;

static void check(bool condition, const char * message)
{
	if (!condition) throw std::runtime_error(message);
}

static const char ** namesArray(const std::vector<std::string> & names)
{
	if (names.empty()) return nullptr;
	size_t size = (names.size() + 1) * sizeof(char *);
	for (const auto & name : names) size += name.size() + 1;
	auto result = static_cast<const char **>(std::calloc(1, size));
	char * text = reinterpret_cast<char *>(result + names.size() + 1);
	for (size_t i = 0; i < names.size(); ++i)
	{
		result[i] = text;
		std::strcpy(text, names[i].c_str());
		text += names[i].size() + 1;
	}
	++allocations;
	return result;
}

extern "C"
{
jack_client_t * jack_client_open(const char * name, jack_options_t, jack_status_t * status, ...)
{ client.name = name; *status = static_cast<jack_status_t>(0); return &client; }
int jack_client_name_size() { return 64; }
int jack_client_close(jack_client_t *) { return 0; }
int jack_set_process_callback(jack_client_t *, JackProcessCallback, void *) { return 0; }
void jack_on_shutdown(jack_client_t *, JackShutdownCallback, void *) {}
jack_nframes_t jack_get_sample_rate(jack_client_t *) { return 48000; }
jack_nframes_t jack_get_buffer_size(jack_client_t *) { return 512; }
int jack_activate(jack_client_t *) { return 0; }
int jack_deactivate(jack_client_t *) { return 0; }
jack_port_t * jack_port_register(jack_client_t * owner, const char * name,
	const char * type, unsigned long flags, unsigned long)
{
	auto port = std::make_unique<_jack_port>();
	port->shortName = name;
	port->name = owner ? owner->name + ":" + name : name;
	port->type = type; port->flags = flags; port->owner = owner;
	auto result = port.get();
	ports[port->name] = std::move(port);
	return result;
}
int jack_port_unregister(jack_client_t *, jack_port_t * port)
{
	for (auto it = connections.begin(); it != connections.end();)
		if (it->first == port->name || it->second == port->name) it = connections.erase(it);
		else ++it;
	ports.erase(port->name);
	return 0;
}
const char * jack_port_name(const jack_port_t * port) { return port->name.c_str(); }
const char * jack_port_short_name(const jack_port_t * port) { return port->shortName.c_str(); }
const char * jack_port_type(const jack_port_t * port) { return port->type.c_str(); }
int jack_port_flags(const jack_port_t * port) { return port->flags; }
int jack_port_is_mine(const jack_client_t * owner, const jack_port_t * port) { return port->owner == owner; }
void * jack_port_get_buffer(jack_port_t * port, jack_nframes_t) { return port->buffer; }
jack_port_t * jack_port_by_name(jack_client_t *, const char * name)
{
	auto it = ports.find(name);
	return it == ports.end() ? nullptr : it->second.get();
}
const char ** jack_get_ports(jack_client_t *, const char * pattern, const char * type, unsigned long flags)
{
	++queries;
	check(pattern && *pattern, "Empty patterns must never enumerate all JACK ports");
	check(type && std::strcmp(type, JACK_DEFAULT_AUDIO_TYPE) == 0, "Pattern discovery must filter audio ports");
	std::vector<std::string> names;
	for (const auto & entry : ports)
		if ((entry.second->flags & flags) && entry.second->type == type &&
			std::regex_search(entry.first, std::regex(pattern))) names.push_back(entry.first);
	return namesArray(names);
}
const char ** jack_port_get_connections(const jack_port_t * port)
{
	std::vector<std::string> names;
	for (const auto & connection : connections)
		if (connection.first == port->name) names.push_back(connection.second);
		else if (connection.second == port->name) names.push_back(connection.first);
	return namesArray(names);
}
int jack_connect(jack_client_t * owner, const char * from, const char * to)
{
	auto source = jack_port_by_name(owner, from), sink = jack_port_by_name(owner, to);
	if (!source || !sink || !(source->flags & JackPortIsOutput) ||
		!(sink->flags & JackPortIsInput) || source->type != sink->type) return EINVAL;
	return connections.emplace(from, to).second ? 0 : EEXIST;
}
void jack_free(void * memory) { --allocations; std::free(memory); }
}

struct Fixture
{
	CLAM::Network network;
	CLAM::JACKNetworkPlayer * player;
	Fixture()
	{
		check(allocations == 0, "Previous player leaked saved JACK connections");
		ports.clear(); connections.clear(); queries = 0;
		for (const auto & name : {"interface[USB]:capture_1", "interface[USB]:capture_2"})
			jack_port_register(nullptr, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
		for (const auto & name : {"speakers:playback_1", "headphones:playback_2"})
			jack_port_register(nullptr, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
		jack_port_register(nullptr, "keyboard:midi", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
		CLAM::AudioSource::Config source; source.SetNSources(2);
		CLAM::AudioSink::Config sink; sink.SetNSinks(2);
		network.AddProcessing("Capture", "AudioSource").Configure(source);
		network.AddProcessing("Playback", "AudioSink").Configure(sink);
		network.ConnectPorts("Capture.1", "Playback.1");
		network.ConnectPorts("Capture.2", "Playback.2");
		player = new CLAM::JACKNetworkPlayer("routing-test");
		network.SetPlayer(player);
	}
	void restart() { player->Stop(); player->Start(); }
	bool connected(const char * from, const char * to) const { return connections.count({from, to}); }
};

int main()
{
	CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = false;
	try
	{
		{
			Fixture f;
			f.player->SetAutoConnectPatterns("", ""); f.player->Start();
			check(connections.empty() && queries == 0, "Blank settings must keep manual routing");
		}
		{
			Fixture f;
			f.player->SetAutoConnectPatterns("", "^(speakers|headphones):"); f.player->Start();
			check(connections.size() == 2 && queries == 1, "Playback-only pattern must not auto-connect capture");
			jack_connect(&client, "interface[USB]:capture_1", "routing-test:Capture_1");
			f.restart();
			check(connections.size() == 3 && f.connected("interface[USB]:capture_1", "routing-test:Capture_1"),
				"Manual capture must survive playback auto-connect restart");
		}
		{
			Fixture f;
			f.player->SetAutoConnectPatterns("capture_", ""); f.player->Start();
			check(connections.size() == 2 && queries == 1, "Capture-only pattern must route both input channels");
			check(f.connected("interface[USB]:capture_2", "routing-test:Capture_2"), "Capture pattern direction is reversed");
		}
		{
			Fixture f;
			f.player->SetPortConnections({{"Capture_1", "interface[USB]:capture_2"}, {"Capture_2", "interface[USB]:capture_1"}},
				{{"Playback_1", "headphones:playback_2"}, {"Playback_2", "speakers:playback_1"}});
			f.player->Start(); f.restart();
			check(connections.size() == 4 && queries == 0, "Exact channel mappings must not use regex matching");
			check(f.connected("interface[USB]:capture_2", "routing-test:Capture_1") &&
				f.connected("routing-test:Playback_1", "headphones:playback_2"), "Independent channel mappings failed");
			f.player->Stop();
			f.player->SetAutoConnectPatterns("capture_", ""); f.player->Start();
			check(f.connected("interface[USB]:capture_1", "routing-test:Capture_1"), "Patterns must replace previous explicit mappings");
		}
		{
			Fixture f;
			f.player->SetPortConnections({{"Capture_1", "missing:port"}, {"Capture_2", "interface[USB]:capture_2"}},
				{{"Playback_1", "keyboard:midi"}, {"Playback_2", "interface[USB]:capture_1"}});
			f.player->Start();
			check(connections.size() == 1 && f.connected("interface[USB]:capture_2", "routing-test:Capture_2"),
				"Missing or incompatible ports must not shift mappings or connect other channels");
		}
		{
			Fixture f;
			f.player->SetPortConnections({}, {{"Playback_1", "headphones:playback_2"}, {"Playback_2", ""}});
			f.player->Start();
			jack_connect(&client, "routing-test:Playback_2", "speakers:playback_1");
			f.restart();
			check(connections.size() == 2 && f.connected("routing-test:Playback_2", "speakers:playback_1"),
				"Manual channels must survive restart alongside mapped channels");
		}
		check(allocations == 0, "Player destruction leaked saved JACK connections");
		std::cout << "JACK routing checks passed\n";
	}
	catch (const std::exception & error) { std::cerr << error.what() << '\n'; return 1; }
	return 0;
}
