#ifndef __BOUNDINGBOXES__
#define __BOUNDINGBOXES__

namespace CLAMVM
{

struct DataBoundBox
{
		float mLeft;
		float mRight;
		float mBottom;
		float mTop;
		float mNear;
		float mFar;				
};

struct ScreenBoundBox
{
		int pixel_width;
		int pixel_height;
};

}

#endif // BoundingBoxes.hxx
