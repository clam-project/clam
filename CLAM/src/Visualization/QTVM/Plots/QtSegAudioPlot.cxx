#include "SegAudioPlotController.hxx"
#include "QtSegAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtSegAudioPlot::QtSegAudioPlot(QWidget* parent) : QtAudioPlot(parent)
		{
			SetPlotController();
			Connect();
		}

		QtSegAudioPlot::~QtSegAudioPlot()
		{
		}

		void QtSegAudioPlot::SetData(const Audio& audio,std::list<unsigned>& marks)
		{
			((SegAudioPlotController*)_controller)->SetData(audio,marks);
			QtAudioPlot::SetPData(audio);
		}

		void QtSegAudioPlot::SetMarksColor(Color c)
		{
			((SegAudioPlotController*)_controller)->SetMarksColor(c);
		}

		void QtSegAudioPlot::SetData(const Audio& audio)
		{
			// Empty body for convenience 
		}

		void QtSegAudioPlot::SetPlotController()
		{
			SetController(new SegAudioPlotController());
		}

		void QtSegAudioPlot::Connect()
		{
			// Connections
			connect(((SegAudioPlotController*)_controller),SIGNAL(xRulerScaleDiv(QwtScaleDiv)),this,SLOT(setXRulerScaleDiv(QwtScaleDiv)));
			connect(((SegAudioPlotController*)_controller),SIGNAL(yRulerScaleDiv(QwtScaleDiv)),this,SLOT(setYRulerScaleDiv(QwtScaleDiv)));
			connect(((SegAudioPlotController*)_controller),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
		}

		void QtSegAudioPlot::DisplayBackgroundBlack()
		{
			QtAudioPlot::DisplayBackgroundBlack();
			SetMarksColor(VMColor::Orange());
		}

		void QtSegAudioPlot::DisplayBackgroundWhite()
		{
			QtAudioPlot::DisplayBackgroundWhite();
			SetMarksColor(VMColor::Orange());
		}
	}	
}

// END

