#ifndef __NETSPECGRAMPLOT__
#define __NETSPECGRAMPLOT__

#include "Spectrum.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
    class SpecgramPortMonitor;

    namespace VM
    {
		class CLAM_DLL_EXPORT NetSpecgramPlot : public NetPlot
		{
			Q_OBJECT
		public:
			typedef SpecgramPortMonitor MonitorType;
			NetSpecgramPlot(QWidget* parent = 0, const char * name = 0);
			~NetSpecgramPlot();

			void SetData(const Spectrum& spec);
			void SetMonitor(MonitorType & monitor);

		protected:
			void SetPlotController();

		private:
			void InitNetSpecgramPlot();
		};
    }
}

#endif

