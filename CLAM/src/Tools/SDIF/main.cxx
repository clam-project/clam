#include "SDIFCollection.hxx"
#include "SDIFFile.hxx"

using namespace SDIF;

int main()
{
	int error = 0;
	int nrefvalsmatched = 0;
	
	Collection wc;
	Collection rc;
	
	float v = 1.1;
	float t = 0;
	
	{
		for (int i=0;i<100;i++)
		{
			Frame *frame = new Frame("1FQ0",t);
			Matrix *matrix = new Matrix("1FQ0",eFloat32,1,1);

			matrix->SetValue(0,0,v);

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
	
	Collection::iterator rit = rc.Begin();
	Collection::iterator rend = rc.End();
	
	Collection::iterator wit = wc.Begin();
	Collection::iterator wend = wc.End();

	v = 1.1;
	t = 0;
		
	while (rit!=rend && wit!=wend)
	{
		Frame *rf = *rit;
		Frame *wf = *wit;
		
		Frame::iterator rfit = rf->Begin();
		Frame::iterator rfend = rf->End();

		Frame::iterator wfit = wf->Begin();
		Frame::iterator wfend = wf->End();
		
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
			}else	for (int j=0;j<wm->mHeader.mnRows;j++)
			{
				for (int i=0;i<wm->mHeader.mnColumns;i++)
				{
					if (rm->GetValue(j,i) != wm->GetValue(j,i))
					{
						printf("read matrix value != write matrix value\n");
						error++;
					}
					if (rm->GetValue(j,i) != v)
					{
						printf("read matrix value != reference value\n");
						error++;
					}else
					if (wm->GetValue(j,i) != v)
					{
						printf("write matrix value != reference value\n");
						error++;
					}else{
						nrefvalsmatched++;
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
	
	printf("%d errors encountered, %d/100 reference values matched\n",error,nrefvalsmatched);

	return 0;
}
