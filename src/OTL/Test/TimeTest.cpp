#include <OTL/Test/BaseTest.h>
#include <OTL/Core/Time.h>

const double TOL_PCT = 0.1;
#define OTL_APPROX(x) Approx(x, TOL_PCT)

struct SomeInterface
{
   virtual int foo(int) = 0;
   virtual int bar(int, int) = 0;
};

TEST_CASE("Time", "Time")
{
   ////auto time = otl::Time::Days(1);

   //fakeit::Mock<otl::Vector3d> mock;
   //fakeit::When(Method(mock, norm)).Return(1.0);

   //otl::Vector3d& i = mock.get();

   //double result = i.norm();

   //fakeit::Verify(Method(mock, norm)).Exactly(1);



   //fakeit::Mock<SomeInterface> mock;
   //fakeit::When(Method(mock, foo(int))).AlwaysReturn(1);

   //SomeInterface& i = mock.get();

   //i.foo(1);
   //i.foo(2);
   //i.foo(3);

   //fakeit::Verify(Method(mock, foo)).AtLeastOnce();
   //fakeit::Verify(Method(mock, foo)).Exactly(3);
   //fakeit::Verify(Method(mock, foo).Using(1)).Once();
}