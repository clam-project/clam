#ifndef __CLAMVM_PLOTAUDIO_HXX
#define __CLAMVM_PLOTAUDIO_HXX

// forward declaration
namespace CLAM
{
	class Audio;
}

namespace CLAMVM
{
	/**
	 * Overload of plot() for CLAM::Audio.
	 */
	void plot(const CLAM::Audio &model, const char *label = "");


} // namespace CLAMVM

#endif // __CLAMVM_PLOTAUDIO_HXX