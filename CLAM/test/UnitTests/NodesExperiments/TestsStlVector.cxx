
#include "MiniCppUnit.hxx"
#include <vector>
#include <list>
#include <deque>

class TestsStlVector : public GrupDeTests<TestsStlVector>
{
public:
	GRUP_DE_TESTS(TestsStlVector)
	{
		CAS_DE_TEST( testReserve );
		CAS_DE_TEST( testAt );
		CAS_DE_TEST( testResize_beforeReserve );
		CAS_DE_TEST( testList_isCircularWithPhantomElement );
		CAS_DE_TEST( testDeque_elementsNotInContiguousSpace );
		CAS_DE_TEST( testVectorInsertInTheMiddle );
	}

	void testReserve()
	{
		std::vector<int> vector;
		vector.reserve(5);
		ASSERT(0 == vector.size());

	}
	void testAt()
	{
		std::vector<int> vector;
		vector.reserve(5);
		try {
			vector.at(1); // en canvi l'operator[] no fa comprovacions
			FALLA("hauria d'haver llençat exception"); //funciona tant en debug com en release
		} catch (std::exception&)
		{
		}
	}

	void testResize_beforeReserve()
	{
		std::vector<int> vector;
		vector.resize(5);
		ASSERT(5 == vector.size());
		ASSERT(5 <= vector.capacity());

		vector.at(4)=1;
		ASSERT(1==vector.at(4));
	}

	void testList_isCircularWithPhantomElement()
	{
		typedef std::list<int> List;
		List list;
		list.push_back(0);
		list.push_back(1);
		list.push_back(2);

		List::iterator it=list.begin();
		for(int i=0; i<3; i++,it++);

		ASSERT(it==list.end());

		it++; // we are out of range: let's see that magically we're in the beginning
		ASSERT(it!=list.end());
		(*it)=66;
		ASSERT(66==*list.begin());

	}

	void testDeque_elementsNotInContiguousSpace()
	{
		std::deque<int> deque;
		
		deque.push_back(1);
		deque.push_back(2);
		deque.push_back(3);
		deque.push_back(4);
		deque.push_back(5);
		deque.push_front(0);

		ASSERT_IGUALS(5, deque.at(5));
		ASSERT(5 != *(&(deque.at(0))+5) );
	}

	void testVectorInsertInTheMiddle()
	{
		std::vector<char> buff;
		buff.resize(8);
		buff[0]='h';
		buff[1]='e';
		buff[2]='o';
		buff[3]=' ';
		buff[4]='a';
		buff[5]='l';
		buff[6]='l';
		buff[7]='\0';
		std::vector<char>::iterator it  = buff.begin();
		it += 2;
		buff.insert(it, 2, 'l');

		ASSERT_IGUALS( 10, int(buff.size()) );
		ASSERT_IGUALS( "hello all", (char *)(&buff[0]));
	}
};

int main()
{
	TestsStlVector().testeja();
	return 0;
}
