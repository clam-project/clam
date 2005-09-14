#ifndef __NETPEAKSPLOT__
#define __NETPEAKSPLOT__

#include "SpectralPeakArray.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	class PeaksPortMonitor;

	namespace VM
	{
		class CLAM_DLL_EXPORT NetPeaksPlot : public NetPlot
		{
			Q_OBJECT
		public:
			typedef PeaksPortMonitor MonitorType;
			NetPeaksPlot(QWidget* parent=0, const char * name=0);
			~NetPeaksPlot();

			void SetData(const SpectralPeakArray& peaks);
			void SetPeaksColor(Color cline, Color cpoint);
			void SetMonitor(MonitorType & monitor);

		protected:
			void SetPlotController();

		private:
			void InitNetPeaksPlot();
		};
	}
}

#endif

