#ifndef __NETSPECTRUMPLOT__
#define __NETSPECTRUMPLOT__

#include "Spectrum.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	class SpectrumPortMonitor;

	namespace VM
	{
		class CLAM_DLL_EXPORT NetSpectrumPlot : public NetPlot
		{
			Q_OBJECT
		public:
			typedef SpectrumPortMonitor MonitorType;
			NetSpectrumPlot(QWidget* parent=0, const char * name=0);
			~NetSpectrumPlot();

			void SetData(const Spectrum& spec);
			void SetDataColor(Color c);
			void SetMonitor(MonitorType & monitor);

		protected:
			void SetPlotController();

		private:
			void InitNetSpectrumPlot();
		};
	}
}

#endif

