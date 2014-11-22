#include <FakeIt/DefaultFakeit.hpp>
#include <Catch/catch.hpp>

namespace fakeit
{
   struct CatchAdapter : public EventHandler
   {
      virtual ~CatchAdapter() = default;
      CatchAdapter(EventFormatter &formatter) : _formatter(formatter) {}

      virtual void handle(const UnexpectedMethodCallEvent &evt) override
      {
         std::string format = _formatter.format(evt);
         CATCH_INTERNAL_ERROR(format);
      }

      virtual void handle(const SequenceVerificationEvent &evt) override
      {
         std::string format(_formatter.format(evt));
         Catch::throwLogicError(format, ::Catch::SourceLineInfo(evt.file(), evt.line()));
      }

      virtual void handle(const NoMoreInvocationsVerificationEvent &evt) override
      {
         std::string format = _formatter.format(evt);
         Catch::throwLogicError(format, ::Catch::SourceLineInfo(evt.file(), evt.line()));
      }

   private:
      EventFormatter &_formatter;
   };

   class CatchFakeit : public DefaultFakeit
   {
   public:
      virtual ~CatchFakeit() = default;
      CatchFakeit() : _catchAdapter(*this) {}

      static CatchFakeit &getInstance()
      {
         static CatchFakeit instance;
         return instance;
      }

   protected:
      fakeit::EventHandler &accessTestingFrameworkAdapter() override
      {
         return _catchAdapter;
      }

   private:
      CatchAdapter _catchAdapter;
   };
}

static fakeit::DefaultFakeit& Fakeit = fakeit::CatchFakeit::getInstance();