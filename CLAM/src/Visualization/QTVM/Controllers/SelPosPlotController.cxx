#include "SelPosPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		SelPosPlotController::SelPosPlotController()
		{
		}

		SelPosPlotController::~SelPosPlotController()
		{
		}

		void SelPosPlotController::SetSelPos(const TData& value)
		{
			if(GetSelPos() != value)
			{
				PlotController::SetSelPos(value);
				UpdateDial(GetSelPos());
			}
		}

		void SelPosPlotController::Draw()
		{
			_dial.Render();
		}

		void SelPosPlotController::SetDialColor(Color c)
		{
			_dial.SetColor(c);
		}
				
		void SelPosPlotController::SetHBounds(const TData& left,const TData& right)
		{
			PlotController::SetHBounds(left,right);
			_dial.SetHBounds(GetLeftBound(),GetRightBound());
		}

		void SelPosPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			PlotController::SetVBounds(bottom,top);
			_dial.SetVBounds(GetBottomBound(),GetTopBound());
		}

		void SelPosPlotController::UpdateDial(const TData& value)
		{
			_dial.Update(value);
		}
	}
}

// END
