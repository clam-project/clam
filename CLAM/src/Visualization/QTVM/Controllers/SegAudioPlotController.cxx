#include "SegAudioPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		SegAudioPlotController::SegAudioPlotController()
		{
		}

		SegAudioPlotController::~SegAudioPlotController()
		{
		}

		void SegAudioPlotController::SetData(const Audio& audio,std::list<unsigned>& marks)
		{
			AudioPlotController::SetData(audio);
			_marks = marks;
			FullView();
			_marksRenderer.SetVBounds(_view.top,_view.bottom);
		}

		void SegAudioPlotController::SetMarksColor(Color c)
		{
			_marksRenderer.SetColor(c);
		}

		void SegAudioPlotController::Draw()
		{
			if(MustProcessData()) ProcessMarksData();
			AudioPlotController::Draw();
			_marksRenderer.Render();
		}

		void SegAudioPlotController::ProcessMarksData()
		{
			unsigned left = unsigned(GetLeftBound());
			unsigned right = unsigned(GetRightBound());

			Array<unsigned> processedMarks;

			Marks::iterator it = _marks.begin();
			for(;it != _marks.end();it++)
			{
				if((*it) > right) break;
				if((*it) >= left) processedMarks.AddElem((*it));
			}

			_marksRenderer.SetHBounds(GetLeftBound(),GetRightBound());
			_marksRenderer.SetVBounds(GetBottomBound(),GetTopBound());
			_marksRenderer.SetData(processedMarks);
		}
	}
}

// END

