#include "SKSE/Trampoline.h"
#include "util.h"
#include "editorID.hpp"
#pragma warning(disable: 4100)


namespace hooks
{

	class util
	{
	private:


	public:
		union ConditionParam
		{
			char c;
			std::int32_t i;
			float f;
			RE::TESForm *form;
		};
		static void install();

		template <class T>
		static std::vector<T *> get_all()
		{
			std::vector<T *> result;

			const auto dataHandler = RE::TESDataHandler::GetSingleton();
			for (const auto &form : dataHandler->GetFormArray<T>())
			{
				if (!form)
				{
					continue;
				}
				result.push_back(form);
			}

			return result;
		}
	};
};

// auto a = RE::TESForm::LookupByEditorID<RE::BGSSoundDescriptorForm>("aaaUDDragonAISpell");
