#include "SDIFCollection.hxx"
#include "SDIFFile.hxx"

using namespace SDIF;

int main()
{
	int error = 0;
	int valsread=0,valswritten = 0;
	
	Collection wc;
	Collection rc;
	
	float v = 1.1;
	float t = 0;
	
	{
		for (int i=0;i<100;i++)
		{
			Frame *frame = new Frame("1TST",t);
			int w = rand()&15;
			int h = rand()&15;
			ConcreteMatrix<TFloat32> *matrix = 
				new ConcreteMatrix<TFloat32>("1TST",h,w);

			for (int r=0;r<h;r++)
			{
				for (int c=0;c<w;c++)
				{
					matrix->SetValue(r,c,v);
					valswritten++;
				}
			}
			v+=1.1;
			t+=0.1;

			frame->Add(matrix);

			wc.Add(frame);
		}
	
		File f("test.sdif",File::eOutput);
		f.Open();
		f.Write(wc);
		f.Close();
	}	
	
	{	
		File f("test.sdif",File::eInput);
		
		f.Open();
		f.Read(rc);
		f.Close();
	}
	
	Collection::FrameIterator rit = rc.Begin();
	Collection::FrameIterator rend = rc.End();
	
	Collection::FrameIterator wit = wc.Begin();
	Collection::FrameIterator wend = wc.End();

	v = 1.1;
	t = 0;
		
	while (rit!=rend && wit!=wend)
	{
		Frame *rf = *rit;
		Frame *wf = *wit;
		
		Frame::MatrixIterator rfit = rf->Begin();
		Frame::MatrixIterator rfend = rf->End();

		Frame::MatrixIterator wfit = wf->Begin();
		Frame::MatrixIterator wfend = wf->End();
		
		if (rf->Time()!=t)
		{
			printf("read frame time != reference time\n");
			error++;
		}
		if (wf->Time()!=t)
		{
			printf("read frame time != reference time\n");
			error++;
		}
		
		while (rfit!=rfend && wfit!=wfend)
		{
			Matrix* rm = *rfit;
			Matrix* wm = *wfit;
			
			//printf("%d %d %f\n",rm->mHeader.mnRows,rm->mHeader.mnColumns,
			//	rm->GetValue(0,0));
			//printf("%d %d %f\n",wm->mHeader.mnRows,wm->mHeader.mnColumns,
			//wm->GetValue(0,0));

			if (rm->Rows()!=wm->Rows() || rm->Columns()!=wm->Columns()) 
			{
				printf("matrix header mismatch\n");
				error++;
			}else	for (int j=0;j<wm->Rows();j++)
			{
				for (int i=0;i<wm->Columns();i++)
				{
					if (
						dynamic_cast< ConcreteMatrix<TFloat32>* >(rm)->GetValue(j,i) != 
						dynamic_cast< ConcreteMatrix<TFloat32>* >(wm)->GetValue(j,i)
					)
					{
						printf("read matrix value != write matrix value\n");
						error++;
					}
					if (
						dynamic_cast< ConcreteMatrix<TFloat32>* >(rm)->GetValue(j,i) != v
					)
					{
						printf("read matrix value != reference value\n");
						error++;
					}else
					if (
						dynamic_cast< ConcreteMatrix<TFloat32>* >(wm)->GetValue(j,i) != v
					)
					{
						printf("write matrix value != reference value\n");
						error++;
					}else{
						valsread++;
					}
				}
			}
		
			rfit++;
			wfit++;
		}

		if (rfit!=rfend)
		{
			printf("ERROR: read frame size > write frame size\n");
			error++;
		}
		else if (wfit!=wfend)
		{
			printf("ERROR: read frame size < write frame size\n");
			error++;
		}
		
		rit++;
		wit++;

		v+=1.1;
		t+=0.1;
	}	
	
	if (rit!=rend)
	{
		printf("ERROR: read collection size > write collection size\n");
		error++;
	}
	else if (wit!=wend)
	{
		printf("ERROR: read collection size < write collection size\n");
		error++;
	}
	
	printf("%d errors encountered, %d/%d values matched\n",error,
		valsread,valswritten);

	return 0;
}
